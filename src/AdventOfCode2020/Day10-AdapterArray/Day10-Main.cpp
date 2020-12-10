#include "Day10-AdapterArray.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<int> joltageRatings;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        const int entry = std::stoi(lineBuffer);
        joltageRatings.push_back(entry);
    }

    std::cout << "First part: " << AoC::numOneAndThreeJoltDifferencesMultiplied(joltageRatings) << std::endl;
}
