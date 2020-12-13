#include "Day13-ShuttleSearch.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day13ShuttleSearch)
{
public:

    TEST_METHOD(earliestBusIDMultipliedByWaitTime_SimpleTests)
    {
        Assert::AreEqual(295, AoC::earliestBusIDMultipliedByWaitTime(939, "7,13,x,x,59,x,31,19"));
    }

};
