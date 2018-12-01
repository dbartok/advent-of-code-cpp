#include "Day01-ChronalCalibration.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

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

};
