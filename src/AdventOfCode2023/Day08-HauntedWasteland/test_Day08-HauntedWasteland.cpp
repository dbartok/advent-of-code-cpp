#include "Day08-HauntedWasteland.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day08;

TEST_CLASS(Day08HauntedWasteland)
{
public:

    TEST_METHOD(numStepsRequiredToReachEndpoint_shortRoute)
    {
        const std::vector<std::string> lines =
        {
            "RL",
            "",
            "AAA = (BBB, CCC)",
            "BBB = (DDD, EEE)",
            "CCC = (ZZZ, GGG)",
            "DDD = (DDD, DDD)",
            "EEE = (EEE, EEE)",
            "GGG = (GGG, GGG)",
            "ZZZ = (ZZZ, ZZZ)"
        };

        Assert::AreEqual(2, CurrentDay::numStepsRequiredToReachEndpoint(lines));
    }

    TEST_METHOD(numStepsRequiredToReachEndpoint_longerRoute)
    {
        const std::vector<std::string> lines =
        {
            "LLR",
            "",
            "AAA = (BBB, BBB)",
            "BBB = (AAA, ZZZ)",
            "ZZZ = (ZZZ, ZZZ)"
        };

        Assert::AreEqual(6, CurrentDay::numStepsRequiredToReachEndpoint(lines));
    }
};
