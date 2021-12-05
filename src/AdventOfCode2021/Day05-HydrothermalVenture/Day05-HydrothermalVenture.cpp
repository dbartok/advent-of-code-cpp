#include "Day05-HydrothermalVenture.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/integer/common_factor.hpp>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day05
{

using Vector2D = Eigen::Matrix<int, 2, 1>;

struct Vector2DHash
{
    size_t operator()(const Vector2D& vec) const
    {
        size_t seed = 0;

        boost::hash_combine(seed, vec.x());
        boost::hash_combine(seed, vec.y());

        return seed;
    }
};

struct LineSegment
{
    Vector2D start;
    Vector2D end;

    bool isAxisParallel() const
    {
        return start.x() == end.x() || start.y() == end.y();
    }

    std::vector<Vector2D> getCoveredPoints() const
    {
        const Vector2D differenceVector = end - start;
        const Vector2D differenceVectorAbs = differenceVector.cwiseAbs();

        const int numStepsFromStartToEnd = boost::integer::gcd(differenceVectorAbs[0], differenceVectorAbs[1]);

        const Vector2D stepVector = differenceVector / numStepsFromStartToEnd;

        std::vector<Vector2D> coveredPoints;
        for (Vector2D innerPointVector = start; ; innerPointVector += stepVector)
        {
            coveredPoints.push_back(innerPointVector);

            if (innerPointVector == end)
            {
                break;
            }
        }

        return coveredPoints;
    }
};

class HydrothermalVentAnalyzer
{
public:
    HydrothermalVentAnalyzer(std::vector<LineSegment> ventLineSegments)
        : m_ventLineSegments{std::move(ventLineSegments)}
    {

    }

    void analyzeOverlapsForAxisParallelLines()
    {
        for (const auto& ventLineSegment : m_ventLineSegments)
        {
            if (ventLineSegment.isAxisParallel())
            {
                markCoveredPoints(ventLineSegment);
            }
        }
    }

    int getNumPointsWithOverlap() const
    {
        return std::count_if(m_coordinatesToNumVents.cbegin(), m_coordinatesToNumVents.cend(), [](const auto& elem)
                             {
                                 return elem.second > 1;
                             });
    }

private:
    std::vector<LineSegment> m_ventLineSegments;
    std::unordered_map<Vector2D, int, Vector2DHash> m_coordinatesToNumVents;

    void markCoveredPoints(const LineSegment& ventLineSegment)
    {
        std::vector<Vector2D> coveredPoints = ventLineSegment.getCoveredPoints();

        for (const auto& coveredPoint : coveredPoints)
        {
            ++m_coordinatesToNumVents[coveredPoint];
        }
    }
};

Vector2D parseCoordinatesString(const std::string& coordinatesString)
{
    std::vector<std::string> tokens;
    boost::split(tokens, coordinatesString, boost::is_any_of(","));

    return Vector2D{std::stoi(tokens.at(0)), std::stoi(tokens.at(1))};
}

LineSegment parseVentLine(const std::string& ventLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, ventLine, boost::is_any_of(" ->"), boost::token_compress_on);

    Vector2D start = parseCoordinatesString(tokens.at(0));
    Vector2D end = parseCoordinatesString(tokens.at(1));

    return LineSegment{std::move(start), std::move(end)};
}


std::vector<LineSegment> parseVentLines(const std::vector<std::string>& ventLines)
{
    std::vector<LineSegment> ventLineSegments;

    for (const auto& line : ventLines)
    {
        LineSegment ventLineSegment = parseVentLine(line);
        ventLineSegments.push_back(std::move(ventLineSegment));
    }

    return ventLineSegments;
}

int numPointsWhereHorizontalOrVerticalLinesOverlap(const std::vector<std::string>& ventLines)
{
    std::vector<LineSegment> ventLineSegments = parseVentLines(ventLines);
    HydrothermalVentAnalyzer hydrothermalVentAnalyzer{ventLineSegments};
    hydrothermalVentAnalyzer.analyzeOverlapsForAxisParallelLines();
    return hydrothermalVentAnalyzer.getNumPointsWithOverlap();
}

}
}
}