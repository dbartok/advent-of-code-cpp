#include "Day06-ChronalCoordinates.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<unsigned, unsigned>;
using CoordinateIDType = char;

const CoordinateIDType NO_CLAIM_COORDINATE_ID = '.';

class GridAnalyzer
{
public:
    GridAnalyzer(std::vector<Coordinates> allCoordinates)
        : m_coordinateIDToCoordinates{}
        , m_highestCoordinates{0, 0}
    {
        char coordinateID = 'a';
        for (auto& coordinates : allCoordinates)
        {
            m_highestCoordinates.first = std::max(m_highestCoordinates.first, coordinates.first);
            m_highestCoordinates.second = std::max(m_highestCoordinates.second, coordinates.second);

            m_coordinateIDToCoordinates.emplace(coordinateID++, std::move(coordinates));
        }
    }

    unsigned getLargestNonInfiniteArea()
    {
        std::unordered_map<char, unsigned> coordinateIDToArea;
        std::unordered_set<CoordinateIDType> edgeIDs;

        for (unsigned i = 0; i <= m_highestCoordinates.first; ++i)
        {
            for (unsigned j = 0; j <= m_highestCoordinates.second; ++j)
            {
                CoordinateIDType coordID = getClosestCoordinateID(i, j);
                ++coordinateIDToArea[coordID];

                if (i == 0 || j == 0 || i == m_highestCoordinates.first || j == m_highestCoordinates.second)
                {
                    edgeIDs.insert(coordID);
                }
            }
        }

        for (const auto& edgeID : edgeIDs)
        {
            coordinateIDToArea.erase(edgeID);
        }

        auto maxIter = std::max_element(coordinateIDToArea.begin(), coordinateIDToArea.end(),
                                        [](const auto& lhs, const auto& rhs)
                                        {
                                            return lhs.second < rhs.second;
                                        });

        return maxIter->second;
    }

    unsigned getAreaWithinGivenDistance(unsigned distanceThreshold)
    {
        unsigned areaSize = 0;
        for (unsigned i = 0; i <= m_highestCoordinates.first; ++i)
        {
            for (unsigned j = 0; j <= m_highestCoordinates.second; ++j)
            {
                if (areCoordinatesWithDistanceThreshold(i, j, distanceThreshold))
                {
                    ++areaSize;
                }
            }
        }

        return areaSize;
    }

private:
    std::unordered_map<CoordinateIDType, Coordinates> m_coordinateIDToCoordinates;
    Coordinates m_highestCoordinates;

    CoordinateIDType getClosestCoordinateID(unsigned i, unsigned j)
    {
        std::vector<std::pair<char, unsigned>> coordinateIDAndDistances = getCoordinateIDAndDistances(i, j);

        std::nth_element(coordinateIDAndDistances.begin(), coordinateIDAndDistances.begin() + 1, coordinateIDAndDistances.end(),
                         [](const auto& lhs, const auto& rhs)
                         {
                             return lhs.second < rhs.second;
                         });

        const auto& maxElem = coordinateIDAndDistances[0];
        const auto& secondMaxElem = coordinateIDAndDistances[1];

        if (maxElem.second == secondMaxElem.second)
        {
            return NO_CLAIM_COORDINATE_ID;
        }

        return maxElem.first;
    }

    bool areCoordinatesWithDistanceThreshold(unsigned i, unsigned j, unsigned distanceThreshold)
    {
        std::vector<std::pair<char, unsigned>> coordinateIDAndDistances = getCoordinateIDAndDistances(i, j);

        unsigned totalDistance = std::accumulate(coordinateIDAndDistances.cbegin(), coordinateIDAndDistances.cend(), 0u,
                                                 [](unsigned acc, const auto& elem)
                                                 {
                                                     return acc + elem.second;
                                                 });

        return totalDistance < distanceThreshold;
    }

    std::vector<std::pair<char, unsigned>> getCoordinateIDAndDistances(unsigned i, unsigned j)
    {
        std::vector<std::pair<char, unsigned>> coordinateIDAndDistances;

        for (const auto& coordinateIDAndCoorindates : m_coordinateIDToCoordinates)
        {
            CoordinateIDType coordinateID;
            Coordinates coordinates;

            std::tie(coordinateID, coordinates) = coordinateIDAndCoorindates;

            unsigned distanceX = std::max(i, coordinates.first) - std::min(i, coordinates.first);
            unsigned distanceY = std::max(j, coordinates.second) - std::min(j, coordinates.second);

            unsigned manhattanDistance = distanceX + distanceY;

            coordinateIDAndDistances.emplace_back(coordinateID, manhattanDistance);
        }

        return coordinateIDAndDistances;
    }
};

std::vector<Coordinates> parseCoordinates(const std::vector<std::string>& coordinatesLines)
{
    std::vector<Coordinates> allCoordinates;

    for (const auto& lines : coordinatesLines)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, lines, boost::is_any_of(" ,"), boost::token_compress_on);

        unsigned x = boost::lexical_cast<unsigned>(tokens[0]);
        unsigned y = boost::lexical_cast<unsigned>(tokens[1]);

        allCoordinates.emplace_back(x, y);
    }

    return allCoordinates;
}

unsigned sizeOfLargestNonInfiniteArea(const std::vector<std::string>& coordinatesLines)
{
    std::vector<Coordinates> allCoordinates = parseCoordinates(coordinatesLines);

    GridAnalyzer gridAnalyzer{std::move(allCoordinates)};

    return gridAnalyzer.getLargestNonInfiniteArea();
}

unsigned sizeOfAreaWithinGivenDistance(const std::vector<std::string>& coordinatesLines, unsigned distanceThreshold)
{
    std::vector<Coordinates> allCoordinates = parseCoordinates(coordinatesLines);

    GridAnalyzer gridAnalyzer{std::move(allCoordinates)};

    return gridAnalyzer.getAreaWithinGivenDistance(distanceThreshold);
}

}
