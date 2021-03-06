#include "Day05-SunnyWithAsteroids.h"

#include "IntcodeInterpreter.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <memory>
#include <stdexcept>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int diagnosticCodeProducedByProgram(const std::vector<int>& intcodeProgram, int input)
{
    IntcodeInterpreter ie{intcodeProgram};
    ie.addInput(input);

    ie.execute();

    return ie.getOutputs().back();
}

}
