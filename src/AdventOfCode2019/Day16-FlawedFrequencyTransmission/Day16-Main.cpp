#include "Day16-FlawedFrequencyTransmission.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::string signalString
    {
        (std::istreambuf_iterator<char>(fileIn)),
        (std::istreambuf_iterator<char>())
    };

    signalString.erase(std::remove(signalString.begin(), signalString.end(), '\n'), signalString.end());

    std::cout << "First part: " << AoC::firstEightDigitsOfFinalOutput(signalString) << std::endl;
    std::cout << "Second part: " << AoC::messageInFinalOutputForRealSignal(signalString) << std::endl;
}
