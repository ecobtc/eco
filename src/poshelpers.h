#ifndef POSHELPERS_H
#define POSHELPERS_H

#include <time.h>
#include <base58.h>

std::string ParseProofOfFork(std::string pScript);
std::string ParseSignature(std::string pScript);
std::string ParseTime(std::string pScript);
std::string ParseHeight(std::string pScript);
std::string ParseMerkleRoot(std::string pScript);
std::string ParseDelegate(std::string pScript);
std::vector<unsigned char> ToUnsignedCharVector(std::string pString);
bool IsProofOfForkScript(const CScript scriptSig);
CTxDestination GetStakeDelegate(const CScript& scriptPubKey, bool *fValid);
uint64_t GetRandomInt();
uint64_t ExtractRandomInt(CScript scriptSig);
#endif
