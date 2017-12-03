#pragma once

#include "InfiniteGrid.h"
#include "Head.h"

class StressTestWriter
{
private:
    InfiniteGrid<unsigned> m_grid;
    unsigned m_lastWritten;
    
    Head m_head;
    unsigned m_totalStepsUntilTurn;
    unsigned m_remainingStepsUntilTurn; 

    void executeNextTurn();
public:
    StressTestWriter();
    void writeUntil(unsigned threshold);
    unsigned getLastWritten() const;
};
