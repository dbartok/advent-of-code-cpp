#include "Day11-CosmicExpansion.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day11;

TEST_CLASS(Day11CosmicExpansion)
{
public:

    TEST_METHOD(sumOfShortestPathsBetweenAllPairs_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "...#......",
            ".......#..",
            "#.........",
            "..........",
            "......#...",
            ".#........",
            ".........#",
            "..........",
            ".......#..",
            "#...#....."
        };

        Assert::AreEqual(374, CurrentDay::sumOfShortestPathsBetweenAllPairs(lines));
    }

};
