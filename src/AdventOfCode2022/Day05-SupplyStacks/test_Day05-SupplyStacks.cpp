#include "Day05-SupplyStacks.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2022::Day05;

TEST_CLASS(Day05SupplyStacks)
{
public:

    TEST_METHOD(cratesOnTopOfEachStack_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "    [D]    ",
            "[N] [C]    ",
            "[Z] [M] [P]",
            " 1   2   3 ",
            "",
            "move 1 from 2 to 1",
            "move 3 from 1 to 3",
            "move 2 from 2 to 1",
            "move 1 from 1 to 2"
        };

        Assert::AreEqual(std::string{"CMZ"}, CurrentDay::cratesOnTopOfEachStack(lines));
    }

};
