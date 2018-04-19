#include <base58.h>
#include <amount.h>
#include <chain.h>
#include <chainparams.h>
#include <consensus/consensus.h>
#include <consensus/params.h>
#include <consensus/validation.h>
#include <core_io.h>
#include <init.h>
#include <validation.h>
#include <miner.h>
#include <net.h>
#include <policy/fees.h>
#include <pow.h>
#include <pos.h>
#include <rpc/blockchain.h>
#include <rpc/staking.h>
#include <rpc/server.h>
#include <wallet/rpcwallet.h>
#include <txmempool.h>
#include <util.h>
#include <utilstrencodings.h>
#include <validationinterface.h>
#include <warnings.h>
#include <script/standard.h>
#include <memory>
#include <stdint.h>
#include <wallet/rpcwallet.h>
#include <timedata.h>
UniValue getluckyaddress(const JSONRPCRequest& request)
{
    UniValue ret(UniValue::VOBJ);
    time_t rando;
    time(&rando);
    CCoinsLotto lotto;
    CWallet * const pwallet = GetWalletForJSONRPCRequest(request);
    if (!EnsureWalletIsAvailable(pwallet, request.fHelp)) {
        return NullUniValue;
    }
    if (GetLotto(rando, lotto)) {
        ret.pushKV("lucky_coin", lotto.nLuckyCoin);
        ret.pushKV("lucky_address", EncodeDestination(lotto.nLuckyAddress));
        ret.pushKV("timestamp", lotto.nTimeStamp);
        CKeyID keyID = GetKeyForDestination(*pwallet, lotto.nLuckyAddress);
        CKey key;
        if (!pwallet->GetKey(keyID, key)) {
          ret.pushKV("owned_address", "false");
        }else{
          ret.pushKV("owned_address", "true");
        }
        if((GetTime() - chainActive.Tip()->nTime)>=15){
          ret.pushKV("up_for_grabs", "true");
          ret.pushKV("time_to_next", 0);
        }else{
          ret.pushKV("up_for_grabs", "false ");
          ret.pushKV("time_to_next", (15 - (GetTime() - chainActive.Tip()->nTime)));
        }
    }
    return ret;
}

static const CRPCCommand commands[] =
{ //  category              name                      actor (function)         argNames
  //  --------------------- ------------------------  -----------------------  ----------
    { "staking",             "getluckyaddress",       &getluckyaddress,       {} },
};

void RegisterStakingRPCCommands(CRPCTable &t)
{
    for (unsigned int vcidx = 0; vcidx < ARRAYLEN(commands); vcidx++)
        t.appendCommand(commands[vcidx].name, &commands[vcidx]);
}
