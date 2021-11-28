#include "DayX-PuzzleName.h" //TODO: Change to the name of the puzzle

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::YearXXXX::DayXX;

//TODO: Change to the name of the puzzle
TEST_CLASS(DayXPuzzleName)
{
public:

    //TODO: Rename and implement unit tests
    TEST_METHOD(solution1_SimpleTests)
    {
        Assert::AreEqual(101, CurrentDay::solution1(100));
        Assert::AreEqual(1002, CurrentDay::solution1(1001));
    }

    TEST_METHOD(solution2_SimpleTests)
    {
        Assert::AreEqual(102, CurrentDay::solution2(100));
        Assert::AreEqual(1003, CurrentDay::solution2(1001));
    }

};
