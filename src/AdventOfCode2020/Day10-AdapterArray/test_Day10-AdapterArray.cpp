#include "Day10-AdapterArray.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day10AdapterArray)
{
public:

    TEST_METHOD(numOneAndThreeJoltDifferencesMultiplied_SimpleTests)
    {
        std::vector<int> joltageRatings =
        {
            16,
            10,
            15,
            5,
            1,
            11,
            7,
            19,
            6,
            12,
            4
        };

        Assert::AreEqual(7 * 5, AoC::numOneAndThreeJoltDifferencesMultiplied(joltageRatings));
    }

    TEST_METHOD(numOneAndThreeJoltDifferencesMultiplied_LargerExample)
    {
        std::vector<int> joltageRatings =
        {
            28,
            33,
            18,
            42,
            31,
            14,
            46,
            20,
            48,
            47,
            24,
            23,
            49,
            45,
            19,
            38,
            39,
            11,
            1,
            32,
            25,
            35,
            8,
            17,
            7,
            9,
            4,
            2,
            34,
            10,
            3
        };

        Assert::AreEqual(22 * 10, AoC::numOneAndThreeJoltDifferencesMultiplied(joltageRatings));
    }

};
