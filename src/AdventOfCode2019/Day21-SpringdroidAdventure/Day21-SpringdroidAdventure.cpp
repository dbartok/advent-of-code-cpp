#include "Day21-SpringdroidAdventure.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <array>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const std::array<std::string, 15> SPRINGSCRIPT_PROGRAM =
{
    "NOT A J", // hole at A
    "NOT B T",
    "OR T J", // or hole at B
    "NOT C T",
    "OR T J", // or hole at C
    "AND D J", // and ground at D
    "WALK"
};

}

namespace AdventOfCode
{

std::string createSpringscriptInput()
{
    std::string springscriptInput;
    for (auto& instruction : SPRINGSCRIPT_PROGRAM)
    {
        if (!instruction.empty())
        {
            springscriptInput += (instruction + '\n');
        }
    }

    return springscriptInput;
}

IntcodeNumberType amountOfHullDamage(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};

    std::string inputString = createSpringscriptInput();

    for (auto c : inputString)
    {
        interpreter.addInput(c);
    }

    interpreter.execute();

    return interpreter.getOutputs().back();
}

}
