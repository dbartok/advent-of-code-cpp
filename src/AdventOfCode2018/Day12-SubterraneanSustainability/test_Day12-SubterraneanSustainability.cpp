#include "Day12-SubterraneanSustainability.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day12SubterraneanSustainability)
{
public:

    TEST_METHOD(sumOfPotNumbersWithPlantAfterGrowth_SimpleTests)
    {
        std::string initialStateLine = "initial state: #..#.#..##......###...###";
        std::vector<std::string> neighborPatternLines =
        {
            "...## => #",
            "..#.. => #",
            ".#... => #",
            ".#.#. => #",
            ".#.## => #",
            ".##.. => #",
            ".#### => #",
            "#.#.# => #",
            "#.### => #",
            "##.#. => #",
            "##.## => #",
            "###.. => #",
            "###.# => #",
            "####. => #"
        };

        Assert::AreEqual(325, AoC::sumOfPotNumbersWithPlantAfterGrowth(initialStateLine, neighborPatternLines));
    }

};
