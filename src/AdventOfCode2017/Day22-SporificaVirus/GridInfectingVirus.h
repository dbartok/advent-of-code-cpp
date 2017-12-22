#pragma once

#include <AdventOfCode2017/Day03-SpiralMemory/InfiniteGrid.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class GridInfectingVirus
{
public:
    GridInfectingVirus(InfiniteGrid<bool> initialGrid);
    void infectRepeatedly(unsigned numIterations);
    unsigned getTotalInfectionBursts() const noexcept;

private:
    InfiniteGrid<bool> m_grid;

    enum class Direction
    {
        LEFT = 0,
        UP = 1,
        RIGHT = 2,
        DOWN = 3,
        _MODULUS = 4
    };

    Direction m_direction;
    std::pair<int, int> m_coords;

    unsigned m_totalInfectionBursts;

    void turnLeft() noexcept;
    void turnRight() noexcept;
    void stepForward() noexcept;
};

}
