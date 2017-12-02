#include "CppUnitTest.h"

#include "DayX-PuzzleName.h" //TODO: Change to the name of the puzzle

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(DayXPuzzleName) //TODO: Change to the name of the puzzle
{
public:

	//TODO: Rename and implement unit tests
	TEST_METHOD(solution1_SimpleTests)
	{
		Assert::AreEqual(101, solution1(100));
		Assert::AreEqual(1002, solution1(1001));
	}

	TEST_METHOD(solution2_SimpleTests)
	{
		Assert::AreEqual(102, solution2(100));
		Assert::AreEqual(1003, solution2(1001));
	}

};
