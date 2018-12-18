#include "Day18-SettlersNorthPole.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day18SettlersNorthPole)
{
public:

    TEST_METHOD(totalResourceValueAfterShortTime_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            ".#.#...|#.",
            ".....#|##|",
            ".|..|...#.",
            "..|#.....#",
            "#.#|||#|#|",
            "...#.||...",
            ".|....|...",
            "||...#|.#|",
            "|.||||..|.",
            "...#.|..|."
        };

        Assert::AreEqual(1147u, AoC::totalResourceValueAfterShortTime(lines));
    }

    TEST_METHOD(totalResourceValueAfterLongTime_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "#|.",
            "|#|",
            ".|#"
        };

        Assert::AreEqual(12u, AoC::totalResourceValueAfterLongTime(lines));
    }

};
