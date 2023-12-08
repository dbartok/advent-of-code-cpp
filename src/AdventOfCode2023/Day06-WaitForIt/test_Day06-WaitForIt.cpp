#include "Day06-WaitForIt.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day06;

TEST_CLASS(Day06WaitForIt)
{
public:

    TEST_METHOD(numWaysRecordCanBeBeatenMultipliedTogether_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "Time:      7  15   30",
            "Distance:  9  40  200"
        };

        Assert::AreEqual(288, CurrentDay::numWaysRecordCanBeBeatenMultipliedTogether(lines));
    }

};
