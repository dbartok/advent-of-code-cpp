#include "Day18-Duet.h"

#include "AssemblyProgramParser.h"
#include "AssemblyProgram.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int valueOfFirstRecoveredFrequency(const std::vector<std::string>& instructionStrings)
{
    AssemblyProgramParser parser{instructionStrings};
    AssemblyProgram program = parser.createProgram();

    program.runUntilTerminated();

    return program.lastRecoveredFrequency();
}

}
