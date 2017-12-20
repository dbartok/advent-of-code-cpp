#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
#include <array>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class PipeDiagramTraverser
{
public:
    PipeDiagramTraverser(std::vector<std::string> pipeDiagramLines) noexcept;
    void traverse();
    std::string getLettersVisitedSequence() const;

    static bool isPath(char currentChar) noexcept;
private:
    using Coordinates = std::pair<int, int>;

    enum class Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    static const std::array<Direction, 4> ALL_DIRECTIONS;

    std::vector<std::string> m_pipeDiagramLines;

    Coordinates m_currentCoords;
    Direction m_currentDirection;
    std::string m_lettersVisitedSequence;

    void resetToStartingCoords();
    bool switchDirection();
    void stepInCurrentDirection();
    bool areCoordsInBounds(Coordinates coords) const;

    bool isDiagramRectangle() const;

    Coordinates neighborCoords(Direction direction);

    static bool areOppositeDirections(Direction d1, Direction d2);
};

}
