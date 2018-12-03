#include "Day02-InventoryManagement.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> words;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        words.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << AoC::checksumTwoOrThreeOfAnyLetter(words) << std::endl;
    std::cout << "Second part: " << AoC::commonLettersBetweenOneCharacterDiff(words) << std::endl;
}
