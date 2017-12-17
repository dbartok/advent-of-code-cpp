#pragma once

#include "InfiniteGrid.h"
#include "Head.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class StressTestWriter
{
public:
    StressTestWriter();
    void writeUntil(unsigned threshold);
    unsigned getLastWritten() const;

private:
    InfiniteGrid<unsigned> m_grid;
    unsigned m_lastWritten;

    Head m_head;
    unsigned m_totalStepsUntilTurn;
    unsigned m_remainingStepsUntilTurn;

    void executeNextTurn();
};

}
