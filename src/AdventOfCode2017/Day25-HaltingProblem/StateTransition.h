#pragma once

#include "TuringMachineState.h"
#include "StateModifier.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class StateTransition
{
public:
    StateTransition(StateModifier valueFalseModifier, StateModifier valueTrueModifier) noexcept;
    void apply(TuringMachineState& state) const;

private:
    StateModifier m_valueFalseModifier;
    StateModifier m_valueTrueModifier;
};

}
