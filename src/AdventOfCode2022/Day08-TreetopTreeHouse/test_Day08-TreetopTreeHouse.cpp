#include "Day08-TreetopTreeHouse.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2022::Day08;

TEST_CLASS(Day08TreetopTreeHouse)
{
public:

    TEST_METHOD(numTreesVisibleFromOutsideTheGrid_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "30373",
            "25512",
            "65332",
            "33549",
            "35390"
        };

        Assert::AreEqual(21, CurrentDay::numTreesVisibleFromOutsideTheGrid(lines));
    }

};
