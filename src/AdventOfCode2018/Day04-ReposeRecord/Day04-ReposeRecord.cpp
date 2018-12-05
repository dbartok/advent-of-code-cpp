#include "Day04-ReposeRecord.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <array>
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using GuardIDType = unsigned;
using MinuteAndAmountPair = std::pair<unsigned, unsigned>;

struct Day
{
    Day()
        : sleep{}
    {

    }

    std::array<bool, 60> sleep;
};

struct Guard
{
    unsigned getHoursSlept() const
    {
        return std::accumulate(days.cbegin(), days.cend(), 0u, [](unsigned acc, const Day& day)
                               {
                                   return acc + std::count(day.sleep.cbegin(), day.sleep.cend(), true);
                               });
    }

    MinuteAndAmountPair getMostSleptMinute() const
    {
        std::array<int, 60> allZeroes;
        std::fill(allZeroes.begin(), allZeroes.end(), 0);
        auto sleepValues = std::accumulate(days.cbegin(), days.cend(), allZeroes, [](std::array<int, 60> acc, const Day& day)
                                           {
                                               for (size_t i = 0; i < day.sleep.size(); ++i)
                                               {
                                                   if (day.sleep[i])
                                                   {
                                                       ++acc[i];
                                                   }
                                               }

                                               return acc;
                                           });

        auto maxIter = std::max_element(sleepValues.cbegin(), sleepValues.cend());
        return std::make_pair(maxIter - sleepValues.cbegin(), *maxIter);
    }

    std::vector<Day> days;
};

struct GuardDutyKeeper
{
    GuardIDType getMostAsleepGuardID()
    {
        auto maxElemIter = std::max_element(guards.cbegin(), guards.cend(), [](const auto& lhs, const auto& rhs)
                                            {
                                                return lhs.second.getHoursSlept() < rhs.second.getHoursSlept();
                                            });

        return maxElemIter->first;
    }

    GuardIDType getMostFrequentlyMinuteAsleepGuardID()
    {
        std::unordered_map<GuardIDType, unsigned> guardsToMostSleptMinute;

        for (const auto& elem : guards)
        {
            guardsToMostSleptMinute.insert(std::make_pair(elem.first, elem.second.getMostSleptMinute().second));
        }

        auto maxElemIter = std::max_element(guardsToMostSleptMinute.cbegin(), guardsToMostSleptMinute.cend(), [](const auto& lhs, const auto& rhs)
                                            {
                                                return lhs.second < rhs.second;
                                            });

        return maxElemIter->first;
    }

    std::unordered_map<GuardIDType, Guard> guards;
};

struct Event
{
    std::tm timestamp;
    std::vector<std::string> args;
};

std::vector<Event> parseEvents(const std::vector<std::string>& eventLines)
{
    std::vector<Event> events;

    for (const auto& line : eventLines)
    {
        std::vector<std::string> tokens;
        boost::algorithm::split(tokens, line, boost::is_any_of("[]"), boost::token_compress_on);

        std::string timestampString = tokens[1].substr(5);
        std::stringstream ss{timestampString};
        std::tm tm = {};
        ss >> std::get_time(&tm, "%m-%d %H:%M");
        tm.tm_year = 70;
        std::mktime(&tm);
        auto timestamp = tm;

        std::vector<std::string> args;
        boost::algorithm::split(args, tokens[2], boost::is_any_of(" #"), boost::token_compress_on);
        Event event{tm, args};

        events.push_back(std::move(event));
    }

    return events;
}

std::vector<std::vector<AdventOfCode::Event>> getEventsForEachDay(std::vector<AdventOfCode::Event> &events)
{
    std::vector<std::vector<AdventOfCode::Event>> eventsForEachDay;

    std::vector<Event> eventsForADay;
    for (auto iter = events.begin(); iter != events.end(); ++iter)
    {
        eventsForADay.push_back(std::move(*iter));

        const auto nextIter = (iter + 1);
        if (nextIter == events.end() || nextIter->args.size() == 5)
        {
            eventsForEachDay.push_back(std::move(eventsForADay));
            eventsForADay.clear();
        }
    }

    return eventsForEachDay;
}

GuardDutyKeeper parseGuardDutyKeeper(const std::vector<std::vector<Event>>& eventsForEachDay)
{
    GuardDutyKeeper guardDutyKeeper;

    for (const auto& eventsForADay : eventsForEachDay)
    {
        GuardIDType guardID = boost::lexical_cast<unsigned>(eventsForADay.front().args[2]);

        Day day;
        unsigned minute = 0;
        bool value = false;
        for (auto iter = eventsForADay.begin() + 1; iter != eventsForADay.end(); ++iter)
        {
            unsigned newMinute = iter->timestamp.tm_min;
            std::fill(day.sleep.begin() + minute, day.sleep.begin() + newMinute, value);
            value = !value;
            minute = newMinute;
        }
        std::fill(day.sleep.begin() + minute, day.sleep.end(), value);

        guardDutyKeeper.guards[guardID].days.push_back(std::move(day));
    }

    return guardDutyKeeper;
}

GuardDutyKeeper parseGuardDutyKeeperFromEventLines(const std::vector<std::string>& eventLines)
{
    std::vector<Event> events = parseEvents(eventLines);

    std::sort(events.begin(), events.end(), [](const Event& lhs, const Event& rhs)
              {
                  auto lts = lhs.timestamp;
                  auto rts = rhs.timestamp;

                  return std::difftime(std::mktime(&lts), std::mktime(&rts)) < 0;
              });

    std::vector<std::vector<Event>> eventsForEachDay = getEventsForEachDay(events);

    return parseGuardDutyKeeper(eventsForEachDay);
}

unsigned guardMostMinutesAsleepTimesMinute(const std::vector<std::string>& eventLines)
{
    GuardDutyKeeper guardDutyKeeper = parseGuardDutyKeeperFromEventLines(eventLines);

    GuardIDType mostAsleepGuardID = guardDutyKeeper.getMostAsleepGuardID();
    unsigned mostSleptMinute = guardDutyKeeper.guards[mostAsleepGuardID].getMostSleptMinute().first;

    return mostAsleepGuardID * mostSleptMinute;
}

unsigned guardMostFrequentlyMinuteAsleepTimesMinute(const std::vector<std::string>& eventLines)
{
    GuardDutyKeeper guardDutyKeeper = parseGuardDutyKeeperFromEventLines(eventLines);

    GuardIDType mostFrequentlyMinuteAsleepGuardID = guardDutyKeeper.getMostFrequentlyMinuteAsleepGuardID();
    unsigned mostSleptMinute = guardDutyKeeper.guards[mostFrequentlyMinuteAsleepGuardID].getMostSleptMinute().first;

    return mostFrequentlyMinuteAsleepGuardID * mostSleptMinute;
}

}
