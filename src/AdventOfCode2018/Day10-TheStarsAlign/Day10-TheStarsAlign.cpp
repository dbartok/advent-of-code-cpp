#include "Day10-TheStarsAlign.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>

#include <boost/algorithm/string.hpp>

#include <regex>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using CoordinateNumericType = long long;
using Vector2D = Eigen::Matrix<CoordinateNumericType, 2, 1>;

struct Starlight
{
    Vector2D position;
    Vector2D velocity;

    Starlight(Vector2D position, Vector2D velocity)
        : position{std::move(position)}
        , velocity{std::move(velocity)}
    {

    }
};

struct StarBoundingBox
{
    Vector2D offset;
    Vector2D size;

    StarBoundingBox(Vector2D offset, Vector2D size)
        : offset{std::move(offset)}
        , size{std::move(size)}
    {

    }
};

class StarAligner
{
public:
    StarAligner(std::vector<Starlight> starlights)
        : m_starlights{std::move(starlights)}
        , m_timeElapsed{0}
    {

    }

    void align()
    {
        CoordinateNumericType previousBoundingBoxArea = std::numeric_limits<CoordinateNumericType>::max();

        while (true)
        {
            stepOneTimeUnit();

            StarBoundingBox currentBoundingBox = getBoundingBox();
            CoordinateNumericType currentBoundingBoxArea = currentBoundingBox.size[0] * currentBoundingBox.size[1];

            if (currentBoundingBoxArea > previousBoundingBoxArea)
            {
                break;
            }

            previousBoundingBoxArea = currentBoundingBoxArea;
            ++m_timeElapsed;
        }

        rewindOneTimeUnit();
    }

    void stepOneTimeUnit()
    {
        for (auto& starlight : m_starlights)
        {
            starlight.position += starlight.velocity;
        }
    }

    void rewindOneTimeUnit()
    {
        for (auto& starlight : m_starlights)
        {
            starlight.position -= starlight.velocity;
        }
    }

    std::string getSerializedMessage() const
    {
        StarBoundingBox boundingBox = getBoundingBox();
        std::vector<std::string> serializedLines(boundingBox.size[1], std::string(boundingBox.size[0], '.'));

        for (const auto& starlight : m_starlights)
        {
            Vector2D offsetStarlightPosition{starlight.position - boundingBox.offset};

            CoordinateNumericType posX = offsetStarlightPosition[0];
            CoordinateNumericType posY = offsetStarlightPosition[1];

            serializedLines[posY][posX] = '#';
        }

        return boost::algorithm::join(serializedLines, "\n");
    }

    unsigned getTimeUnitsElapsed() const
    {
        return m_timeElapsed;
    }

private:
    std::vector<Starlight> m_starlights;
    unsigned m_timeElapsed;

    StarBoundingBox getBoundingBox() const
    {
        auto minmaxPositionXIter = std::minmax_element(m_starlights.cbegin(), m_starlights.cend(), [](const Starlight& lhs, const Starlight& rhs)
                                                   {
                                                       return lhs.position[0] < rhs.position[0];
                                                   });

        CoordinateNumericType minX = minmaxPositionXIter.first->position[0];
        CoordinateNumericType maxX = minmaxPositionXIter.second->position[0];

        auto minmaxPositionYIter = std::minmax_element(m_starlights.cbegin(), m_starlights.cend(), [](const Starlight& lhs, const Starlight& rhs)
                                                       {
                                                           return lhs.position[1] < rhs.position[1];
                                                       });

        CoordinateNumericType minY = minmaxPositionYIter.first->position[1];
        CoordinateNumericType maxY = minmaxPositionYIter.second->position[1];

        return StarBoundingBox{Vector2D{minX, minY}, Vector2D{maxX - minX + 1, maxY - minY + 1}};
    }
};

std::vector<Starlight> parseStarlights(const std::vector<std::string>& starlightLines)
{
    std::vector<Starlight> starlights;

    for (const auto& line : starlightLines)
    {
        std::regex coordinatesRegex(R"(position=<\s*(-?[0-9]+),\s*(-?[0-9]+)>\s*velocity=<\s*(-?[0-9]+),\s*(-?[0-9]+)>)");
        std::smatch coordinatesMatch;

        std::regex_match(line, coordinatesMatch, coordinatesRegex);

        CoordinateNumericType posX = std::stoi(coordinatesMatch[1].str());
        CoordinateNumericType posY = std::stoi(coordinatesMatch[2].str());
        CoordinateNumericType velocityX = std::stoi(coordinatesMatch[3].str());
        CoordinateNumericType velocityY = std::stoi(coordinatesMatch[4].str());

        starlights.emplace_back(Vector2D{posX, posY}, Vector2D{velocityX, velocityY});

    }

    return starlights;

}

std::string serializedMessageWhenAligned(const std::vector<std::string>& starlightLines)
{
    std::vector<Starlight> starlights = parseStarlights(starlightLines);

    StarAligner starAligner{std::move(starlights)};

    starAligner.align();

    return starAligner.getSerializedMessage();
}

unsigned timeTakenUntilAligned(const std::vector<std::string>& starlightLines)
{
    std::vector<Starlight> starlights = parseStarlights(starlightLines);

    StarAligner starAligner{std::move(starlights)};

    starAligner.align();

    return starAligner.getTimeUnitsElapsed();
}

}
