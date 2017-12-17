#include "Day06-MemoryReallocation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    AoC::MemoryBanks inputBanks;

    unsigned inputBank;
    while (fileIn >> inputBank)
    {
        inputBanks.push_back(inputBank);
    }

    std::cout << "First part: " << AoC::numUniqueRedistributions(inputBanks) << std::endl;
    std::cout << "Second part: " << AoC::cyclesUntilReoccurs(inputBanks) << std::endl;
}
