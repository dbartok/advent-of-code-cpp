#include "Day08-LikeRegisters.h"

#include "Processor.h"

#include <fstream>
#include <iostream>
#include <string>

namespace AdventOfCode
{

int largestRegisterAfterCompletion(const std::vector<Instruction>& instructions)
{
    Processor processor{instructions};
    processor.run();
    return processor.largestRegister();
}

int largestRegisterDuringExecution(const std::vector<Instruction>& instructions)
{
    Processor processor{instructions};
    processor.run();
    return processor.globalMaxValueOfRegisters();
}

}


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
