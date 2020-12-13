#include "Day13-ShuttleSearch.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <vector>
#include <cmath>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::vector<int> parseScheduleDescription(const std::string& scheduleDescriptionString)
{
    std::vector<std::string> tokens;
    boost::split(tokens, scheduleDescriptionString, boost::is_any_of(","));

    std::vector<std::string> validTokens;
    std::copy_if(tokens.cbegin(), tokens.cend(), std::back_inserter(validTokens), [](const auto& token) { return token != "x"; });

    std::vector<int> intTokens;
    std::transform(validTokens.cbegin(), validTokens.cend(), std::back_inserter(intTokens), [](const auto& token) { return std::stoi(token); });

    return intTokens;
}

int getWaitTime(int earliestDepartureTimestamp, int busID)
{
    const int earliestDepartureTimeViaThisBus = std::ceil((static_cast<double>(earliestDepartureTimestamp) / busID)) * busID;
    return earliestDepartureTimeViaThisBus - earliestDepartureTimestamp;
}

int earliestBusIDMultipliedByWaitTime(int earliestDepartureTimestamp, const std::string& scheduleDescriptionString)
{
    std::vector<int> departureTimes = parseScheduleDescription(scheduleDescriptionString);

    const int minWaitTimeBusID = *std::min_element(departureTimes.cbegin(), departureTimes.cend(), [earliestDepartureTimestamp](const auto& lhs, const auto& rhs)
                                                   {
                                                       return getWaitTime(earliestDepartureTimestamp, lhs) < getWaitTime(earliestDepartureTimestamp, rhs);
                                                   });

    return getWaitTime(earliestDepartureTimestamp, minWaitTimeBusID) * minWaitTimeBusID;
}

}
