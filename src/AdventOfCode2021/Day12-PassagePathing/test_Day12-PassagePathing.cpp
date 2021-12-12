#include "Day12-PassagePathing.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2021::Day12;

TEST_CLASS(Day12PassagePathing)
{
public:

    TEST_METHOD(numPathsWithSmallCavesVisitedAtMostOnce_SimpleTests)
    {
        const std::vector<std::string>& lines =
        {
            "start-A",
            "start-b",
            "A-c",
            "A-b",
            "b-d",
            "A-end",
            "b-end"
        };

        Assert::AreEqual(10u, CurrentDay::numPathsWithSmallCavesVisitedAtMostOnce(lines));
    }

    TEST_METHOD(numPathsWithSmallCavesVisitedAtMostOnce_LargerExample)
    {
        const std::vector<std::string>& lines =
        {
            "dc-end",
            "HN-start",
            "start-kj",
            "dc-start",
            "dc-HN",
            "LN-dc",
            "HN-end",
            "kj-sa",
            "kj-HN",
            "kj-dc"
        };

        Assert::AreEqual(19u, CurrentDay::numPathsWithSmallCavesVisitedAtMostOnce(lines));
    }

    TEST_METHOD(numPathsWithSmallCavesVisitedAtMostOnce_EvenLargerExample)
    {
        const std::vector<std::string>& lines =
        {
            "fs-end",
            "he-DX",
            "fs-he",
            "start-DX",
            "pj-DX",
            "end-zg",
            "zg-sl",
            "zg-pj",
            "pj-he",
            "RW-he",
            "fs-DX",
            "pj-RW",
            "zg-RW",
            "start-pj",
            "he-WI",
            "zg-he",
            "pj-fs",
            "start-RW"
        };

        Assert::AreEqual(226u, CurrentDay::numPathsWithSmallCavesVisitedAtMostOnce(lines));
    }

};
