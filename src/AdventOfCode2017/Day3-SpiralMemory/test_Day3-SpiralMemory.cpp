#include "CppUnitTest.h"

#include "Day3-SpiralMemory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Day3SpiralMemory)
{
public:

    TEST_METHOD(stepsToCarryBack_SimpleTests)
    {
        Assert::AreEqual(0u, stepsToCarryBack(1u));
        Assert::AreEqual(3u, stepsToCarryBack(12u));
        Assert::AreEqual(2u, stepsToCarryBack(23u));
        Assert::AreEqual(31u, stepsToCarryBack(1024u));
    }

    TEST_METHOD(stepsToCarryBack_LocationOnCorner)
    {
        Assert::AreEqual(2u, stepsToCarryBack(9u));
        Assert::AreEqual(4u, stepsToCarryBack(25u));
    }

    TEST_METHOD(stressTestFirstValueGreaterThan_SimpleTests)
    {
        Assert::AreEqual(4u, stressTestFirstValueGreaterThan(2u));
        Assert::AreEqual(26u, stressTestFirstValueGreaterThan(25u));
        Assert::AreEqual(304u, stressTestFirstValueGreaterThan(147u));
        Assert::AreEqual(806u, stressTestFirstValueGreaterThan(747u));
    }
};
