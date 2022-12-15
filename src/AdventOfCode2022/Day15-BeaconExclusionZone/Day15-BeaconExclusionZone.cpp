#include "Day15-BeaconExclusionZone.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/icl/interval_set.hpp>

#include <regex>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2022
{
namespace Day15
{

using Coordinates = std::pair<int, int>;
using CoordinatesPair = std::pair<Coordinates, Coordinates>;
using IntIntervalSet = boost::icl::interval_set<int>;

class SensorReportAnalyzer
{
public:
    SensorReportAnalyzer(const std::vector<CoordinatesPair> sensorAndBeaconPositions)
    {
        for (const auto& sensorAndBeaconPosition : sensorAndBeaconPositions)
        {
            const auto& sensorPosition = sensorAndBeaconPosition.first;
            const auto& beaconPosition = sensorAndBeaconPosition.second;

            const int distanceX = std::abs(sensorPosition.first - beaconPosition.first);
            const int distanceY = std::abs(sensorPosition.second - beaconPosition.second);
            const int exclusionRadius = distanceX + distanceY;

            m_sensorPositionsAndExclusionRadiuses.emplace_back(sensorPosition, exclusionRadius);
        }
    }

    unsigned getNumPositionsThatCannotContainBeaconInSpecificRow(int y)
    {
        IntIntervalSet exclusionZone;

        for (const auto& sensorPositionAndExclusionRadius : m_sensorPositionsAndExclusionRadiuses)
        {
            IntIntervalSet::interval_type excludedInterval = getExclusionIntervalInSpecificRow(sensorPositionAndExclusionRadius.first, sensorPositionAndExclusionRadius.second, y);

            exclusionZone.insert(excludedInterval);
        }

        return exclusionZone.iterative_size();
    }

private:
    std::vector<std::pair<Coordinates, int>> m_sensorPositionsAndExclusionRadiuses;

    static IntIntervalSet::interval_type getExclusionIntervalInSpecificRow(const Coordinates& sensorPosition, int exclusionRadius, int y)
    {
        const int offsetY = std::abs(sensorPosition.second - y);
        const int remainingExtraRadius = exclusionRadius - offsetY;

        if (remainingExtraRadius < 0)
        {
            return IntIntervalSet::interval_type{};
        }

        return IntIntervalSet::interval_type{sensorPosition.first - remainingExtraRadius, sensorPosition.first + remainingExtraRadius + 1};
    }

};

CoordinatesPair parseSensorAndBeaconPositionLine(const std::string& sensorAndBeaconPositionLine)
{
    const std::regex sensorAndBeaconPositionLineRegex(R"(Sensor at x=(-?[0-9]+), y=(-?[0-9]+): closest beacon is at x=(-?[0-9]+), y=(-?[0-9]+))");
    std::smatch sensorAndBeaconPositionLineMatch;

    std::regex_match(sensorAndBeaconPositionLine, sensorAndBeaconPositionLineMatch, sensorAndBeaconPositionLineRegex);

    int sensorX = std::stoi(sensorAndBeaconPositionLineMatch[1]);
    int sensorY = std::stoi(sensorAndBeaconPositionLineMatch[2]);
    int beaconX = std::stoi(sensorAndBeaconPositionLineMatch[3]);
    int beaconY = std::stoi(sensorAndBeaconPositionLineMatch[4]);

    return {{sensorX, sensorY}, {beaconX, beaconY}};
}

std::vector<CoordinatesPair> parseSensorAndBeaconPositionLines(const std::vector<std::string>& sensorAndBeaconPositionLines)
{
    std::vector<CoordinatesPair> sensorAndBeaconPositions;

    for (const auto& sensorAndBeaconPositionLine : sensorAndBeaconPositionLines)
    {
        CoordinatesPair sensorAndBeaconPosition = parseSensorAndBeaconPositionLine(sensorAndBeaconPositionLine);
        sensorAndBeaconPositions.push_back(std::move(sensorAndBeaconPosition));
    }

    return sensorAndBeaconPositions;
}

unsigned numPositionsThatCannotContainBeaconInSpecificRow(const std::vector<std::string>& sensorAndBeaconPositionLines, int y)
{
    std::vector<CoordinatesPair> sensorAndBeaconPositions = parseSensorAndBeaconPositionLines(sensorAndBeaconPositionLines);

    SensorReportAnalyzer sensorReportAnalyzer{std::move(sensorAndBeaconPositions)};

    return sensorReportAnalyzer.getNumPositionsThatCannotContainBeaconInSpecificRow(y);
}

}
}
}