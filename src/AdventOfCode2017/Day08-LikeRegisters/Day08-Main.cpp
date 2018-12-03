#include "Day08-LikeRegisters.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<AoC::Instruction> inputInsructions;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        AoC::Instruction inputInstruction = AoC::Instruction::fromString(lineBuffer);
        inputInsructions.push_back(std::move(inputInstruction));
    }

    std::cout << "First part: " << AoC::largestRegisterAfterCompletion(inputInsructions) << std::endl;
    std::cout << "Second part: " << AoC::largestRegisterDuringExecution(inputInsructions) << std::endl;
}
