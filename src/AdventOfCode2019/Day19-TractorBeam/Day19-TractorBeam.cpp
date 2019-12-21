#include "Day19-TractorBeam.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
size_t AREA_SIZE = 50;
}

namespace AdventOfCode
{

int numPointsAffectedByBeam(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    int numPointsAffected = 0;

    for (int j = 0; j < AREA_SIZE; ++j)
    {
        for (int i = 0; i < AREA_SIZE; ++i)
        {
            IntcodeInterpreter interpreter{intcodeProgram};
            interpreter.addInput(i);
            interpreter.addInput(j);
            interpreter.execute();
            if (interpreter.getOutputs().front())
            {
                ++numPointsAffected;
            }
        }
    }

    return numPointsAffected;
}

}
