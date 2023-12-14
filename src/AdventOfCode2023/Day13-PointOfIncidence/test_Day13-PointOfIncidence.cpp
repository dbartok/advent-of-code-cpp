#include "Day13-PointOfIncidence.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day13;

TEST_CLASS(Day13PointOfIncidence)
{
public:

    TEST_METHOD(numberAfterSummarizingAllNotes_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "#.##..##.",
            "..#.##.#.",
            "##......#",
            "##......#",
            "..#.##.#.",
            "..##..##.",
            "#.#.##.#.",
            "",
            "#...##..#",
            "#....#..#",
            "..##..###",
            "#####.##.",
            "#####.##.",
            "..##..###",
            "#....#..#"
        };

        Assert::AreEqual(405, CurrentDay::numberAfterSummarizingAllNotes(lines));
    }

};
