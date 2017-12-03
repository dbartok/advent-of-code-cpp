#include "CppUnitTest.h"

#include "Day2-CorruptedChecksum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Day2CorruptedChecksum)
{
public:

    TEST_METHOD(checkSumDiffSmallestLargest_SimpleTests)
    {
        RowsOfNumbers rows =
        { 
            {5, 1, 9, 5},
            {7, 5, 3,  },
            {2, 4, 6, 8}
        };

        Assert::AreEqual(18u, checkSumDiffSmallestLargest(rows));
    }

    TEST_METHOD(sumEvenlyDivisbleValues_SimpleTests)
    {
        RowsOfNumbers rows =
        {
            {5, 9, 2, 8},
            {9, 4, 7, 3},
            {3, 8, 6, 5}
        };

        Assert::AreEqual(9u, sumEvenlyDivisbleValues(rows));
    }

};
