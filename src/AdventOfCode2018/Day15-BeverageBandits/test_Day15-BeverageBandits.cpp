#include "Day15-BeverageBandits.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day15BeverageBandits)
{
public:

    TEST_METHOD(outcomeOfCombat_simpleExample)
    {
        std::vector<std::string> mapLines =
        {
            "#######",
            "#.G...#",
            "#...EG#",
            "#.#.#G#",
            "#..G#E#",
            "#.....#",
            "#######"
        };

        Assert::AreEqual(27730u, AoC::outcomeOfCombat(mapLines));
    }

    TEST_METHOD(outcomeOfCombat_elvesWin)
    {
        std::vector<std::string> mapLines =
        {
            "#######",
            "#G..#E#",
            "#E#E.E#",
            "#G.##.#",
            "#...#E#",
            "#...E.#",
            "#######"
        };

        Assert::AreEqual(36334u, AoC::outcomeOfCombat(mapLines));
    }

    TEST_METHOD(outcomeOfCombat_elvesWinLonger)
    {
        std::vector<std::string> mapLines =
        {
            "#######",
            "#E..EG#",
            "#.#G.E#",
            "#E.##E#",
            "#G..#.#",
            "#..E#.#",
            "#######"
        };

        Assert::AreEqual(39514u, AoC::outcomeOfCombat(mapLines));
    }

    TEST_METHOD(outcomeOfCombat_goblinsWin)
    {
        std::vector<std::string> mapLines =
        {
            "#######",
            "#E.G#.#",
            "#.#G..#",
            "#G.#.G#",
            "#G..#.#",
            "#...E.#",
            "#######"
        };

        Assert::AreEqual(27755u, AoC::outcomeOfCombat(mapLines));
    }

    TEST_METHOD(outcomeOfCombat_goblinsWinLonger)
    {
        std::vector<std::string> mapLines =
        {
            "#######",
            "#.E...#",
            "#.#..G#",
            "#.###.#",
            "#E#G#G#",
            "#...#G#",
            "#######"
        };

        Assert::AreEqual(28944u, AoC::outcomeOfCombat(mapLines));
    }

    TEST_METHOD(outcomeOfCombat_largerMap)
    {
        std::vector<std::string> mapLines =
        {
            "#########",
            "#G......#",
            "#.E.#...#",
            "#..##..G#",
            "#...##..#",
            "#...#...#",
            "#.G...G.#",
            "#.....G.#",
            "#########"
        };

        Assert::AreEqual(18740u, AoC::outcomeOfCombat(mapLines));
    }

};
