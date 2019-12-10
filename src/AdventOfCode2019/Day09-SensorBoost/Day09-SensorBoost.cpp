#include "Day09-SensorBoost.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::vector<IntcodeNumberType> boostKeycodeProduced(const std::vector<IntcodeNumberType>& intcodeProgram, int input)
{
    IntcodeInterpreter ie{intcodeProgram};
    ie.addInput(input);

    ie.execute();

    return ie.getOutputs();
}

}
