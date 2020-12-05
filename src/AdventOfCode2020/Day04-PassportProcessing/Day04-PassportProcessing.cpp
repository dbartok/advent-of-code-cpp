#include "Day04-PassportProcessing.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_map>
#include <array>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using PassportData = std::unordered_map<std::string, std::string>;

const std::array<std::string, 7> REQUIRED_FIELDS =
{
    "byr",
    "iyr",
    "eyr",
    "hgt",
    "hcl",
    "ecl",
    "pid",
};

bool isValidPassportData(const PassportData& passportData)
{
    for (const auto& requiredField : REQUIRED_FIELDS)
    {
        if (passportData.find(requiredField) == passportData.cend())
        {
            return false;
        }
    }

    return true;
}

std::vector<std::string> transformBatchFileLinesToSingleLines(const std::vector<std::string>& batchFileLines)
{
    std::vector<std::string> singleLines;
    std::vector<std::string> currentLineParts;
    for (const auto& batchFileLine : batchFileLines)
    {
        if (batchFileLine == "")
        {
            singleLines.push_back(boost::algorithm::join(currentLineParts, " "));
            currentLineParts = {};
            continue;
        }

        currentLineParts.push_back(batchFileLine);
    }

    singleLines.push_back(boost::algorithm::join(currentLineParts, " "));

    return singleLines;
}

PassportData parsePassportData(const std::string& singleLine)
{
    PassportData passportData;

    std::vector<std::string> keyValuePairs;
    boost::split(keyValuePairs, singleLine, boost::is_any_of(" "));

    for (const auto& keyValuePair : keyValuePairs)
    {
        std::vector<std::string> keyValuePairTokens;
        boost::split(keyValuePairTokens, keyValuePair, boost::is_any_of(":"));

        passportData.emplace(std::move(keyValuePairTokens.at(0)), std::move(keyValuePairTokens.at(1)));
    }

    return passportData;
}

std::vector<PassportData> parseAllPassportData(const std::vector<std::string>& batchFileLines)
{
    std::vector<PassportData> allPassportData;
    std::vector<std::string> singleLines = transformBatchFileLinesToSingleLines(batchFileLines);

    for (const auto& singleLine : singleLines)
    {
        PassportData passportData = parsePassportData(singleLine);
        allPassportData.push_back(std::move(passportData));
    }

    return allPassportData;
}

int numValidPassports(const std::vector<std::string>& batchFileLines)
{
    std::vector<PassportData> allPassportData = parseAllPassportData(batchFileLines);
    return std::count_if(allPassportData.cbegin(), allPassportData.cend(), [](const auto& passportData)
                         {
                             return isValidPassportData(passportData);
                         });
}

}
