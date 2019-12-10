#include "Day10-MonitoringStation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day10MonitoringStation)
{
public:

    TEST_METHOD(maxNumAsteroidsDetected_SimpleTests)
    {
        const std::vector<std::string> map1 =
        {
            ".#..#",
            ".....",
            "#####",
            "....#",
            "...##"
        };
        Assert::AreEqual(8u, AoC::maxNumAsteroidsDetected(map1));

        const std::vector<std::string> map2 =
        {
            "......#.#.",
            "#..#.#....",
            "..#######.",
            ".#.#.###..",
            ".#..#.....",
            "..#....#.#",
            "#..#....#.",
            ".##.#..###",
            "##...#..#.",
            ".#....####"
        };
        Assert::AreEqual(33u, AoC::maxNumAsteroidsDetected(map2));

        const std::vector<std::string> map3 =
        {
            "#.#...#.#.",
            ".###....#.",
            ".#....#...",
            "##.#.#.#.#",
            "....#.#.#.",
            ".##..###.#",
            "..#...##..",
            "..##....##",
            "......#...",
            ".####.###."
        };
        Assert::AreEqual(35u, AoC::maxNumAsteroidsDetected(map3));

        const std::vector<std::string> map4 =
        {
            ".#..#..###",
            "####.###.#",
            "....###.#.",
            "..###.##.#",
            "##.##.#.#.",
            "....###..#",
            "..#.#..#.#",
            "#..#.#.###",
            ".##...##.#",
            ".....#.#.."
        };
        Assert::AreEqual(41u, AoC::maxNumAsteroidsDetected(map4));

        const std::vector<std::string> map5 =
        {
            ".#..##.###...#######",
            "##.############..##.",
            ".#.######.########.#",
            ".###.#######.####.#.",
            "#####.##.#.##.###.##",
            "..#####..#.#########",
            "####################",
            "#.####....###.#.#.##",
            "##.#################",
            "#####.##.###..####..",
            "..######..##.#######",
            "####.##.####...##..#",
            ".#####..#.######.###",
            "##...#.##########...",
            "#.##########.#######",
            ".####.#.###.###.#.##",
            "....##.##.###..#####",
            ".#.#.###########.###",
            "#.#.#.#####.####.###",
            "###.##.####.##.#..##"
        };
        Assert::AreEqual(210u, AoC::maxNumAsteroidsDetected(map5));
    }

};
