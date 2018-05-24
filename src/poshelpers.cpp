#include <utilstrencodings.h>
#include <script/script.h>
#include <script/standard.h>
#include <base58.h>
#include <util.h>
#include <boost/lexical_cast.hpp>
#include <crypto/sha512.h>

#include "uccurl.h"

typedef std::vector<unsigned char> valtype;

std::string ParseSection(std::string blockName, std::string pScript)
{
  std::string pStart = "start" + blockName;
  std::string pEnd = "end" + blockName;
  std::size_t nSignatureStart = pScript.find(pStart)+pStart.length();
  std::size_t nSignatureEnd = pScript.find(pEnd);
  if(nSignatureEnd == std::string::npos) return "";
  return pScript.substr(nSignatureStart, nSignatureEnd-nSignatureStart);
}

std::string ParseProofOfFork(std::string pScript)
{
    std::string pParsed = ParseSection("proof", pScript);
    return pParsed;
}
std::string ParseHeight(std::string pScript)
{
    return DecodeBase64(ParseSection("height", pScript));
}

std::string ParseTime(std::string pScript)
{
    return DecodeBase64(ParseSection("ntime", pScript));
}

std::string ParseRandomInt(std::string pScript)
{
    return DecodeBase64(ParseSection("nrandint", pScript));
}

std::string ParseMerkleRoot(std::string pScript)
{
    return DecodeBase64(ParseSection("merkle", pScript));
}

std::string ParseSignature(std::string pScript)
{
    return DecodeBase64(ParseSection("sig", pScript));
}

std::string ParseDelegate(std::string pScript)
{
    return DecodeBase64(ParseSection("delegate", pScript));
}

std::vector<unsigned char> ToUnsignedCharVector(std::string pString)
{
    std::vector<unsigned char> result(pString.begin(), pString.end());
}

uint64_t ExtractRandomInt(CScript scriptSig)
{
    std::string pScript = ScriptToString(scriptSig);
    return boost::lexical_cast<uint64_t>(ParseRandomInt(pScript));
}


bool IsProofOfForkScript(const CScript scriptSig)
{
    std::string pScript = ScriptToString(scriptSig);
    pScript = ParseProofOfFork(pScript);
    return (pScript.size() > 0);
}

CTxDestination GetStakeDelegate(const CScript& scriptPubKey, bool *fValid){
    CTxDestination dest;
    std::string pScript = ScriptToString(scriptPubKey);
    std::string pdest = ParseDelegate(pScript);
    *fValid = false;
    if (pdest.size() > 0)
    {
        dest = DecodeDestination(pdest);
        *fValid = true;
    }
    return dest;
}

uint64_t GetRandomInt()
{
    uint64_t randomInt;
    std::string pDataRaw; // = pOutputValue->FirstChild()->Value();
    try {
        uc::curl::global libcurlInit;
        uc::curl::easy("https://cdn.star.nesdis.noaa.gov/GOES16/ABI/CONUS/16/latest.jpg") >> pDataRaw;
        CSHA512 hasher;
        unsigned char sha512hash[64];
        hasher.Write((const unsigned char*)pDataRaw.c_str(), pDataRaw.length()).Finalize(&sha512hash[0]);
        memcpy(&randomInt, &sha512hash[0], sizeof(uint64_t));
        return randomInt;
    } catch (...) {
        LogPrintf("exception : Couldn't connect to endpoint\n");
        return 0;
    }
}
