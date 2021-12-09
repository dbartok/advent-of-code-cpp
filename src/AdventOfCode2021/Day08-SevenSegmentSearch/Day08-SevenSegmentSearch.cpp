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
const size_t NUM_POSSIBLE_DIGITS = 10;
const std::array<int, 4> EASY_DIGIT_OUTPUT_LENGTHS = {2, 3, 4, 7};
const std::array<std::string, NUM_POSSIBLE_DIGITS> EXPECTED_DECODED_SIGNAL_PATTERNS =
{
    "abcefg",
    "cf",
    "acdeg",
    "acdfg",
    "bcdf",
    "abdfg",
    "abdefg",
    "acf",
    "abcdefg",
    "abcdfg",
};

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day08
{

class SignalDecoder
{
public:
    SignalDecoder(std::array<std::string, NUM_POSSIBLE_DIGITS> signalPatterns, std::array<std::string, NUM_DIGITS_IN_OUTPUT> outputValues)
        : m_signalPatterns{std::move(signalPatterns)}
        , m_outputDigits{std::move(outputValues)}
    {

    }

    unsigned getNumEasyDigits() const
    {
        return std::count_if(m_outputDigits.cbegin(), m_outputDigits.cend(), [](const auto& outputValue)
                             {
                                 return std::find(EASY_DIGIT_OUTPUT_LENGTHS.cbegin(), EASY_DIGIT_OUTPUT_LENGTHS.cend(), outputValue.size()) != EASY_DIGIT_OUTPUT_LENGTHS.cend();
                             });
    }

    void determineDecodingMapping()
    {
        const std::vector<std::string> allDecodingMappingCandidates = getAllDecodingMappingCandidates();

        for (const auto& decodingMappingCandidate : allDecodingMappingCandidates)
        {
            if (isValidDecodingMappingCandidate(decodingMappingCandidate))
            {
                m_decodingMapping = decodingMappingCandidate;
                return;
            }
        }

        throw std::runtime_error("No valid decoding found");
    }

    int getDecodedOutputValue() const
    {
        int decodedOutputValue = 0;

        for (const auto& outputDigit : m_outputDigits)
        {
            decodedOutputValue *= 10;
            const int currentOutputDigitValue = getOutputDigitValue(outputDigit);
            decodedOutputValue += currentOutputDigitValue;
        }
        
        return decodedOutputValue;
    }

private:
    std::array<std::string, NUM_POSSIBLE_DIGITS> m_signalPatterns;
    std::array<std::string, NUM_DIGITS_IN_OUTPUT> m_outputDigits;

    std::string m_decodingMapping;

    bool isValidDecodingMappingCandidate(const std::string& decodingMappingCandidate)
    {
        std::array<std::string, NUM_POSSIBLE_DIGITS> decodedSignalPatterns = getDecodedSignalPatterns(decodingMappingCandidate);
        
        std::sort(decodedSignalPatterns.begin(), decodedSignalPatterns.end());

        auto expectedDecodedSignalPatterns{EXPECTED_DECODED_SIGNAL_PATTERNS};
        std::sort(expectedDecodedSignalPatterns.begin(), expectedDecodedSignalPatterns.end());

        return decodedSignalPatterns == expectedDecodedSignalPatterns;
    }

    std::array<std::string, NUM_POSSIBLE_DIGITS> getDecodedSignalPatterns(const std::string& decodingMappingCandidate)
    {
        std::array<std::string, NUM_POSSIBLE_DIGITS> decodedSignalPatterns;

        for(size_t i = 0; i < m_signalPatterns.size(); ++i)
        {
            std::string decodedSignalPattern = getDecodedSignalPattern(decodingMappingCandidate, m_signalPatterns.at(i));
            decodedSignalPatterns.at(i) = std::move(decodedSignalPattern);
        }

        return decodedSignalPatterns;
    }

    std::string getDecodedSignalPattern(const std::string& decodingMapping, const std::string& signalPattern) const
    {
        std::string decodedSignalPattern;

        for (const char c : signalPattern)
        {
            const char decodedCharacter = decodingMapping.at(c - 'a');
            decodedSignalPattern.push_back(decodedCharacter);
        }

        std::sort(decodedSignalPattern.begin(), decodedSignalPattern.end());

        return decodedSignalPattern;
    }

    int getOutputDigitValue(const std::string& outputDigit) const
    {
        const std::string decodedSignalPattern = getDecodedSignalPattern(m_decodingMapping, outputDigit);
        const auto currentDecodedDigitOffsetIter = std::find(EXPECTED_DECODED_SIGNAL_PATTERNS.cbegin(), EXPECTED_DECODED_SIGNAL_PATTERNS.cend(), decodedSignalPattern);
        
        if (currentDecodedDigitOffsetIter == EXPECTED_DECODED_SIGNAL_PATTERNS.cend())
        {
            throw std::runtime_error("Cannot find output digit in the expected decoded signal patterns");
        }

        return currentDecodedDigitOffsetIter - EXPECTED_DECODED_SIGNAL_PATTERNS.cbegin();
    }

    static std::vector<std::string> getAllDecodingMappingCandidates()
    {
        std::string decodingMappingCandidate = "abcdefg";
        std::vector<std::string> allDecodingMappingCandidates = {decodingMappingCandidate};
        
        while (std::next_permutation(decodingMappingCandidate.begin(), decodingMappingCandidate.end()))
        {
            allDecodingMappingCandidates.push_back(decodingMappingCandidate);
        }

        return allDecodingMappingCandidates;
    }
};

SignalDecoder parseSignalDecodingEntryLine(const std::string& signalDecodingEntryLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, signalDecodingEntryLine, boost::is_any_of("|"));

    std::vector<std::string> signalPatterns;
    boost::trim(tokens.at(0));
    boost::split(signalPatterns, tokens.at(0), boost::is_any_of(" "));
    std::array<std::string, NUM_POSSIBLE_DIGITS> signalPatternsArray;
    std::copy_n(signalPatterns.begin(), NUM_POSSIBLE_DIGITS, signalPatternsArray.begin());

    std::vector<std::string> outputDigits;
    boost::trim(tokens.at(1));
    boost::split(outputDigits, tokens.at(1), boost::is_any_of(" "));
    std::array<std::string, NUM_DIGITS_IN_OUTPUT> outputDigitsArray;
    std::copy_n(outputDigits.begin(), NUM_DIGITS_IN_OUTPUT, outputDigitsArray.begin());

    return SignalDecoder{std::move(signalPatternsArray), std::move(outputDigitsArray)};
}

