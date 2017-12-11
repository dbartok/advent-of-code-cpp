#include "CppUnitTest.h"

#include "Day11-HexEd.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day11HexEd)
{
public:

    TEST_METHOD(numStepsFromStart_SimpleTests)
    {
        Assert::AreEqual(3, AoC::numStepsFromStart({"ne", "ne", "ne"}));
        Assert::AreEqual(0, AoC::numStepsFromStart({"ne", "ne", "sw", "sw"}));
        Assert::AreEqual(2, AoC::numStepsFromStart({"ne", "ne", "s", "s"}));
        Assert::AreEqual(3, AoC::numStepsFromStart({"se", "sw", "se", "sw", "sw"}));
    }

};
