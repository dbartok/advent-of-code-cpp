#include "Day17-TrickShot.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/dense>

#include <regex>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day17
{

using Vector2D = Eigen::Vector2i;

struct TargetArea
{
    Vector2D lowerBoundVector;
    Vector2D upperBoundVector;
};

class ProbeLauncherSimulator
{
public:
    ProbeLauncherSimulator(TargetArea targetArea)
        : m_targetArea{std::move(targetArea)}
    {

    }

    int getHighestPossibleYPosition() const
    {
        if (!canProbeStopHorizonatallyInsideTargetArea())
        {
            throw std::runtime_error("Probe horizontal stop condition violated");
        }
        const int highestInitialVelocityY = std::abs(m_targetArea.lowerBoundVector.y()) - 1;
        const int highestPossibleYPosition = highestInitialVelocityY * (highestInitialVelocityY + 1) / 2;
        return highestPossibleYPosition;
    }

private:
    TargetArea m_targetArea;

    bool canProbeStopHorizonatallyInsideTargetArea() const
    {
        int initialVelocityX = sqrt(std::abs(m_targetArea.upperBoundVector.x()));
        while (true)
        {
            const int stopPositionX = initialVelocityX * (initialVelocityX + 1) / 2;
            if (stopPositionX >= m_targetArea.lowerBoundVector.x())
            {
                return stopPositionX <= m_targetArea.upperBoundVector.x();
            }

            ++initialVelocityX;
        }
    }
};

TargetArea parseTargetAreaString(const std::string& targetAreaString)
{
    std::regex coordinatesRegex(R"(target area: x=(-?[0-9]+)..(-?[0-9]+), y=(-?[0-9]+)..(-?[0-9]+))");
    std::smatch coordinatesMatch;

    std::regex_match(targetAreaString, coordinatesMatch, coordinatesRegex);

    const int lowerBoundX = std::stoi(coordinatesMatch[1].str());
    const int upperBoundX = std::stoi(coordinatesMatch[2].str());
    const int lowerBoundY = std::stoi(coordinatesMatch[3].str());
    const int upperBoundY = std::stoi(coordinatesMatch[4].str());

    return TargetArea{{lowerBoundX, lowerBoundY}, {upperBoundX, upperBoundY}};
}

int highestYPositionToHitTarget(const std::string& targetAreaString)
{
    TargetArea targetArea = parseTargetAreaString(targetAreaString);
    ProbeLauncherSimulator probeLauncherSimulator{std::move(targetArea)};
    return probeLauncherSimulator.getHighestPossibleYPosition();
}

}
}
}