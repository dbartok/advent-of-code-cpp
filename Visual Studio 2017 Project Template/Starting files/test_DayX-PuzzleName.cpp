#include "DayX-PuzzleName.h" //TODO: Change to the name of the puzzle

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(DayXPuzzleName) //TODO: Change to the name of the puzzle
{
public:

    //TODO: Rename and implement unit tests
    TEST_METHOD(solution1_SimpleTests)
    {
        Assert::AreEqual(101, AoC::solution1(100));
        Assert::AreEqual(1002, AoC::solution1(1001));
    }

    TEST_METHOD(solution2_SimpleTests)
    {
        Assert::AreEqual(102, AoC::solution2(100));
        Assert::AreEqual(1003, AoC::solution2(1001));
    }

};
