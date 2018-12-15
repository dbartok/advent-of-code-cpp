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

    // First part
    TEST_METHOD(outcomeOfCombat_simple)
    {
        Assert::AreEqual(27730u, AoC::outcomeOfCombat(m_mapLinesSimple));
    }

    TEST_METHOD(outcomeOfCombat_elvesWin)
    {
        Assert::AreEqual(36334u, AoC::outcomeOfCombat(m_mapLinesElvesWin));
    }

    TEST_METHOD(outcomeOfCombat_elvesWinLonger)
    {
        Assert::AreEqual(39514u, AoC::outcomeOfCombat(m_mapLinesElvesWinLonger));
    }

    TEST_METHOD(outcomeOfCombat_goblinsWin)
    {
        Assert::AreEqual(27755u, AoC::outcomeOfCombat(m_mapLinesGoblinsWin));
    }

    TEST_METHOD(outcomeOfCombat_goblinsWinLonger)
    {
        Assert::AreEqual(28944u, AoC::outcomeOfCombat(m_mapLinesGoblinsWinLonger));
    }

    TEST_METHOD(outcomeOfCombat_largerMap)
    {
        Assert::AreEqual(18740u, AoC::outcomeOfCombat(m_mapLinesLargerMap));
    }

    // Second part
    TEST_METHOD(outcomeOfCombatIfElvesBarelyWin_simple)
    {
        Assert::AreEqual(4988u, AoC::outcomeOfCombatIfElvesBarelyWin(m_mapLinesSimple));
    }

    TEST_METHOD(outcomeOfCombatIfElvesBarelyWin_elvesWinLonger)
    {
        Assert::AreEqual(31284u, AoC::outcomeOfCombatIfElvesBarelyWin(m_mapLinesElvesWinLonger));
    }

    TEST_METHOD(outcomeOfCombatIfElvesBarelyWin_goblinsWin)
    {
        Assert::AreEqual(3478u, AoC::outcomeOfCombatIfElvesBarelyWin(m_mapLinesGoblinsWin));
    }

    TEST_METHOD(outcomeOfCombatIfElvesBarelyWin_goblinsWinLonger)
    {
        Assert::AreEqual(6474u, AoC::outcomeOfCombatIfElvesBarelyWin(m_mapLinesGoblinsWinLonger));
    }

    TEST_METHOD(outcomeOfCombatIfElvesBarelyWin_largerMap)
    {
        Assert::AreEqual(1140u, AoC::outcomeOfCombatIfElvesBarelyWin(m_mapLinesLargerMap));
    }

private:
    std::vector<std::string> m_mapLinesSimple =
    {
        "#######",
        "#.G...#",
        "#...EG#",
        "#.#.#G#",
        "#..G#E#",
        "#.....#",
        "#######"
    };

    std::vector<std::string> m_mapLinesElvesWin =
    {
        "#######",
        "#G..#E#",
        "#E#E.E#",
        "#G.##.#",
        "#...#E#",
        "#...E.#",
        "#######"
    };

    std::vector<std::string> m_mapLinesElvesWinLonger =
    {
        "#######",
        "#E..EG#",
        "#.#G.E#",
        "#E.##E#",
        "#G..#.#",
        "#..E#.#",
        "#######"
    };

    std::vector<std::string> m_mapLinesGoblinsWin =
    {
        "#######",
        "#E.G#.#",
        "#.#G..#",
        "#G.#.G#",
        "#G..#.#",
        "#...E.#",
        "#######"
    };

    std::vector<std::string> m_mapLinesGoblinsWinLonger =
    {
        "#######",
        "#.E...#",
        "#.#..G#",
        "#.###.#",
        "#E#G#G#",
        "#...#G#",
        "#######"
    };

    std::vector<std::string> m_mapLinesLargerMap =
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

};
