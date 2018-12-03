#include "Day03-SpiralMemory.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day03SpiralMemory)
{
public:

    TEST_METHOD(stepsToCarryBack_SimpleTests)
    {
        Assert::AreEqual(0u, AoC::stepsToCarryBack(1u));
        Assert::AreEqual(3u, AoC::stepsToCarryBack(12u));
        Assert::AreEqual(2u, AoC::stepsToCarryBack(23u));
        Assert::AreEqual(31u, AoC::stepsToCarryBack(1024u));
    }

    TEST_METHOD(stepsToCarryBack_LocationOnCorner)
    {
        Assert::AreEqual(2u, AoC::stepsToCarryBack(9u));
        Assert::AreEqual(4u, AoC::stepsToCarryBack(25u));
    }

    TEST_METHOD(stressTestFirstValueGreaterThan_SimpleTests)
    {
        Assert::AreEqual(4u, AoC::stressTestFirstValueGreaterThan(2u));
        Assert::AreEqual(26u, AoC::stressTestFirstValueGreaterThan(25u));
        Assert::AreEqual(304u, AoC::stressTestFirstValueGreaterThan(147u));
        Assert::AreEqual(806u, AoC::stressTestFirstValueGreaterThan(747u));
    }
};
