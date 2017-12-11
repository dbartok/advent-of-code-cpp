#include "Day11-HexEd.h"

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

namespace AdventOfCode
{

void applyStep(const std::string& step, std::pair<int, int>& coordinates)
{
    if (step == "n")
    {
        coordinates.second -= 2;
    }
    else if (step == "s")
    {
        coordinates.second += 2;
    }
    else if (step == "nw")
    {
        --coordinates.first;
        --coordinates.second;
    }
    else if (step == "sw")
    {
        --coordinates.first;
        ++coordinates.second;
    }
    else if (step == "ne")
    {
        ++coordinates.first;
        --coordinates.second;
    }
    else if (step == "se")
    {
        ++coordinates.first;
        ++coordinates.second;
    }
    else
    {
        throw std::runtime_error("Unknown step direction in path.");
    }

    assert((coordinates.first + coordinates.second) % 2 == 0);
}

int distanceFromStart(std::pair<int, int> coordinates)
{
    coordinates.first = std::abs(coordinates.first);
    coordinates.second = std::abs(coordinates.second);

    int diagonalDistance = std::min(coordinates.first, coordinates.second);
    coordinates.first -= diagonalDistance;
    coordinates.second -= diagonalDistance;

    int totalDistance = diagonalDistance;
    if (coordinates.first == 0)
    {
        assert(coordinates.second % 2 == 0);
        totalDistance += coordinates.second / 2;
    }
    else if (coordinates.second == 0)
    {
        assert(coordinates.first % 2 == 0);
        totalDistance += coordinates.first;
    }
    else
    {
        assert(false);
    }

    return totalDistance;
}

int numStepsFromStartAtEnd(const std::vector<std::string>& path)
{
    std::pair<int, int> coordinates{0, 0};

    for (const auto& step : path)
    {
        applyStep(step, coordinates);
    }

    return distanceFromStart(coordinates);
}

int maxNumStepsFromStart(const std::vector<std::string>& path)
{
    std::pair<int, int> coordinates{0, 0};
    int maxDistance = std::numeric_limits<int>::min();

    for (const auto& step : path)
    {
        applyStep(step, coordinates);
        maxDistance = std::max(maxDistance, distanceFromStart(coordinates));
    }

    return maxDistance;
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string commaSeparatedPath;
    fileIn >> commaSeparatedPath;

    std::vector<std::string> inputPath;
    boost::split(inputPath, commaSeparatedPath, boost::is_any_of(","));

    std::cout << "First part: " << AoC::numStepsFromStartAtEnd(inputPath) << std::endl;
    std::cout << "Second part: " << AoC::maxNumStepsFromStart(inputPath) << std::endl;
}
