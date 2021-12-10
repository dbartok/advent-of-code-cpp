#include "Day09-SmokeBasin.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2021::Day09;

TEST_CLASS(Day09SmokeBasin)
{
public:

    TEST_METHOD(sumOfRiskLevelsOfAllLowPoints_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "2199943210",
            "3987894921",
            "9856789892",
            "8767896789",
            "9899965678"
        };

        Assert::AreEqual(15, CurrentDay::sumOfRiskLevelsOfAllLowPoints(lines));
    }

};
