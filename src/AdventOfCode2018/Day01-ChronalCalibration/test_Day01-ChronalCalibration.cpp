#include "Day01-ChronalCalibration.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day01ChronalCalibration)
{
public:

    TEST_METHOD(sumOfAllNumbers_SimpleTests)
    {
        Assert::AreEqual(3, AoC::sumOfAllNumbers({+1, +1, +1}));
        Assert::AreEqual(0, AoC::sumOfAllNumbers({+1, +1, -2}));
        Assert::AreEqual(-6, AoC::sumOfAllNumbers({-1, -2, -3}));
    }

    TEST_METHOD(firstDuplicateSumReached_SimpleTests)
    {
        Assert::AreEqual(0, AoC::firstDuplicateSumReached({+1, -1}));
        Assert::AreEqual(10, AoC::firstDuplicateSumReached({+3, +3, +4, -2, -4}));
        Assert::AreEqual(5, AoC::firstDuplicateSumReached({-6, +3, +8, +5, -6}));
        Assert::AreEqual(14, AoC::firstDuplicateSumReached({+7, +7, -2, -7, -4}));
    }

};
