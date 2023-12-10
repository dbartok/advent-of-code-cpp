#include "Day10-PipeMaze.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day10;

TEST_CLASS(Day10PipeMaze)
{
public:

    TEST_METHOD(numStepsAlongLoopToFarthestPosition_SimpleLoop)
    {
        const std::vector<std::string> lines =
        {
            "-L|F7",
            "7S-7|",
            "L|7||",
            "-L-J|",
            "L|-JF"
        };

        Assert::AreEqual(4, CurrentDay::numStepsAlongLoopToFarthestPosition(lines));
    }

    TEST_METHOD(numStepsAlongLoopToFarthestPosition_MoreComplexLoop)
    {
        const std::vector<std::string> lines =
        {
            "7-F7-",
            ".FJ|7",
            "SJLL7",
            "|F--J",
            "LJ.LJ"
        };

        Assert::AreEqual(8, CurrentDay::numStepsAlongLoopToFarthestPosition(lines));
    }

};
