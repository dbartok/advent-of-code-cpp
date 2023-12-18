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

using Vector2D = Eigen::Matrix<int64_t, 2, 1>;
using Matrix2D = Eigen::Matrix<int64_t, 2, 2>;

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

    int64_t getAmountOfLavaHeld() const
    {
        const int64_t area = getArea();
        const int64_t perimeter = getPerimeter();

        // Pick's theorem: A = i + b / 2 - 1
        // Where A is the area, i is the number of interior points, b is the number of boundary points
        // Solving for i:
        // i = A - b / 2 + 1
        const int64_t numInternalPoints = area - perimeter / 2 + 1;

        return numInternalPoints + perimeter;
    }

private:
    std::vector<Instruction> m_instructions;
    std::vector<Vector2D> m_perimeterPoints;

    int64_t getArea() const
    {
        int64_t twiceArea = 0;

        // Apply the Shoelace formula
        for (size_t i = 0; i < m_perimeterPoints.size(); ++i)
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

    int64_t getPerimeter() const
    {
        return std::accumulate(m_instructions.cbegin(), m_instructions.cend(), 0ll, [](int64_t acc, const auto& instruction)
                               {
                                   return acc + instruction.numTiles;
                               });
    }
};

Vector2D parseDirectionChar(const char directionChar)
{
    if (directionChar == 'U' || directionChar == '3')
    {
        return {0, -1};
    }
    else if (directionChar == 'D' || directionChar == '1')
    {
        return {0, 1};
    }
    else if (directionChar == 'L' || directionChar == '2')
    {
        return {-1, 0};
    }
    else if (directionChar == 'R' || directionChar == '0')
    {
        return {1, 0};
    }

    throw std::runtime_error("Invalid direction char: " + std::string{directionChar});
}

Instruction parseDigPlanLine(const std::string& digPlanLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, digPlanLine, boost::is_any_of(" "));

    Vector2D direction = parseDirectionChar(tokens.at(0).front());
    const int numTiles = std::stoi(tokens.at(1));

    return {std::move(direction), numTiles};
}

Instruction parseDigPlanLineWithSwappedInstructions(const std::string& digPlanLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, digPlanLine, boost::is_any_of(" "));

    const std::string hexString = tokens.back().substr(2, 6);
    const char directionChar = hexString.back();
    const std::string numTilesString = hexString.substr(0, hexString.size() - 1);

    Vector2D direction = parseDirectionChar(directionChar);
    const int numTiles = std::stoi(numTilesString, nullptr, 16);

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

std::vector<Instruction> parseDigPlanLinesWithSwappedInstructions(const std::vector<std::string>& digPlanLines)
{
    std::vector<Instruction> instructions;

    for (const auto& digPlanLine : digPlanLines)
    {
        Instruction instruction = parseDigPlanLineWithSwappedInstructions(digPlanLine);
        instructions.push_back(std::move(instruction));
    }

    return instructions;
}

int64_t amountOfLavaHeld(const std::vector<std::string>& digPlanLines)
{
    std::vector<Instruction> instructions = parseDigPlanLines(digPlanLines);

    DigPlanAnalyzer digPlanAnalyzer{std::move(instructions)};

    return digPlanAnalyzer.getAmountOfLavaHeld();
}

int64_t amountOfLavaHeldWithSwappedInstructions(const std::vector<std::string>& digPlanLines)
{
    std::vector<Instruction> instructions = parseDigPlanLinesWithSwappedInstructions(digPlanLines);

    DigPlanAnalyzer digPlanAnalyzer{std::move(instructions)};

    return digPlanAnalyzer.getAmountOfLavaHeld();
}

}
}
}