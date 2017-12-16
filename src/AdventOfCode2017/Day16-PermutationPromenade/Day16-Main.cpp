#include "Day16-PermutationPromenade.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <string>

#include <boost/algorithm/string.hpp>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::string commaSeparatedDanceMoves;
    std::getline(fileIn, commaSeparatedDanceMoves);

    std::vector<std::string> inputDanceMovesStrings;
    boost::split(inputDanceMovesStrings, commaSeparatedDanceMoves, boost::is_any_of(","));

    std::cout << "First part: " << AoC::orderAfterDanceMoves(inputDanceMovesStrings) << std::endl;
}
