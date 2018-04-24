#ifndef POS_H
#define POS_H

#include <time.h>
#include <base58.h>
class CBlock;
class CBlockIndex;
class UniValue;

struct CCoinsLotto
{
    std::map<std::string, CAmount> nWallets;
    CAmount nTotalAmount;
    double nLuckyCoin = 0;
    CAmount nLuckyAmount = 0;
    bool nLuckyFound = false;
    CScript nLuckyScript;
    CTxDestination nLuckyAddress;
    time_t nTimeStamp;
    CCoinsLotto() : nTotalAmount(0), nLuckyCoin(0) {}

};

bool GetLotto(time_t timestamp, CCoinsLotto &lotto, uint64_t randomInt = 0);
bool CheckProofOfStake(const CBlock *pcblock, const Consensus::Params& params);
unsigned int GetPrevBlockHeight(CBlock *pblock);
CTxDestination ExtractDestinationFromSignature(CScript scriptSig, uint256 merkleRoot, uint32_t nTime, unsigned int nHeight);
CTxDestination ParseCoinbaseSignature(CBlock *pblock);
CTxDestination ParseProofOfForkSignature(CScript script);
unsigned int GetHeight(CBlock *pblock);
#endif
