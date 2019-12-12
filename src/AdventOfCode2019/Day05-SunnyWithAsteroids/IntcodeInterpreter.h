#pragma once

#include "IntcodeInstruction.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class ParameterCreator
{
public:
    ParameterCreator(IntcodeNumberType opcodeWithParameterModes, const AdventOfCode::IntcodeProgamState& state);

    IntcodeParameter getParam(size_t parameterPosition);

private:
    IntcodeNumberType m_opcodeWithParameterModes;
    const AdventOfCode::IntcodeProgamState& m_state;

    IntcodeParameterMode createParameterMode(size_t parameterPosition);
};

class IntcodeInterpreter
{
public:
    IntcodeInterpreter(std::vector<IntcodeNumberType> program);

    void execute();

    void addInput(IntcodeNumberType input);
    const std::vector<IntcodeNumberType>& getOutputs() const;
    void clearOutputs();
    const IntcodeProgramExecutionState getExecutionState() const;

private:
    IntcodeProgamState m_state;

    IntcodeInstruction::SharedPtr createNextInstruction();
};

}