std::vector<SignalDecoder> parseSignalDecodingEntryLines(const std::vector<std::string>& signalDecodingEntryLines)
{
    std::vector<SignalDecoder> signalDecoders;

    for (const auto& line : signalDecodingEntryLines)
    {
        SignalDecoder signalDecoder = parseSignalDecodingEntryLine(line);
        signalDecoders.push_back(std::move(signalDecoder));
    }

    return signalDecoders;
}

unsigned numEasyDigitsInOutput(const std::vector<std::string>& signalDecodingEntryLines)
{
    const std::vector<SignalDecoder> signalDecoders = parseSignalDecodingEntryLines(signalDecodingEntryLines);
    return std::accumulate(signalDecoders.cbegin(), signalDecoders.cend(), 0u, [](const auto& acc, const auto& signalDecoder)
                           {
                               return acc + signalDecoder.getNumEasyDigits();
                           });
}

int sumOfDecodedOutputValues(const std::vector<std::string>& signalDecodingEntryLines)
{
    std::vector<SignalDecoder> signalDecoders = parseSignalDecodingEntryLines(signalDecodingEntryLines);
    
    for (auto& signalDecoder : signalDecoders)
    {
        signalDecoder.determineDecodingMapping();
    }
    
    return std::accumulate(signalDecoders.cbegin(), signalDecoders.cend(), 0u, [](const auto& acc, const auto& signalDecoder)
                           {
                               return acc + signalDecoder.getDecodedOutputValue();
                           });
}

}
}
}