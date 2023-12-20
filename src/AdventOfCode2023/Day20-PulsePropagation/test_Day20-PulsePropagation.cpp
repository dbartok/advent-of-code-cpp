#include "Day20-PulsePropagation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day20;

TEST_CLASS(Day20PulsePropagation)
{
public:

    TEST_METHOD(numLowAndHighPulsesMultipliedTogether_StatelessConfiguration)
    {
        const std::vector<std::string> lines =
        {
            "broadcaster -> a, b, c",
            "%a -> b",
            "%b -> c",
            "%c -> inv",
            "&inv -> a"
        };

        Assert::AreEqual(32000000, CurrentDay::numLowAndHighPulsesMultipliedTogether(lines));
    }

    TEST_METHOD(numLowAndHighPulsesMultipliedTogether_StatefulConfiguration)
    {
        const std::vector<std::string> lines =
        {
            "broadcaster -> a",
            "%a -> inv, con",
            "&inv -> b",
            "%b -> con",
            "&con -> output"
        };

        Assert::AreEqual(11687500, CurrentDay::numLowAndHighPulsesMultipliedTogether(lines));
    }

};
