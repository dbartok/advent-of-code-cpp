#include "Day09-MarbleMania.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day09MarbleMania)
{
public:

    TEST_METHOD(winningElfsScore_SimpleTests)
    {
        Assert::AreEqual(32u, AoC::winningElfsScore("9 players; last marble is worth 25 points"));
        Assert::AreEqual(8317u, AoC::winningElfsScore("10 players; last marble is worth 1618 points"));
        Assert::AreEqual(146373u, AoC::winningElfsScore("13 players; last marble is worth 7999 points"));
        Assert::AreEqual(2764u, AoC::winningElfsScore("17 players; last marble is worth 1104 points"));
        Assert::AreEqual(54718u, AoC::winningElfsScore("21 players; last marble is worth 6111 points"));
        Assert::AreEqual(37305u, AoC::winningElfsScore("30 players; last marble is worth 5807 points"));
    }

    TEST_METHOD(winningElfsScoreLongerGame_SimpleTests)
    {
        Assert::AreEqual(22563u, AoC::winningElfsScoreLongerGame("9 players; last marble is worth 25 points"));
    }

};
