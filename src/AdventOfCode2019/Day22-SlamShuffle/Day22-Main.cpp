#include "Day22-SlamShuffle.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string lineBuffer;
    std::vector<std::string> lines;
    while (std::getline(fileIn, lineBuffer))
    {
        lines.push_back(std::move(lineBuffer));
    }

    auto cards = AoC::cardsAfterShuffle(lines, 10007);
    std::cout << "First part: " << (std::find(cards.cbegin(), cards.cend(), 2019) - cards.cbegin()) << std::endl;
}
