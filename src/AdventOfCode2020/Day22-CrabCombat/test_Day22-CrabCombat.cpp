#include "Day22-CrabCombat.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day22CrabCombat)
{
public:

    TEST_METHOD(winningPlayerScore_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "Player 1:",
            "9",
            "2",
            "6",
            "3",
            "1",
            "",
            "Player 2:",
            "5",
            "8",
            "4",
            "7",
            "10"
        };

        Assert::AreEqual(306, AoC::winningPlayerScore(lines));
    }

};
