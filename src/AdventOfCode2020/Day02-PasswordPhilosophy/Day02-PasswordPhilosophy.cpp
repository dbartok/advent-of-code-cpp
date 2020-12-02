#include "Day02-PasswordPhilosophy.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct PasswordWithPolicy
{
    PasswordWithPolicy(std::string password, char givenLetter, int minNumOccurrences, int maxNumOccurrences)
        : password{std::move(password)}
        , givenLetter{givenLetter}
        , minNumOccurrences{minNumOccurrences}
        , maxNumOccurrences{maxNumOccurrences}
    {

    }

    std::string password;
    char givenLetter;
    int minNumOccurrences;
    int maxNumOccurrences;
};

PasswordWithPolicy parsePasswordWithPolicy(const std::string& passwordWithPolicyLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, passwordWithPolicyLine, boost::is_any_of(" :-"), boost::token_compress_on);

    std::string password = tokens.at(3);

    std::string givenLetterToken = tokens.at(2);
    if (givenLetterToken.size() != 1)
    {
        throw std::runtime_error("Size of given letter larger than 1: " + givenLetterToken);
    }
    const char givenLetter = givenLetterToken.front();

    const int minNumOccurrences = std::stoi(tokens.at(0));
    const int maxNumOccurrences = std::stoi(tokens.at(1));

    return PasswordWithPolicy{std::move(password), givenLetter, minNumOccurrences, maxNumOccurrences};
}

std::vector<PasswordWithPolicy> parsePasswordWithPolicyList(const std::vector<std::string>& passwordWithPolicyLines)
{
    std::vector<PasswordWithPolicy> passwordWithPolicyList;

    for (const auto& line : passwordWithPolicyLines)
    {
        PasswordWithPolicy passwordWithPolicy = parsePasswordWithPolicy(line);
        passwordWithPolicyList.push_back(std::move(passwordWithPolicy));
    }

    return passwordWithPolicyList;
}

int numValidPasswords(const std::vector<std::string>& passwordWithPolicyLines)
{
    std::vector<PasswordWithPolicy> passwordWithPolicyList = parsePasswordWithPolicyList(passwordWithPolicyLines);

    return std::count_if(passwordWithPolicyList.cbegin(), passwordWithPolicyList.cend(), [](const auto& passwordWithPolicy)
                         {
                             const int numOccurrencesOfGivenLetter = std::count(passwordWithPolicy.password.cbegin(),
                                                                          passwordWithPolicy.password.cend(),
                                                                          passwordWithPolicy.givenLetter);
                             return (numOccurrencesOfGivenLetter >= passwordWithPolicy.minNumOccurrences &&
                                     numOccurrencesOfGivenLetter <= passwordWithPolicy.maxNumOccurrences);
                         });
}

}
