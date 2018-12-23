#include "Day23-EmergencyTeleportation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>

#include <regex>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using NumericType = int;
using Vector3D = Eigen::Matrix<NumericType, 3, 1>;

struct Nanobot
{
    Vector3D position;
    NumericType range;

    Nanobot(Vector3D position, NumericType range)
        : position{std::move(position)}
        , range(range)
    {

    }
};

std::vector<Nanobot> parseNanobots(const std::vector<std::string>& nanobotLines)
{
    std::vector<Nanobot> nanobots;

    for (const auto& line : nanobotLines)
    {
        std::regex nanobotLineRegex(R"(pos=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>,\s*r=(-?[0-9]+))");
        std::smatch nanobotLineMatch;

        std::regex_match(line, nanobotLineMatch, nanobotLineRegex);

        NumericType posX = std::stoi(nanobotLineMatch[1].str());
        NumericType posY = std::stoi(nanobotLineMatch[2].str());
        NumericType posZ = std::stoi(nanobotLineMatch[3].str());
        NumericType range = std::stoi(nanobotLineMatch[4].str());

        nanobots.emplace_back(Vector3D{posX, posY, posZ}, range);
    }

    return nanobots;

}

unsigned numNanobotsInStrongestRange(const std::vector<std::string>& nanobotLines)
{
    std::vector<Nanobot> nanobots = parseNanobots(nanobotLines);

    const Nanobot strongestNanobot = *std::max_element(nanobots.cbegin(), nanobots.cend(), [](const Nanobot& lhs, const Nanobot& rhs)
                                                       {
                                                           return lhs.range < rhs.range;
                                                       });

    unsigned numInRange = std::count_if(nanobots.cbegin(), nanobots.cend(), [&strongestNanobot](const Nanobot& nb)
                                        {
                                            auto distanceVector = strongestNanobot.position - nb.position;
                                            unsigned manhattanDistance = distanceVector.array().abs().sum();
                                            return manhattanDistance <= strongestNanobot.range;
                                        });

    return numInRange;
}

}
