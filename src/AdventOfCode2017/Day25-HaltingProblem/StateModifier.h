#pragma once

#include "TuringMachineState.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class StateModifier
{
public:
    StateModifier(bool writtenValue, int cursorOffset, StateIDType nextStateID) noexcept;
    void modify(TuringMachineState& state) const;

private:
    bool m_writtenValue;
    int m_cursorOffset;
    StateIDType m_nextStateID;
};

}
