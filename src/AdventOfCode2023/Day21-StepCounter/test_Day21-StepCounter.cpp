#include "Day21-StepCounter.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day21;

TEST_CLASS(Day21StepCounter)
{
public:

    TEST_METHOD(numGardenPlotsReachableAfterSteps_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "...........",
            ".....###.#.",
            ".###.##..#.",
            "..#.#...#..",
            "....#.#....",
            ".##..S####.",
            ".##..#...#.",
            ".......##..",
            ".##.#.####.",
            ".##..##.##.",
            "..........."
        };

        Assert::AreEqual(16, CurrentDay::numGardenPlotsReachableAfterSteps(lines, 6));
    }

};
