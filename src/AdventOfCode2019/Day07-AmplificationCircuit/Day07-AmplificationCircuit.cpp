#include "Day07-AmplificationCircuit.h"

#include <AdventOfCode2019/Day05-SunnyWithAsteroids/IntcodeInterpreter.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

size_t NUM_AMPLIFIERS = 5;

int highestPossibleSignalSent(const std::vector<int>& intcodeProgram)
{
    std::vector<int> phaseSettings(NUM_AMPLIFIERS);
    std::iota(phaseSettings.begin(), phaseSettings.end(), 0);

    int maxOutput = std::numeric_limits<int>::min();

    while (true)
    {
        int previousOutput = 0;

        for (size_t i = 0; i < NUM_AMPLIFIERS; ++i)
        {
            IntcodeInterpreter ie{intcodeProgram, {phaseSettings.at(i), previousOutput}};
            ie.execute();

            previousOutput = ie.getOutputs().back();
        }

        maxOutput = std::max(maxOutput, previousOutput);

        if (!std::next_permutation(phaseSettings.begin(), phaseSettings.end()))
        {
            break;
        }
    }

    return maxOutput;
}

}
