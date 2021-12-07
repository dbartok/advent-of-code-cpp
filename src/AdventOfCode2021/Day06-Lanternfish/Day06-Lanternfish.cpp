#include "Day06-Lanternfish.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <array>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned NUM_DAYS_FIRST_PART = 80;
const unsigned NUM_DAYS_SECOND_PART = 256;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day06
{

class LanterfishSimulator
{
public:
    LanterfishSimulator(const std::vector<int>& initialState)
        : m_timerToNumFish(9)
    {
        for (const auto timer : initialState)
        {
            ++m_timerToNumFish.at(timer);
        }
    }

    void simulateDays(unsigned numRounds)
    {
        for (size_t i = 0; i < numRounds; ++i)
        {
            simulateDay();
        }
    }

    uint64_t getNumTotalFish() const
    {
        return std::accumulate(m_timerToNumFish.cbegin(), m_timerToNumFish.cend(), 0ull, [](const auto& acc, const auto& elem)
                               {
                                   return acc + elem;
                               });
    }

private:
    std::vector<uint64_t> m_timerToNumFish;

    void simulateDay()
    {
        const uint64_t numFishWithZeroTimer = m_timerToNumFish.at(0);
        m_timerToNumFish.erase(m_timerToNumFish.begin());
        m_timerToNumFish.push_back(numFishWithZeroTimer);
        m_timerToNumFish.at(6) += numFishWithZeroTimer;
    }
};

uint64_t numLanternfishAfterSmallNumberOfDays(const std::vector<int>& initialState)
{
    LanterfishSimulator lanternfishSimulator{initialState};
    lanternfishSimulator.simulateDays(NUM_DAYS_FIRST_PART);
    return lanternfishSimulator.getNumTotalFish();
}

uint64_t numLanternfishAfterLargeNumberOfDays(const std::vector<int>& initialState)
{
    LanterfishSimulator lanternfishSimulator{initialState};
    lanternfishSimulator.simulateDays(NUM_DAYS_SECOND_PART);
    return lanternfishSimulator.getNumTotalFish();
}

}
}
}