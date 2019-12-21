#include "Day19-TractorBeam.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
size_t AREA_SIZE = 50;
size_t SQUARE_SIZE = 100;
}

namespace AdventOfCode
{

bool isPointAffected(const std::vector<IntcodeNumberType>& intcodeProgram, int x, int y)
{
    IntcodeInterpreter interpreter{intcodeProgram};
    interpreter.addInput(x);
    interpreter.addInput(y);
    interpreter.execute();
    if (interpreter.getOutputs().front() == 1)
    {
        return true;
    }

    return false;
}

int numPointsAffectedByBeam(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    int numPointsAffected = 0;

    for (int j = 0; j < AREA_SIZE; ++j)
    {
        for (int i = 0; i < AREA_SIZE; ++i)
        {
            if (isPointAffected(intcodeProgram, i, j))
            {
                ++numPointsAffected;
            }
        }
    }

    return numPointsAffected;
}

int squareClosestPointChecksum(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    int i = 0;
    int j = 0;
    bool arePositionsCorrect = false;

    while (!arePositionsCorrect)
    {
        arePositionsCorrect = true;

        if (!isPointAffected(intcodeProgram, i + SQUARE_SIZE - 1, j))
        {
            ++j;
            arePositionsCorrect = false;
        }

        if (!isPointAffected(intcodeProgram, i, j + SQUARE_SIZE - 1))
        {
            ++i;
            arePositionsCorrect = false;
        }
    }

    return i * 10000 + j;
}

}
