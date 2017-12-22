#include "Day22-SporificaVirus.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> initialVirusPosLines;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        initialVirusPosLines.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << AoC::basicVirusInfectingBursts(initialVirusPosLines) << std::endl;
    std::cout << "Second part: " << AoC::evolvedVirusInfectingBursts(initialVirusPosLines) << std::endl;
}
