#include "Day08-SevenSegmentSearch.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <array>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const size_t NUM_DIGITS_IN_OUTPUT = 4;
const std::array<int, 4> EASY_DIGIT_OUTPUT_LENGTHS = {2, 3, 4, 7};

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day08
{

struct SignalDecodingEntry
{
    std::vector<std::string> signalPatterns;
    std::array<std::string, NUM_DIGITS_IN_OUTPUT> outputValues;
};

class SignalDecoder
{
public:
    SignalDecoder(std::vector<SignalDecodingEntry> signalDecodingEntries)
        : m_signalDecodingEntries{std::move(signalDecodingEntries)}
    {

    }

    unsigned getNumEasyDigits() const
    {
        return std::accumulate(m_signalDecodingEntries.cbegin(), m_signalDecodingEntries.cend(), 0u, [](const auto& acc, const auto& signalDecodingEntry)
                               {
                                   const auto& outputValues = signalDecodingEntry.outputValues;
                                   return acc + std::count_if(outputValues.cbegin(), outputValues.cend(), [](const auto& outputValue)
                                                              {
                                                                  return std::find(EASY_DIGIT_OUTPUT_LENGTHS.cbegin(), EASY_DIGIT_OUTPUT_LENGTHS.cend(), outputValue.size()) != EASY_DIGIT_OUTPUT_LENGTHS.cend();
                                                              });
                               });
    }

private:
    std::vector<SignalDecodingEntry> m_signalDecodingEntries;
};

SignalDecodingEntry parseSignalDecodingEntryLine(const std::string& signalDecodingEntryLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, signalDecodingEntryLine, boost::is_any_of("|"));

    std::vector<std::string> signalPatterns;
    boost::trim(tokens.at(0));
    boost::split(signalPatterns, tokens.at(0), boost::is_any_of(" "));

    std::vector<std::string> outputValues;
    boost::trim(tokens.at(1));
    boost::split(outputValues, tokens.at(1), boost::is_any_of(" "));

    std::array<std::string, NUM_DIGITS_IN_OUTPUT> outputValuesArray;
    std::copy_n(outputValues.begin(), NUM_DIGITS_IN_OUTPUT, outputValuesArray.begin());

    return SignalDecodingEntry{std::move(signalPatterns), std::move(outputValuesArray)};
}

std::vector<SignalDecodingEntry> parseSignalDecodingEntryLines(const std::vector<std::string>& signalDecodingEntryLines)
{
    std::vector<SignalDecodingEntry> signalDecodingEntries;

    for (const auto& line : signalDecodingEntryLines)
    {
        SignalDecodingEntry signalDecodingEntry = parseSignalDecodingEntryLine(line);
        signalDecodingEntries.push_back(std::move(signalDecodingEntry));
    }

    return signalDecodingEntries;
}

unsigned numEasyDigitsInOutput(const std::vector<std::string>& signalDecodingEntryLines)
{
    std::vector<SignalDecodingEntry> signalDecodingEntries = parseSignalDecodingEntryLines(signalDecodingEntryLines);
    SignalDecoder signalDecoder{std::move(signalDecodingEntries)};
    return signalDecoder.getNumEasyDigits();
}

}
}
}