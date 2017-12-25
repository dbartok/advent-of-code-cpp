#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <unordered_set>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using StateIDType = char;

struct TuringMachineState
{
    StateIDType currentStateID;
    int cursorPos;
    std::unordered_set<int> tapeTrueSet;

    TuringMachineState(StateIDType currentStateID, int cursorPos);
};

}
