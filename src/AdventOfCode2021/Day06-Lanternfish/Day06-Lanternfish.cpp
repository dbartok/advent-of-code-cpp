#include "Day06-Lanternfish.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <array>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned NUM_DAYS = 80;

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

    unsigned getNumTotalFish() const
    {
        return std::accumulate(m_timerToNumFish.cbegin(), m_timerToNumFish.cend(), 0, [](const auto& acc, const auto& elem)
                               {
                                   return acc + elem;
                               });
    }

private:
    std::vector<int> m_timerToNumFish;

    void simulateDay()
    {
        const int numFishWithZeroTimer = m_timerToNumFish.at(0);
        m_timerToNumFish.erase(m_timerToNumFish.begin());
        m_timerToNumFish.push_back(numFishWithZeroTimer);
        m_timerToNumFish.at(6) += numFishWithZeroTimer;
    }
};

unsigned numLanternfishAfterMultipleDays(const std::vector<int>& initialState)
{
    LanterfishSimulator lanternfishSimulator{initialState};
    lanternfishSimulator.simulateDays(NUM_DAYS);
    return lanternfishSimulator.getNumTotalFish();
}

}
}
}