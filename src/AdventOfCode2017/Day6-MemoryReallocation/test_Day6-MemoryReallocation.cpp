#include "CppUnitTest.h"

#include "Day6-MemoryReallocation.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day6MemoryReallocation)
{
public:

    TEST_METHOD(numUniqueRedistributions_SimpleTests)
    {
        AoC::MemoryBanks banks{0, 2, 7, 0};
        Assert::AreEqual(5u, AoC::numUniqueRedistributions(banks));
    }

};
