#include "Day09-EncodingError.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day09EncodingError)
{
public:

    TEST_METHOD(firstInvalidNumber_SimpleTests)
    {
        std::vector<int64_t> numbers =
        {
            20,
            35,
            15,
            25,
            47,
            40,
            62,
            55,
            65,
            95,
            102,
            117,
            150,
            182,
            127,
            219,
            299,
            277,
            309,
            576
        };

        Assert::AreEqual(127ll, AoC::firstInvalidNumber(numbers, 5));
    }

};
