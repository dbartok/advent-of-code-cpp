#include "Day02-ProgramAlarm.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int getValueInPositionZeroAfterProgramHalts(std::vector<int> intcodeProgram)
{
    size_t index = 0;
    while (true)
    {
        const int code = intcodeProgram.at(index);
        if (code == 1 || code == 2)
        {
            const int operandOneIndex = intcodeProgram.at(index + 1);
            const int operandTwoIndex = intcodeProgram.at(index + 2);
            const int resultIndex = intcodeProgram.at(index + 3);

            const int operandOne = intcodeProgram.at(operandOneIndex);
            const int operandTwo = intcodeProgram.at(operandTwoIndex);
            int& result = intcodeProgram.at(resultIndex);
            if (code == 1)
            {
                result = operandOne + operandTwo;
            }
            else if (code == 2)
            {
                result = operandOne * operandTwo;
            }
        }
        else if (code == 99)
        {
            break;
        }

        index += 4;
    }
    return intcodeProgram.front();
}

}
