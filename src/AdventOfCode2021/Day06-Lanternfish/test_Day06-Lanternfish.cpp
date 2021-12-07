#include "Day06-Lanternfish.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2021::Day06;

TEST_CLASS(Day06Lanternfish)
{
public:

    TEST_METHOD(numLanternfishAfterMultipleDays_SimpleTests)
    {
        Assert::AreEqual(5934u, CurrentDay::numLanternfishAfterMultipleDays({3, 4, 3, 1, 2}));
    }

};
