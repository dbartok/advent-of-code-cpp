#include "Day06-WaitForIt.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day06
{

class Race
{
public:
    Race(int raceDuration, int recordDistance)
        : m_raceDuration{raceDuration}
        , m_recordDistance{recordDistance}
    {

    }

    int getNumWaysToBeatRecord() const
    {
        int numWaysToBeatRecord = 0;

        for (int buttonPressDuration = 0; buttonPressDuration <= m_raceDuration; ++buttonPressDuration)
        {
            const int distance = buttonPressDuration * (m_raceDuration - buttonPressDuration);
            if (distance > m_recordDistance)
            {
                ++numWaysToBeatRecord;
            }
        }

        return numWaysToBeatRecord;
    }

private:
    int m_raceDuration;
    int m_recordDistance;
};

std::vector<int> parseRaceDescriptionLine(const std::string& raceDescriptionLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, raceDescriptionLine, boost::is_any_of(" :"), boost::token_compress_on);

    std::vector<int> numbers;

    std::transform(tokens.cbegin() + 1, tokens.cend(), std::back_inserter(numbers), [](const auto& token)
                   {
                       return std::stoi(token);
                   });

    return numbers;
}

std::vector<Race> parseRaceDescriptionLines(const std::vector<std::string>& raceDescriptionLines)
{
    const std::vector<int> times = parseRaceDescriptionLine(raceDescriptionLines.at(0));
    const std::vector<int> recordDistances = parseRaceDescriptionLine(raceDescriptionLines.at(1));

    std::vector<Race> races;

    for (size_t i = 0; i < times.size(); ++i)
    {
        races.emplace_back(times.at(i), recordDistances.at(i));
    }

    return races;
}

int numWaysRecordCanBeBeatenMultipliedTogether(const std::vector<std::string>& raceDescriptionLines)
{
    std::vector<Race> races = parseRaceDescriptionLines(raceDescriptionLines);

    return std::accumulate(races.cbegin(), races.cend(), 1, [](int acc, const auto& race)
                           {
                               return acc * race.getNumWaysToBeatRecord();
                           });
}

}
}
}