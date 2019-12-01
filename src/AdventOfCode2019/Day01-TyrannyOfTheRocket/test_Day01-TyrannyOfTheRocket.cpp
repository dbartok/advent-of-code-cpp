#include "Day01-TyrannyOfTheRocket.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day01TyrannyOfTheRocket)
{
public:

    TEST_METHOD(getSumOfFuelRequirements_SimpleTests)
    {
        Assert::AreEqual(34241, AoC::getSumOfFuelRequirements({12, 14, 1969, 100756}));
    }

};
