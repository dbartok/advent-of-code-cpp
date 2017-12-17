#include "Day15-DuelingGenerators.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string generatorAStartLine;
    std::getline(fileIn, generatorAStartLine);
    const unsigned generatorAStartInput = AoC::generatorStartFromLine(generatorAStartLine);

    std::string generatorBStartLine;
    std::getline(fileIn, generatorBStartLine);
    const unsigned generatorBStartInput = AoC::generatorStartFromLine(generatorBStartLine);

    std::cout << "First part: " << AoC::judgeFinalCountNoCriteria(generatorAStartInput, generatorBStartInput) << std::endl;
    std::cout << "Second part: " << AoC::judgeFinalCountWithCriteria(generatorAStartInput, generatorBStartInput) << std::endl;
}
