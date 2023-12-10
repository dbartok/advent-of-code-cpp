#include "Day09-MirageMaintenance.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day09;

TEST_CLASS(Day09MirageMaintenance)
{
public:

    TEST_METHOD(sumOfExtrapolatedValues_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "0 3 6 9 12 15",
            "1 3 6 10 15 21",
            "10 13 16 21 30 45"
        };

        Assert::AreEqual(114, CurrentDay::sumOfExtrapolatedValues(lines ));
    }

};
