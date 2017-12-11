#include "CppUnitTest.h"

#include "Day05-MazeOfTrampolines.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day05MazeOfTrampolines)
{
private:
    std::vector<int> m_instructions{0, 3,  0,  1, -3};

public:

    TEST_METHOD(jumpInstructionsIncreasing_SimpleTests)
    {
        Assert::AreEqual(5u, AoC::stepsInstructionsIncreasing(m_instructions));
    }

    TEST_METHOD(jumpInstructionsIncreasingDecreasing_SimpleTests)
    {
        Assert::AreEqual(10u, AoC::stepsInstructionsIncreasingDecreasing(m_instructions));
    }

};
