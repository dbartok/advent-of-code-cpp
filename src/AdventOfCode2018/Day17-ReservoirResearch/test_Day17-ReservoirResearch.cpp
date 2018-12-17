#include "Day17-ReservoirResearch.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day17ReservoirResearch)
{
public:

    TEST_METHOD(numTilesWaterCanReach_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "x=495, y=2..7",
            "y=7, x=495..501",
            "x=501, y=3..7",
            "x=498, y=2..4",
            "x=506, y=1..2",
            "x=498, y=10..13",
            "x=504, y=10..13",
            "y=13, x=498..504"
        };

        Assert::AreEqual(57u, AoC::numTilesWaterCanReach(lines));
    }

};
