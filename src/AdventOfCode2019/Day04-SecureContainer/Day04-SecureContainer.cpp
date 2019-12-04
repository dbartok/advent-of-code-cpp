#include "Day04-SecureContainer.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/optional.hpp>

#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::vector<int> convertIntToDigitVector(int number)
{
    std::vector<int> digits;

    while (number > 0)
    {
        digits.push_back(number % 10);
        number /= 10;
    }

    std::reverse(digits.begin(), digits.end());

    return digits;
}

bool areDigitsIncreasing(const std::vector<int>& digits)
{
    boost::optional<int> previousDigit{};

    for (const auto digit : digits)
    {
        if (digit < previousDigit)
        {
            return false;
        }

        previousDigit = digit;
    }

    return true;
}

bool hasTwoAdjacentEqualDigits(const std::vector<int>& digits)
{
    boost::optional<int> previousDigit{};

    for (const auto digit : digits)
    {
        if (digit == previousDigit)
        {
            return true;
        }

        previousDigit = digit;
    }

    return false;
}

bool isValidPassword(int candidate)
{
    std::vector<int> digits = convertIntToDigitVector(candidate);

    return areDigitsIncreasing(digits) && hasTwoAdjacentEqualDigits(digits);
}

unsigned numDifferentValidPasswords(int lowerBound, int upperBound)
{
    unsigned numValidPasswords = 0;

    for (int candidate = lowerBound; candidate <= upperBound; ++candidate)
    {
        if (isValidPassword(candidate))
        {
            ++numValidPasswords;
        }
    }

    return numValidPasswords;
}

}
