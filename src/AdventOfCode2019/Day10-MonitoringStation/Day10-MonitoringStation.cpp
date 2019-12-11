#include "Day10-MonitoringStation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>
#include <boost/integer/common_factor.hpp>
#include <boost/math/constants/constants.hpp>

#include <Eigen/Dense>

#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const int SOME_NUMBER_LARGER_THAN_GRID_SIZE = 10000;
}

namespace AdventOfCode
{

using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

using Vector2D = Eigen::Matrix<int, 2, 1>;

class AsteroidMap
{
public:
    AsteroidMap(CoordinatesSet asteroidCoordinatesSet)
        : m_asteroidCoordinatesSet{std::move(asteroidCoordinatesSet)}
        , m_laserVector{-1, -SOME_NUMBER_LARGER_THAN_GRID_SIZE}
    {
        m_locatorPosition = determineLocatorPosition();
        m_asteroidCoordinatesSet.erase(m_locatorPosition);
    }

    unsigned getMaxNumAsteroidsDetected() const
    {
        return getNumAsteroidsDetected(m_locatorPosition);
    }

    std::vector<Coordinates> vaporize()
    {
        while (!m_asteroidCoordinatesSet.empty())
        {
            vaporizeNextAsteroid();
        }

        return m_vaporizationOrder;
    }

private:
    CoordinatesSet m_asteroidCoordinatesSet;
    Coordinates m_locatorPosition;
    Vector2D m_laserVector;

    std::vector<Coordinates> m_vaporizationOrder;

    Coordinates determineLocatorPosition() const
    {
        return *std::max_element(m_asteroidCoordinatesSet.cbegin(), m_asteroidCoordinatesSet.cend(), [this](const auto& lhs, const auto& rhs)
                         {
                             return this->getNumAsteroidsDetected(lhs) < this->getNumAsteroidsDetected(rhs);
                         });
    }

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

    void vaporizeNextAsteroid()
    {
        Coordinates nextAsteroidToVaporize = findNextAsteroidToVaporize();
        Vector2D nextAsteroidToVaporizeVector{nextAsteroidToVaporize.first, nextAsteroidToVaporize.second};

        Vector2D locatorVector{m_locatorPosition.first, m_locatorPosition.second};

        m_laserVector = nextAsteroidToVaporizeVector - locatorVector;

        m_vaporizationOrder.push_back(nextAsteroidToVaporize);
        m_asteroidCoordinatesSet.erase(nextAsteroidToVaporize);
    }

    Coordinates findNextAsteroidToVaporize() const
    {
        Coordinates closestAsteroid;
        double minAngle = std::numeric_limits<double>::max();

        for (const auto& asteroidCoordinates : m_asteroidCoordinatesSet)
        {
            double angle = getLaserAngleToAsteroid(asteroidCoordinates);

            // The laser has to spin to some non-zero degree, so zero degrees
            // actually means that  a full revolution has to be made
            if (angle == 0)
            {
                angle = 2 * boost::math::constants::pi<double>();
            }

            if (angle <= minAngle && isThereDirectLineOfSight(m_locatorPosition, asteroidCoordinates))
            {
                minAngle = angle;
                closestAsteroid = asteroidCoordinates;
            }
        }

        return closestAsteroid;
    }

    double getLaserAngleToAsteroid(const Coordinates& asteroidCoordinates) const
    {
        Vector2D fromVector{m_laserVector};

        Vector2D asteroidVector{asteroidCoordinates.first, asteroidCoordinates.second};
        Vector2D locatorVector{m_locatorPosition.first, m_locatorPosition.second};

        Vector2D toVector{asteroidVector - locatorVector};

        double angle = std::atan2(toVector[1], toVector[0]) - std::atan2(fromVector[1], fromVector[0]);

        double pi = boost::math::constants::pi<double>();
        if (angle < 0)
        {
            angle += 2 * pi;
        }

        // We are looking for negative angles, so we need to multiply the angle by -1.
        // And we are in a mirrored coordinate sytem, so we need to multiply it by -1 again.
        // These two operations simplified just mean that we can return the angle as is.
        return angle;
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

std::vector<Coordinates> vaporizationOrder(const std::vector<std::string> asteroidMapLines)
{
    CoordinatesSet asteroidCoordinatesSet = createAsteroidCoordinatesSet(asteroidMapLines);

    AsteroidMap asteroidMap{asteroidCoordinatesSet};

    return asteroidMap.vaporize();
}

}
