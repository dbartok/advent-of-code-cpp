#include "CppUnitTest.h"

#include "Day10-KnotHash.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day10KnotHash)
{
public:

    TEST_METHOD(firstTwoMultipliedAfterKnotting_SimpleTests)
    {
        std::vector<unsigned> lengths = {3, 4, 1, 5};
        Assert::AreEqual(12u, AoC::firstTwoMultipliedAfterKnotting(5, lengths));
    }

    TEST_METHOD(firstTwoMultipliedAfterKnotting_0Length_NoElementsAreMoved)
    {
        std::vector<unsigned> lengths = {2, 0, 3};
        Assert::AreEqual(4u, AoC::firstTwoMultipliedAfterKnotting(5, lengths));
    }

};
