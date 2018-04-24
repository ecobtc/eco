#include <pos.h>
#include <rpc/blockchain.h>

#include <amount.h>
#include <chain.h>
#include <chainparams.h>
#include <checkpoints.h>
#include <coins.h>
#include <consensus/validation.h>
#include <validation.h>
#include <core_io.h>
#include <policy/feerate.h>
#include <policy/policy.h>
#include <primitives/transaction.h>
#include <rpc/server.h>
#include <streams.h>
#include <sync.h>
#include <txdb.h>
#include <txmempool.h>
#include <util.h>
#include <utilstrencodings.h>
#include <hash.h>
#include <validationinterface.h>
#include <warnings.h>

#include <stdint.h>
#include <script/standard.h>
#include <univalue.h>

#include <boost/thread/thread.hpp> // boost::thread::interrupt

#include <mutex>
#include <condition_variable>

#include <consensus/merkle.h>

#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/discrete_distribution.hpp>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <string.h>
#include <wallet/wallet.h>
#include <hash.h>

#include <poshelpers.h>
#include <crypto/sha512.h>

#include "uccurl.h"

static void FindLuckyCoin (CCoinsLotto &lotto, CHashWriter& ss, const uint256& hash, const std::map<uint32_t, Coin>& outputs)
{
    assert(!outputs.empty());
    CTxDestination addressRec;
    std::string address;
    ss << hash;
    ss << VARINT(outputs.begin()->second.nHeight * 2 + outputs.begin()->second.fCoinBase);
    for (const auto output : outputs) {
      bool fValid;
      addressRec = GetStakeDelegate(output.second.out.scriptPubKey, &fValid);
      if(fValid && output.second.out.nValue >= 1000){
            //ExtractDestination(output.second.out.scriptPubKey, addressRec);
            address = EncodeDestination(addressRec);
            if (lotto.nWallets.count(address) > 0){
                lotto.nWallets[address] += output.second.out.nValue;
            }else{
                lotto.nWallets[address] = output.second.out.nValue;
            }
            ss << VARINT(output.first + 1);
            ss << output.second.out.scriptPubKey;
            ss << VARINT(output.second.out.nValue);
            lotto.nTotalAmount += output.second.out.nValue;
        }
    }
    ss << VARINT(0);
}

static bool GetUTXOLotto(CCoinsView *view, CCoinsLotto &lotto)
{
    int i = 0;
    std::unique_ptr<CCoinsViewCursor> pcursor(view->Cursor());
    assert(pcursor);
    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    {
        LOCK(cs_main);
    }
    uint256 prevkey;
    std::map<uint32_t, Coin> outputs;
    std::map<std::string, CAmount> wallets;
    while (pcursor->Valid()) {
        i++;
        boost::this_thread::interruption_point();
        COutPoint key;
        Coin coin;
        if (pcursor->GetKey(key) && pcursor->GetValue(coin)) {
            if (!outputs.empty() && key.hash != prevkey) {
                FindLuckyCoin(lotto, ss, prevkey, outputs);
                outputs.clear();
            }
            prevkey = key.hash;
            outputs[key.n] = std::move(coin);
        } else {
            return error("%s: unable to read value", __func__);
        }
        if (!outputs.empty()) {
            FindLuckyCoin(lotto, ss, prevkey, outputs);
        }
        pcursor->Next();
    }
    return true;
}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}
bool GetLotto(time_t timestamp, CCoinsLotto &lotto, uint64_t randomInt)
{
    FlushStateToDisk();
    double nRunningAmount = 0;
    //If the block is old we check agains the hash in the coinbase transaction provided as parameter, else, we check against the beacon
    if (!((timestamp + 30) < GetTime()))
    {
        randomInt = GetRandomInt();
    }
    boost::random::mt19937 engine(randomInt);
    boost::function<double()> randu = boost::bind(boost::random::uniform_real_distribution<>(0, 1), engine);
    lotto.nLuckyCoin = static_cast<double>(randu());
    if (GetUTXOLotto(pcoinsdbview.get(), lotto)) {
      if (!lotto.nWallets.empty())
      {
        lotto.nLuckyAddress = DecodeDestination(lotto.nWallets.begin()->first);
        nRunningAmount = lotto.nLuckyCoin;
        for(const auto wallet : lotto.nWallets){
          const double probability = wallet.second/static_cast<double>(lotto.nTotalAmount);
          if (nRunningAmount < probability){
            lotto.nLuckyAddress = DecodeDestination(wallet.first);
            return true;
          }
          nRunningAmount -= probability;
        }
      }
    }
    return true;
}


CTxDestination ExtractDestinationFromSignature(CScript scriptSig, uint256 merkleRoot, uint32_t nTime, unsigned int nHeight){
    CTxDestination parsedAddress;
    CPubKey pubkey;
    OutputType output_type = g_address_type;
    std::string pScript = ScriptToString(scriptSig);
    std::string pStringSignature = ParseSignature(pScript);
    std::vector<unsigned char> vchSig(pStringSignature.begin(), pStringSignature.end());
    CHashWriter ss(SER_GETHASH, 0);
    ss << merkleRoot;
    ss << nTime;
    ss << nHeight;
    if (!pubkey.RecoverCompact(ss.GetHash(), vchSig))
      throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Couldn't recover message");
    if (!pubkey.IsValid())
      throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "invalid pubkey");
    parsedAddress = GetDestinationForKey(pubkey, output_type);
    if (!IsValidDestination(parsedAddress))
      throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "invalid destination");
    return parsedAddress;
}

