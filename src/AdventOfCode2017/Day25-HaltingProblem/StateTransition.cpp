#include "StateTransition.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

StateTransition::StateTransition(StateModifier valueFalseModifier, StateModifier valueTrueModifier) noexcept
    : m_valueFalseModifier{std::move(valueFalseModifier)}
    , m_valueTrueModifier{std::move(valueTrueModifier)}
{

}

void StateTransition::apply(TuringMachineState& state) const
{
    if (state.tapeTrueSet.find(state.cursorPos) != state.tapeTrueSet.cend())
    {
        m_valueTrueModifier.modify(state);
    }
    else
    {
        m_valueFalseModifier.modify(state);
    }
}

}
