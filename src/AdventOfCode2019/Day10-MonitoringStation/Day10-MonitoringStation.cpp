#include "Day10-MonitoringStation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>
#include <boost/integer/common_factor.hpp>

#include <Eigen/Dense>

#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<int, int>;
using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

using Vector2D = Eigen::Matrix<int, 2, 1>;

class AsteroidMap
{
public:
    AsteroidMap(CoordinatesSet asteroidCoordinatesSet)
        : m_asteroidCoordinatesSet{std::move(asteroidCoordinatesSet)}
    {

    }

    unsigned getMaxNumAsteroidsDetected() const
    {
        auto maxIter = std::max_element(m_asteroidCoordinatesSet.cbegin(), m_asteroidCoordinatesSet.cend(), [this](const auto& lhs, const auto& rhs)
                                        {
                                            return this->getNumAsteroidsDetected(lhs) < this->getNumAsteroidsDetected(rhs);
                                        });

        return getNumAsteroidsDetected(*maxIter);
    }

private:
    CoordinatesSet m_asteroidCoordinatesSet;

    unsigned getNumAsteroidsDetected(const Coordinates& stationPos) const
    {
        unsigned numAsteroidsDetected = 0;

        for (const auto& asteroidCoordinates : m_asteroidCoordinatesSet)
        {
            if (asteroidCoordinates == stationPos)
            {
                continue;
            }

            if (isThereDirectLineOfSight(stationPos, asteroidCoordinates))
            {
                ++numAsteroidsDetected;
            }
        }

        return numAsteroidsDetected;
    }

    bool isThereDirectLineOfSight(const Coordinates& fromCoordinates, const Coordinates& toCoordinates) const
    {
        Vector2D fromVector{fromCoordinates.first, fromCoordinates.second};
        Vector2D toVector{toCoordinates.first, toCoordinates.second};

        Vector2D differenceVector = toVector - fromVector;
        Vector2D differenceVectorAbs = differenceVector.cwiseAbs();

        int gcd = boost::integer::gcd(differenceVectorAbs[0], differenceVectorAbs[1]);

        Vector2D differenceBetweenTwoMidpointsVector = differenceVector / gcd;

        for (Vector2D midpointVector = fromVector + differenceBetweenTwoMidpointsVector; midpointVector != toVector; midpointVector += differenceBetweenTwoMidpointsVector)
        {
            Coordinates midpointCoordinates{midpointVector[0], midpointVector[1]};
            if (m_asteroidCoordinatesSet.find(midpointCoordinates) != m_asteroidCoordinatesSet.cend())
            {
                return false;
            }
        }

        return true;
    }
};

CoordinatesSet createAsteroidCoordinatesSet(const std::vector<std::string> asteroidMapLines)
{
    CoordinatesSet asteroidCoordinatesSet;

    for (int j = 0; j < asteroidMapLines.size(); ++j)
    {
        for (int i = 0; i < asteroidMapLines.at(j).size(); ++i)
        {
            if (asteroidMapLines.at(j).at(i) == '#')
            {
                asteroidCoordinatesSet.emplace(i, j);
            }
        }
    }

    return asteroidCoordinatesSet;
}

unsigned maxNumAsteroidsDetected(const std::vector<std::string> asteroidMapLines)
{
    CoordinatesSet asteroidCoordinatesSet = createAsteroidCoordinatesSet(asteroidMapLines);

    AsteroidMap asteroidMap{asteroidCoordinatesSet};

    return asteroidMap.getMaxNumAsteroidsDetected();
}

}
