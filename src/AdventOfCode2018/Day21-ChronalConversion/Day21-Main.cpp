#include "Day21-ChronalConversion.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::cout << "First part: " << AoC::lowestValueForFirstRegisterFewestInstructions() << std::endl;
}
