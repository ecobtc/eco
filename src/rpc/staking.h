#ifndef STAKING_H
#define STAKING_H

#include <script/script.h>
#include <wallet/wallet.h>
#include <univalue.h>
class CRPCTable;
class JSONRPCRequest;

void RegisterStakingRPCCommands(CRPCTable &t);
#endif
