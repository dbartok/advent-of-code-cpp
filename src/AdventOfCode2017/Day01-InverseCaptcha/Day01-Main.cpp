#include "Day01-InverseCaptcha.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::string digitsString;

    fileIn >> digitsString;

    std::cout << "First part: " << AoC::sumOfDigitsMatchNextDigit(digitsString) << std::endl;
    std::cout << "Second part: " << AoC::sumOfDigitsMatchHalfwayRoundDigit(digitsString) << std::endl;
}
