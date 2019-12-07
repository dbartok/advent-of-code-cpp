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
    ParameterCreator(int opcodeWithParameterModes, const AdventOfCode::IntcodeProgamState& state);

    IntcodeParameter getParam(size_t parameterPosition);

private:
    int m_opcodeWithParameterModes;
    const AdventOfCode::IntcodeProgamState& m_state;

    IntcodeParameterMode createParameterMode(size_t parameterPosition);
};

class IntcodeInterpreter
{
public:
    IntcodeInterpreter(std::vector<int> program);

    void execute();

    void addInput(int input);
    const std::vector<int>& getOutputs() const;
    const IntcodeProgramExecutionState getExecutionState() const;

private:
    IntcodeProgamState m_state;

    IntcodeInstruction::SharedPtr createNextInstruction();
};

}
