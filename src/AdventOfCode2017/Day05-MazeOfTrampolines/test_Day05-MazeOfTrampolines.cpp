#include "Day05-MazeOfTrampolines.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day05MazeOfTrampolines)
{
public:

    TEST_METHOD(jumpInstructionsIncreasing_SimpleTests)
    {
        Assert::AreEqual(5u, AoC::stepsInstructionsIncreasing(m_instructions));
    }

    TEST_METHOD(jumpInstructionsIncreasingDecreasing_SimpleTests)
    {
        Assert::AreEqual(10u, AoC::stepsInstructionsIncreasingDecreasing(m_instructions));
    }

private:
    std::vector<int> m_instructions{0, 3,  0,  1, -3};

};
