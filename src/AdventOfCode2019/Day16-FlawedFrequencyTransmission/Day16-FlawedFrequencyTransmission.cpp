#include "Day16-FlawedFrequencyTransmission.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <array>
#include <numeric>
#include <vector>
#include <sstream>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const size_t NUM_ITERATIONS = 100;
const size_t PATTERN_OFFSET = 1;
const size_t BASE_PATTERN_SIZE = 4;
const std::array<int, BASE_PATTERN_SIZE> BASE_PATTERN{0, 1, 0, -1};
}

namespace AdventOfCode
{

std::vector<int> convertToVectorOfDigits(const std::string& s)
{
    std::vector<int> intVector;

    for (auto c : s)
    {
        intVector.push_back(c - '0');
    }

    return intVector;
}

std::string convertToString(std::vector<int> vec)
{
    std::ostringstream oss;

    for (auto num : vec)
    {
        oss << num;
    }

    return oss.str();
}

std::vector<int> getRepeatingPattern(size_t patternRepeatLength, size_t patternLength)
{
    std::vector<int> repeatingPattern;

    for (size_t i = 0; i < patternLength; ++i)
    {
        int patternElement = BASE_PATTERN.at((PATTERN_OFFSET + i) / patternRepeatLength % BASE_PATTERN_SIZE);
        repeatingPattern.push_back(patternElement);
    }

    return repeatingPattern;
}

int multiplyElementwiseAndSummarize(const std::vector<int>& v1, const std::vector<int>& v2)
{
    assert(v1.size() == v2.size());

    std::vector<int> result;

    std::transform(v1.cbegin(), v1.cend(), v2.begin(), std::back_inserter(result), std::multiplies<int>());

    return std::accumulate(result.begin(), result.end(), 0);
}

std::vector<int> applyPhase(const std::vector<int>& digits)
{
    std::vector<int> result;

    for (size_t i = 0; i < digits.size(); ++i)
    {
        std::vector<int> repeatingPattern = getRepeatingPattern(i + 1, digits.size());

        int reductionResult = multiplyElementwiseAndSummarize(digits, repeatingPattern);

        result.push_back(std::abs(reductionResult) % 10);
    }

    return result;
}

std::string firstEightDigitsOfFinalOutput(const std::string& signalString)
{
    std::vector<int> digits = convertToVectorOfDigits(signalString);

    for (size_t i = 0; i < NUM_ITERATIONS; ++i)
    {
        digits = applyPhase(digits);
    }

    std::string digitsAsString = convertToString(digits);

    return digitsAsString.substr(0, 8);
}

}
