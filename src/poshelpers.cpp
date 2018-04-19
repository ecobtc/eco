#include <utilstrencodings.h>
#include <script/script.h>
#include <script/standard.h>
#include <base58.h>
#include <util.h>
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
