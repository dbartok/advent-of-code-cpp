#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

enum class IntcodeProgramExecutionState
{
    RUNNING,
    TERMINATED,
};

struct IntcodeProgamState
{
    std::vector<int> program;
    std::vector<int> inputs;

    std::vector<int> outputs;

    IntcodeProgramExecutionState executionState;
    size_t instructionPointer;
};

}
