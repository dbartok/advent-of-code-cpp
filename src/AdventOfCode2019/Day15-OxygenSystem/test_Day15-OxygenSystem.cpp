#include "Day15-OxygenSystem.h" //TODO: Change to the name of the puzzle

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day15OxygenSystem)
{
public:

    TEST_METHOD(fewestNumberOfMovementsToOxygen_SimpleTests)
    {
        AoC::IntcodeNumberType mem = 1000; // Some high memory address
        Assert::AreEqual(1, AoC::fewestNumberOfMovementsToOxygen(
            {
                3, mem, 104, 2, 99
            }
        ));
    }

};
