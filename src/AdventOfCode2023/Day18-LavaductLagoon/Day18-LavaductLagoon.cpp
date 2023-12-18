#include "Day18-LavaductLagoon.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <Eigen/dense>

#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day18
{

using Vector2D = Eigen::Vector2i;
using Matrix2D = Eigen::Matrix2i;

struct Instruction
{
    Vector2D direction;
    int numTiles;
};

class DigPlanAnalyzer
{
public:
    DigPlanAnalyzer(std::vector<Instruction> instructions)
        : m_instructions{std::move(instructions)}
    {
        Vector2D currentPerimeterPoint{0, 0};
        for (const auto& instruction : m_instructions)
        {
            m_perimeterPoints.push_back(currentPerimeterPoint);
            currentPerimeterPoint += instruction.numTiles * instruction.direction;
        }
    }

    int getAmountOfLavaHeld() const
    {
        const int area = getArea();
        const int perimeter = getPerimeter();

        // Pick's theorem: A = i + b / 2 - 1
        // Where A is the area, i is the number of interior points, b is the number of boundary points
        // Solving for i:
        // i = A - b / 2 + 1
        const int numInternalPoints = area - perimeter / 2 + 1;

        return numInternalPoints + perimeter;
    }

private:
    std::vector<Instruction> m_instructions;
    std::vector<Vector2D> m_perimeterPoints;

    int getArea() const
    {
        int twiceArea = 0;

        // Apply the Shoelace formula
        for (int i = 0; i < m_perimeterPoints.size(); ++i)
        {
            const Vector2D& currentPerimeterPoint = m_perimeterPoints.at(i);
            const Vector2D& nextPerimeterPoint = m_perimeterPoints.at((i + 1) % m_perimeterPoints.size());

            Matrix2D currentMatrix;
            currentMatrix <<
                currentPerimeterPoint.x(), nextPerimeterPoint.x(),
                currentPerimeterPoint.y(), nextPerimeterPoint.y();

            twiceArea += currentMatrix.determinant();
        }

        return twiceArea / 2;
    }

    int getPerimeter() const
    {
        return std::accumulate(m_instructions.cbegin(), m_instructions.cend(), 0, [](int acc, const auto& instruction)
                               {
                                   return acc + instruction.numTiles;
                               });
    }
};

Vector2D parseDirectionString(const std::string& directionString)
{
    if (directionString == "U")
    {
        return {0, -1};
    }
    else if (directionString == "D")
    {
        return {0, 1};
    }
    else if (directionString == "L")
    {
        return {-1, 0};
    }
    else if (directionString == "R")
    {
        return {1, 0};
    }

    throw std::runtime_error("Invalid direction string: " + directionString);
}

Instruction parseDigPlanLine(const std::string& digPlanLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, digPlanLine, boost::is_any_of(" "));

    Vector2D direction = parseDirectionString(tokens.at(0));
    const int numTiles = std::stoi(tokens.at(1));

    return {std::move(direction), numTiles};
}

std::vector<Instruction> parseDigPlanLines(const std::vector<std::string>& digPlanLines)
{
    std::vector<Instruction> instructions;

    for (const auto& digPlanLine : digPlanLines)
    {
        Instruction instruction = parseDigPlanLine(digPlanLine);
        instructions.push_back(std::move(instruction));
    }

    return instructions;
}

int amountOfLavaHeld(const std::vector<std::string>& digPlanLines)
{
    std::vector<Instruction> instructions = parseDigPlanLines(digPlanLines);

    DigPlanAnalyzer digPlanAnalyzer{std::move(instructions)};

    return digPlanAnalyzer.getAmountOfLavaHeld();
}

}
}
}