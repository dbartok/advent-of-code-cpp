#include "Day01-ChronalCalibration.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<int> numbers;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        int number = std::stoi(lineBuffer);
        numbers.push_back(number);
    }

    std::cout << "First part: " << AoC::sumOfAllNumbers(numbers) << std::endl;
    std::cout << "Second part: " << AoC::firstDuplicateSumReached(numbers) << std::endl;
}
