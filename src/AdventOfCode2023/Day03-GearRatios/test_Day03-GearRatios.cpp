#include "Day03-GearRatios.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day03;

TEST_CLASS(Day03GearRatios)
{
public:

    TEST_METHOD(sumOfAllPartNumbers_SimpleTests)
    {
        const std::vector<std::string>& lines =
        {
            "467..114..",
            "...*......",
            "..35..633.",
            "......#...",
            "617*......",
            ".....+.58.",
            "..592.....",
            "......755.",
            "...$.*....",
            ".664.598.."
        };

        Assert::AreEqual(4361, CurrentDay::sumOfAllPartNumbers(lines));
    }

};
