#include "Day11-HexEd.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string commaSeparatedPath;
    fileIn >> commaSeparatedPath;

    std::vector<std::string> inputPath;
    boost::split(inputPath, commaSeparatedPath, boost::is_any_of(","));

    std::cout << "First part: " << AoC::numStepsFromStartAtEnd(inputPath) << std::endl;
    std::cout << "Second part: " << AoC::maxNumStepsFromStart(inputPath) << std::endl;
}
