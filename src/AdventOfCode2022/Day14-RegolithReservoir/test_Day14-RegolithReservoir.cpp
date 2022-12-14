#include "Day14-RegolithReservoir.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2022::Day14;

TEST_CLASS(Day14RegolithReservoir)
{
public:

    TEST_METHOD(numSettledUnitsBeforeSandFlowsIntoAbyss_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "498,4 -> 498,6 -> 496,6",
            "503,4 -> 502,4 -> 502,9 -> 494,9"
        };

        Assert::AreEqual(24u, CurrentDay::numSettledUnitsBeforeSandFlowsIntoAbyss(lines));
    }

};
