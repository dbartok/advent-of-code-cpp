#include "Day15-LensLibrary.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <vector>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day15
{

class InitializationSequenceAnalyzer
{
public:
    InitializationSequenceAnalyzer(std::vector<std::string> initializationSequence)
        : m_initializationSequence{std::move(initializationSequence)}
    {

    }

    int getSumOfAllHashes() const
    {
        return std::accumulate(m_initializationSequence.cbegin(), m_initializationSequence.cend(), 0, [](int acc, const auto& initializationString)
                               {
                                   return acc + getHashValue(initializationString);
                               });
    }

private:
    std::vector<std::string> m_initializationSequence;

    static int getHashValue(const std::string& str)
    {
        int hashValue = 0;

        for (const char c : str)
        {
            hashValue += c;
            hashValue *= 17;
            hashValue %= 256;
        }

        return hashValue;
    }
};

std::vector<std::string> parseInitializationSequenceString(const std::string& initializationSequenceString)
{
    std::vector<std::string> initializationSequence;
    boost::split(initializationSequence, initializationSequenceString, boost::is_any_of(","));
    return initializationSequence;
}

int sumOfHashResults(const std::string& initializationSequenceString)
{
    std::vector<std::string> initializationSequence = parseInitializationSequenceString(initializationSequenceString);

    InitializationSequenceAnalyzer initializationSequenceAnalyzer{std::move(initializationSequence)};

    return initializationSequenceAnalyzer.getSumOfAllHashes();
}

}
}
}