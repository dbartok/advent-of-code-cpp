#include "Day17-SetAndForget.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day17SetAndForget)
{
public:

    TEST_METHOD(sumOfAlignmentParameters_SimpleTests)
    {
        Assert::AreEqual(1, AoC::sumOfAlignmentParameters(
            {
                104, 46, 104, 35, 104, 46, 104, 10,   // .#.\n
                104, 35, 104, 35, 104, 35, 104, 10,   // ###\n
                104, 46, 104, 35, 104, 46,            // .#.
                99
            }));
    }

    TEST_METHOD(dustCollectedByRobot_SimpleTests)
    {
        AoC::IntcodeNumberType mem = 1000; // Some high memory address
        Assert::AreEqual(1234, AoC::dustCollectedByRobot(
            {
                104, 46, 104, 46, 104, 46, 104, 46, 104, 10,   // ....\n
                104, 94, 104, 35, 104, 46, 104, 35, 104, 10,   // ^#.#\n
                104, 46, 104, 35, 104, 35, 104, 35, 104, 10,   // .###\n
                3, mem,                                        // wait for input
                104, 1234,                                     // output result
                99
            }));
    }

};
