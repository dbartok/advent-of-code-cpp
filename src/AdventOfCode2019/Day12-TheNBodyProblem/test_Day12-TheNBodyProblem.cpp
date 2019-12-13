#include "Day12-TheNBodyProblem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day12TheNBodyProblem)
{
public:

    TEST_METHOD(totalEnergyAfterSteps_SimpleTests)
    {
        const std::vector<std::string> positions1 =
        {
            "<x=-1, y=0, z=2>",
            "<x=2, y=-10, z=-7>",
            "<x=4, y=-8, z=8>",
            "<x=3, y=5, z=-1>"
        };
        Assert::AreEqual(179, AoC::totalEnergyAfterSteps(positions1, 10));

        const std::vector<std::string> positions2 =
        {
            "<x=-8, y=-10, z=0>",
            "<x=5, y=5, z=10>",
            "<x=2, y=-7, z=3>",
            "<x=9, y=-8, z=-3>"
        };
        Assert::AreEqual(1940, AoC::totalEnergyAfterSteps(positions2, 100));
    }

};
