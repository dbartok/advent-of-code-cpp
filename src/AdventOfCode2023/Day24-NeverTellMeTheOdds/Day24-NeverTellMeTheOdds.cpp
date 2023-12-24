#include "Day24-NeverTellMeTheOdds.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
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

struct Hailstone
{
    Vector3D position;
    Vector3D velocity;
};

struct Intersection
{
    Vector3D position;
    std::pair<double, double> times;
};

class HailstoneSystem
{
public:
    HailstoneSystem(std::vector<Hailstone> hailstones)
        : m_hailstones{std::move(hailstones)}
    {

    }

    int getNumXYIntersectionsWithinTestArea(double testAreaMin, double testAreaMax) const
    {
        int numXYIntersectionsWithinTestArea = 0;

        for (int i = 0; i < m_hailstones.size(); ++i)
        {
            for (int j = i + 1; j < m_hailstones.size(); ++j)
            {
                Intersection intersection = getXYIntersection(m_hailstones.at(i), m_hailstones.at(j));

                if (intersection.position.x() >= testAreaMin && intersection.position.x() <= testAreaMax &&
                    intersection.position.y() >= testAreaMin && intersection.position.y() <= testAreaMax &&
                    intersection.times.first >= 0 && intersection.times.second >= 0)
                {
                    ++numXYIntersectionsWithinTestArea;
                }
            }
        }

        return numXYIntersectionsWithinTestArea;
    }

private:
    std::vector<Hailstone> m_hailstones;

    Intersection getXYIntersection(const Hailstone& firstHailstone, const Hailstone& secondHailstone) const
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

        const double firstHailstoneTime = ((intersectionX - firstHailstone.position.x()) / firstHailstone.velocity.x()).convert_to<double>();
        const double secondHailstoneTime = ((intersectionX - secondHailstone.position.x()) / secondHailstone.velocity.x()).convert_to<double>();

        const PreciseFloat intersectionY = firstHailstone.position.y() + firstHailstone.velocity.y() * firstHailstoneTime;

        return Intersection{{intersectionX, intersectionY, 0}, {firstHailstoneTime, secondHailstoneTime}};
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

}
}
}