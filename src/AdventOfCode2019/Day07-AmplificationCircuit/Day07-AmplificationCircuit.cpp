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
size_t FEEDBACK_LOOP_FIRST_PHASE = 5;

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
            IntcodeInterpreter amplifier{intcodeProgram};
            amplifier.addInput(phaseSettings.at(i));
            amplifier.addInput(previousOutput);
            amplifier.execute();

            previousOutput = amplifier.getOutputs().back();
        }

        maxOutput = std::max(maxOutput, previousOutput);

        if (!std::next_permutation(phaseSettings.begin(), phaseSettings.end()))
        {
            break;
        }
    }

    return maxOutput;
}

int highestPossibleSignalSentWithAmplification(const std::vector<int>& intcodeProgram)
{
    std::vector<int> phaseSettings(NUM_AMPLIFIERS);
    std::iota(phaseSettings.begin(), phaseSettings.end(), FEEDBACK_LOOP_FIRST_PHASE);

    int maxOutput = std::numeric_limits<int>::min();

    while (true)
    {
        std::vector<IntcodeInterpreter> amplifiers;
        for (size_t i = 0; i < NUM_AMPLIFIERS; ++i)
        {
            IntcodeInterpreter amplifier{intcodeProgram};
            amplifier.addInput(phaseSettings.at(i));

            amplifiers.push_back(std::move(amplifier));
        }

        int previousOutput = 0;

        while (true)
        {
            for (size_t i = 0; i < NUM_AMPLIFIERS; ++i)
            {
                amplifiers.at(i).addInput(previousOutput);
                amplifiers.at(i).execute();
                previousOutput = amplifiers.at(i).getOutputs().back();
            }

            maxOutput = std::max(maxOutput, previousOutput);

            if (amplifiers.at(0).getExecutionState() == IntcodeProgramExecutionState::TERMINATED)
            {
                break;
            }
        }

        if (!std::next_permutation(phaseSettings.begin(), phaseSettings.end()))
        {
            break;
        }
    }

    return maxOutput;
}

}
