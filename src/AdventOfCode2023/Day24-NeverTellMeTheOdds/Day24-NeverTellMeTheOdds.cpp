#include "Day24-NeverTellMeTheOdds.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <Eigen/dense>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day24
{

using PreciseFloat = boost::multiprecision::cpp_dec_float_100;
using Vector3D = Eigen::Matrix<PreciseFloat, 3, 1>;

PreciseFloat EPSILON = std::numeric_limits<PreciseFloat>::epsilon();

struct Hailstone
{
    Vector3D position;
    Vector3D velocity;
};

class HailstoneSystem
{
public:
    HailstoneSystem(std::vector<Hailstone> hailstones)
        : m_hailstones{std::move(hailstones)}
    {

    }

    void calculateRockStartingPositionForRockVelocityXY(const Vector3D rockVelocityXY)
    {
        adjustVelocitiesForRockVelocity(rockVelocityXY);

        boost::optional<Vector3D> commonXYIntersection = getCommonXYIntersection();

        if (!commonXYIntersection)
        {
            return;
        }

        boost::optional<PreciseFloat> rockVelocityZ = getCommonRockVelocityZ(commonXYIntersection.get());

        if (!rockVelocityZ)
        {
            return;
        }

        const PreciseFloat rockStartingPositionZ = getRockStartingPositionZ(commonXYIntersection.get(), rockVelocityZ.get());

        m_rockStartingPosition = Vector3D{commonXYIntersection.get().x(), commonXYIntersection.get().y(), rockStartingPositionZ};
    }

    int getNumXYIntersectionsWithinTestArea(double testAreaMin, double testAreaMax) const
    {
        int numXYIntersectionsWithinTestArea = 0;

        for (int i = 0; i < m_hailstones.size(); ++i)
        {
            for (int j = i + 1; j < m_hailstones.size(); ++j)
            {
                boost::optional<Vector3D> intersection = getXYIntersection(m_hailstones.at(i), m_hailstones.at(j));

                if (!intersection)
                {
                    continue;
                }

                const auto& intersectionPosition = intersection.get();

                if (intersectionPosition.x() >= testAreaMin && intersectionPosition.x() <= testAreaMax &&
                    intersectionPosition.y() >= testAreaMin && intersectionPosition.y() <= testAreaMax)
                {
                    ++numXYIntersectionsWithinTestArea;
                }
            }
        }

        return numXYIntersectionsWithinTestArea;
    }

    boost::optional<Vector3D> getRockStartingPosition() const
    {
        return m_rockStartingPosition;
    }

private:
    std::vector<Hailstone> m_hailstones;

    boost::optional<Vector3D> m_rockStartingPosition;

    void adjustVelocitiesForRockVelocity(const Vector3D rockVelocity)
    {
        for (auto& hailstone : m_hailstones)
        {
            hailstone.velocity -= rockVelocity;
        }
    }

    boost::optional<Vector3D> getCommonXYIntersection() const
    {
        boost::optional<Vector3D> firstIntersection = getXYIntersection(m_hailstones.at(0), m_hailstones.at(1));

        if (!firstIntersection)
        {
            return boost::none;
        }

        for (int i = 2; i < m_hailstones.size(); ++i)
        {
            boost::optional<Vector3D> currentIntersection = getXYIntersection(m_hailstones.at(0), m_hailstones.at(i));

            if (!currentIntersection || !currentIntersection.get().isApprox(firstIntersection.get(), EPSILON))
            {
                return boost::none;
            }
        }

        return firstIntersection;
    }

    boost::optional<Vector3D> getXYIntersection(const Hailstone& firstHailstone, const Hailstone& secondHailstone) const
    {
        // https://en.wikipedia.org/wiki/Line–line_intersection#Given_two_points_on_each_line

        const PreciseFloat x1 = firstHailstone.position.x();
        const PreciseFloat y1 = firstHailstone.position.y();
        const PreciseFloat x2 = firstHailstone.position.x() + firstHailstone.velocity.x();
        const PreciseFloat y2 = firstHailstone.position.y() + firstHailstone.velocity.y();

        const PreciseFloat x3 = secondHailstone.position.x();
        const PreciseFloat y3 = secondHailstone.position.y();
        const PreciseFloat x4 = secondHailstone.position.x() + secondHailstone.velocity.x();
        const PreciseFloat y4 = secondHailstone.position.y() + secondHailstone.velocity.y();

        const PreciseFloat intersectionX = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
        const PreciseFloat intersectionY = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
        Vector3D intersection{intersectionX, intersectionY, 0};

        const PreciseFloat firstHailstoneTime = getTimeUntilXYPosition(firstHailstone, intersection);
        const PreciseFloat secondHailstoneTime = getTimeUntilXYPosition(secondHailstone, intersection);

        if (firstHailstoneTime < 0 || secondHailstoneTime < 0)
        {
            return boost::none;
        }

        return intersection;
    }

    boost::optional<PreciseFloat> getCommonRockVelocityZ(const Vector3D& commonXYIntersection) const
    {
        PreciseFloat firstRockVelocityZ = getRockVelocityZ(m_hailstones.at(0), m_hailstones.at(1), commonXYIntersection);

        for (int i = 2; i < m_hailstones.size(); ++i)
        {
            const PreciseFloat currentRockVelocityZ = getRockVelocityZ(m_hailstones.at(0), m_hailstones.at(i), commonXYIntersection);

            if (boost::multiprecision::abs(firstRockVelocityZ - currentRockVelocityZ) > EPSILON)
            {
                return boost::none;
            }
        }

        return firstRockVelocityZ;
    }

    PreciseFloat getRockVelocityZ(const Hailstone& firstHailstone, const Hailstone& secondHailstone, const Vector3D& commonXYIntersection) const
    {
        const PreciseFloat firstHailstoneIntersectionTime = getTimeUntilXYPosition(firstHailstone, commonXYIntersection);
        const PreciseFloat secondHailstoneIntersectionTime = getTimeUntilXYPosition(secondHailstone, commonXYIntersection);

        // Looking for the z component of the velocity where the two hailstones arrive at commonZPosition exactly when they also arrive at the XY intersection
        // Using the following two equations:
        // commonZPosition = firstHailstone.position.z + firstHailstoneIntersectionTime * (firstHailstone.velocity.z - rockVelocityZ)
        // commonZPosition = secondHailstone.position.z + secondHailstoneIntersectionTime * (secondHailstone.velocity.z - rockVelocityZ)
        //
        // firstHailstone.position.z + firstHailstoneIntersectionTime * firstHailstone.velocity.z - firstHailstoneIntersectionTime * rockVelocityZ = secondHailstone.position.z + secondHailstoneIntersectionTime * secondHailstone.velocity.z - secondHailstoneIntersectionTime * rockVelocityZ
        // firstHailstone.position.z + firstHailstoneIntersectionTime * firstHailstone.velocity.z - secondHailstone.position.z - secondHailstoneIntersectionTime * secondHailstone.velocity.z = firstHailstoneIntersectionTime * rockVelocityZ - secondHailstoneIntersectionTime * rockVelocityZ
        const PreciseFloat rockVelocityZ = (firstHailstone.position.z() + firstHailstoneIntersectionTime * firstHailstone.velocity.z() - secondHailstone.position.z() - secondHailstoneIntersectionTime * secondHailstone.velocity.z()) / (firstHailstoneIntersectionTime - secondHailstoneIntersectionTime);

        return rockVelocityZ;
    }

    PreciseFloat getRockStartingPositionZ(const Vector3D& commonXYIntersection, PreciseFloat rockVelocityZ) const
    {
        // Use the same equation as before, but rockVelocityZ is now known
        const Hailstone& someHailstone = m_hailstones.front();
        const PreciseFloat timeToIntersection = getTimeUntilXYPosition(someHailstone, commonXYIntersection);
        return someHailstone.position.z() + timeToIntersection * (someHailstone.velocity.z() - rockVelocityZ);
    }

    static PreciseFloat getTimeUntilXYPosition(const Hailstone& hailstone, const Vector3D& position)
    {
        if (hailstone.velocity.x() != 0)
        {
            return (position.x() - hailstone.position.x()) / hailstone.velocity.x();
        }
        else
        {
            return (position.y() - hailstone.position.y()) / hailstone.velocity.y();
        }
    }
};

Hailstone parseHailStoneLine(const std::string& hailstoneLine)
{
    std::vector<std::string> tokens;

    boost::split(tokens, hailstoneLine, boost::is_any_of("@, "), boost::token_compress_on);

    Vector3D position{std::stod(tokens.at(0)), std::stod(tokens.at(1)), std::stod(tokens.at(2))};
    Vector3D velocity{std::stod(tokens.at(3)), std::stod(tokens.at(4)), std::stod(tokens.at(5))};

    return Hailstone{std::move(position), std::move(velocity)};
}

std::vector<Hailstone> parseHailStoneLines(const std::vector<std::string>& hailstoneLines)
{
    std::vector<Hailstone> hailstones;

    for (const auto& line : hailstoneLines)
    {
        Hailstone hailstone = parseHailStoneLine(line);
        hailstones.push_back(std::move(hailstone));
    }

    return hailstones;
}

int numXYIntersectionsWithinTestArea(const std::vector<std::string>& hailstoneLines, double testAreaMin, double testAreaMax)
{
    std::vector<Hailstone> hailstones = parseHailStoneLines(hailstoneLines);

    HailstoneSystem hailstoneSystem{std::move(hailstones)};

    return hailstoneSystem.getNumXYIntersectionsWithinTestArea(testAreaMin, testAreaMax);
}

int64_t sumOfCoordinatesOfIniitialRockPosition(const std::vector<std::string>& hailstoneLines)
{
    std::vector<Hailstone> hailstones = parseHailStoneLines(hailstoneLines);

    for (int xySum = 2; ; ++xySum)
    {
        for (int x = 1; x < xySum; ++x)
        {
            for (int minusOneMultiplierBits = 0; minusOneMultiplierBits < 4; ++minusOneMultiplierBits)
            {
                const int y = xySum - x;

                const int signedX = minusOneMultiplierBits % 2 == 1 ? x : -x;
                const int signedY = (minusOneMultiplierBits >> 1) % 2 == 1 ? y : -y;

                HailstoneSystem hailstoneSystem{hailstones};
                hailstoneSystem.calculateRockStartingPositionForRockVelocityXY({signedX, signedY, 0});

                boost::optional<Vector3D> rockStartingPosition = hailstoneSystem.getRockStartingPosition();

                if (rockStartingPosition)
                {
                    return rockStartingPosition.get().sum().convert_to<int64_t>();
                }
            }
        }
    }

    HailstoneSystem hailstoneSystem{std::move(hailstones)};

}

}
}
}