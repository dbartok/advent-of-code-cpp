#include "TuringMachineState.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

TuringMachineState::TuringMachineState(StateIDType currentStateID, int cursorPos)
    : currentStateID{currentStateID}
    , cursorPos{cursorPos}
    , tapeTrueSet{}
{

}

}
