#include "CppUnitTest.h"

#include "Day5-MazeOfTrampolines.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day5MazeOfTrampolines)
{
public:

    TEST_METHOD(jumpInstructionsIncreasing_SimpleTests)
    {
        std::vector<int> instructions{0, 3,  0,  1, -3};
        Assert::AreEqual(5u, AoC::jumpInstructionsIncreasing(instructions));
    }

};
