#include "Day11-HexEd.h"

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

namespace AdventOfCode
{

int numStepsFromStart(const std::vector<std::string>& path)
{
    std::pair<int, int> coordinates{0, 0};
    for (const auto& step : path)
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

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string commaSeparatedPath;
    fileIn >> commaSeparatedPath;

    std::vector<std::string> inputPath;
    boost::split(inputPath, commaSeparatedPath, boost::is_any_of(","));

    std::cout << "First part: " << AoC::numStepsFromStart(inputPath) << std::endl;
}
