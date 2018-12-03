#include "Day11-HexEd.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day11HexEd)
{
public:

    TEST_METHOD(numStepsFromStart_SimpleTests)
    {
        Assert::AreEqual(3, AoC::numStepsFromStartAtEnd({"ne", "ne", "ne"}));
        Assert::AreEqual(0, AoC::numStepsFromStartAtEnd({"ne", "ne", "sw", "sw"}));
        Assert::AreEqual(2, AoC::numStepsFromStartAtEnd({"ne", "ne", "s", "s"}));
        Assert::AreEqual(3, AoC::numStepsFromStartAtEnd({"se", "sw", "se", "sw", "sw"}));
    }

    TEST_METHOD(maxNumStepsFromStart_MaxDistanceEqualsDistanceAtEnd_ReturnDistanceAtEnd)
    {
        Assert::AreEqual(3, AoC::maxNumStepsFromStart({"ne", "ne", "ne"}));
        Assert::AreEqual(2, AoC::maxNumStepsFromStart({"ne", "ne", "s", "s"}));
        Assert::AreEqual(3, AoC::maxNumStepsFromStart({"se", "sw", "se", "sw", "sw"}));
    }

    TEST_METHOD(maxNumStepsFromStart_MaxDistanceReachedNotAtEnd_ReturnMaxDistance)
    {
        Assert::AreEqual(2, AoC::maxNumStepsFromStart({"ne", "ne", "sw", "sw"}));
        Assert::AreEqual(4, AoC::maxNumStepsFromStart({"n", "s", "se", "sw", "s", "s", "s", "n"}));
    }

};