CTxDestination ParseProofOfForkSignature(CScript script)
{
    std::string pStringScript = ScriptToString(script);
    std::string pProof = ParseProofOfFork(pStringScript);
    std::string pProofParsedMerkle = ParseMerkleRoot(pProof);
    std::string pProofParsedTime = ParseTime(pProof);
    std::string pProofParsedSignature = ParseSignature(pProof);
    std::string pProofParsedHeight = ParseHeight(pProof);
    uint256 nMerkleRoot = uint256S(pProofParsedMerkle);
    uint32_t nParsedTime = stoi(pProofParsedTime);
    unsigned int nHeight = stoi(pProofParsedHeight);
    CTxDestination proofdestination = ExtractDestinationFromSignature(script, nMerkleRoot, nParsedTime, nHeight);
    return proofdestination;
}

CTxDestination ParseCoinbaseSignature(CBlock *pblock){
    unsigned int nHeight = GetPrevBlockHeight(pblock) + 1;
    return ExtractDestinationFromSignature(pblock->vtx[0]->vin[0].scriptSig, BlockWitnessMerkleRoot(*pblock), pblock->nTime, nHeight);
}

bool IsGenesisBlock(CBlock *pblock)
{
    uint256 nullHash;
    nullHash.SetNull();
    return (pblock->hashPrevBlock == nullHash || chainActive.Height() < 1);
}
unsigned int GetHeight(CBlock *pblock)
{
    std::string pParsedHeight = ParseHeight(ScriptToString(pblock->vtx[0]->vin[0].scriptSig));
    unsigned int nHeight = boost::lexical_cast<unsigned int>(pParsedHeight);
    return nHeight;
}
bool IsStakelessHeight(CBlock *pblock)
{
    return GetHeight(pblock) < 1000;
}

bool IsLottoWinner(CBlock *pblock)
{
    CTxDestination address;
    CCoinsLotto lotto;
    ExtractDestination(pblock->vtx[0]->vout[0].scriptPubKey, address);
    uint64_t randomInt = ExtractRandomInt(pblock->vtx[0]->vin[0].scriptSig);
    GetLotto(pblock->nTime, lotto, randomInt);
    LogPrintf("Valid address: %s\n", EncodeDestination(lotto.nLuckyAddress));
    bool fLottoMatch = false;
    if(!lotto.nWallets.empty()){
      fLottoMatch = EncodeDestination(address) == EncodeDestination(lotto.nLuckyAddress);
    }else{
      fLottoMatch = true;
    }
    return fLottoMatch;
}

bool IsNewer(CBlock *pblock)
{
    CBlockIndex* pprevblockindex = mapBlockIndex[pblock->GetBlockHeader().hashPrevBlock];
    time_t blocktime = pblock->nTime;
    time_t prevblocktime = pprevblockindex->nTime;
    unsigned int prevblockheight = pprevblockindex->nHeight;
    return (GetHeight(pblock) == (prevblockheight + 1) && ((blocktime - prevblocktime) >= 15) || IsStakelessHeight(pblock));
}

unsigned int GetPrevBlockHeight(CBlock *pblock)
{
    CBlockIndex* pprevblockindex = mapBlockIndex[pblock->GetBlockHeader().hashPrevBlock];
    return pprevblockindex->nHeight;
}

bool IsHeightConsistent(CBlock *pblock)
{
  return (GetPrevBlockHeight(pblock) + 1)  == GetHeight(pblock);
}

bool CheckProofOfStake(const CBlock *pcblock, const Consensus::Params& params){
    CTxDestination address;
    CBlock* pblock = const_cast<CBlock*>(pcblock);
    CTxDestination parsedAddress;
    if (ExtractDestination(pblock->vtx[0]->vout[0].scriptPubKey, address)){
        CTxDestination parsedAddress;
        bool fmatch = false;
        if (IsGenesisBlock(pblock)){
            return true;
        } else {
            parsedAddress = ParseCoinbaseSignature(pblock);
            fmatch = EncodeDestination(parsedAddress) == EncodeDestination(address);
        }
        if (!fmatch) return false;
        if (!IsHeightConsistent(pblock)) return false;
        if (IsStakelessHeight(pblock) && fmatch && IsNewer(pblock)) return true;
        LogPrintf("Block Timestamp: %d\n", pblock->nTime);
        LogPrintf("Requesting address: %s\n", EncodeDestination(address));
        LogPrintf("Block height: %s\n", GetHeight(pblock));
        return IsLottoWinner(pblock) && IsNewer(pblock) && fmatch;// && pubkey.GetID() == *keyID;
    }
    return false;
}
