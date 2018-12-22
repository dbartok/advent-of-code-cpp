#include "Day22-ModeMaze.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<unsigned, unsigned>;
using CoordinatesToErosionLevel = std::unordered_map<Coordinates, unsigned, boost::hash<Coordinates>>;

class CaveMap
{
public:
    CaveMap(unsigned depth, Coordinates targetCoordinates)
        : m_depth{depth}
        , m_targetCoordinates{std::move(targetCoordinates)}
    {

    }

    unsigned calculateRiskLevelOfArea()
    {
        unsigned totalRiskLevelOfArea = 0;

        for (size_t j = 0; j <= m_targetCoordinates.first; ++j)
        {
            for (size_t i = 0; i <= m_targetCoordinates.second; ++i)
            {
                unsigned riskLevel = getRiskLevelAt({j, i});
                totalRiskLevelOfArea += riskLevel;
            }
        }
        return totalRiskLevelOfArea;
    }

private:
    unsigned m_depth;
    Coordinates m_targetCoordinates;

    CoordinatesToErosionLevel m_erosionLevelCache;

    unsigned getRiskLevelAt(const Coordinates& coordinates)
    {
        return getErosionLevelAt(coordinates) % 3;
    }

    unsigned getErosionLevelAt(const Coordinates& coordinates)
    {
        unsigned erosionLevel;

        auto findResult = m_erosionLevelCache.find(coordinates);
        if (findResult != m_erosionLevelCache.end())
        {
            erosionLevel = findResult->second;
        }
        else
        {
            erosionLevel = calculateErosionLevelAt(coordinates);
            m_erosionLevelCache.emplace(coordinates, erosionLevel);
        }

        return erosionLevel;
    }

    unsigned calculateErosionLevelAt(const Coordinates& coordinates)
    {
        unsigned geologicalIndex = 0;

        if (coordinates.first == 0 && coordinates.second == 0)
        {
            geologicalIndex = 0;
        }
        else if (coordinates == m_targetCoordinates)
        {
            geologicalIndex = 0;
        }
        else if (coordinates.first == 0)
        {
            geologicalIndex = coordinates.second * 16807;
        }
        else if (coordinates.second == 0)
        {
            geologicalIndex = coordinates.first * 48271;
        }
        else
        {
            unsigned erosionLevelToLeft = getErosionLevelAt({coordinates.first, coordinates.second - 1});
            unsigned erosionLevelToUpwards = getErosionLevelAt({coordinates.first - 1, coordinates.second});

            geologicalIndex = erosionLevelToLeft * erosionLevelToUpwards;
        }

        return (geologicalIndex + m_depth) % 20183;
    }
};

CaveMap parseCaveMap(const std::vector<std::string>& depthAndTargetLines)
{
    std::vector<std::string> tokens;
    boost::split(tokens, depthAndTargetLines.at(0), boost::is_any_of(" "));
    unsigned depth = boost::lexical_cast<unsigned>(tokens.at(1));

    boost::split(tokens, depthAndTargetLines.at(1), boost::is_any_of(" ,"));
    unsigned targetX = boost::lexical_cast<unsigned>(tokens.at(1));
    unsigned targetY = boost::lexical_cast<unsigned>(tokens.at(2));

    return CaveMap{depth, Coordinates{targetY, targetX}};
}

unsigned riskLevelOfSmallestArea(const std::vector<std::string>& depthAndTargetLines)
{
    CaveMap caveMap = parseCaveMap(depthAndTargetLines);

    return caveMap.calculateRiskLevelOfArea();
}

}
