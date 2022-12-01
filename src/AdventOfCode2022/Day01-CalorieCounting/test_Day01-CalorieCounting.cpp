#include "Day01-CalorieCounting.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2022::Day01;

TEST_CLASS(Day01CalorieCounting)
{
public:

    TEST_METHOD(maxTotalCaloriesCarried_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "1000",
            "2000",
            "3000",
            "",
            "4000",
            "",
            "5000",
            "6000",
            "",
            "7000",
            "8000",
            "9000",
            "",
            "10000"
        };

        Assert::AreEqual(24000, CurrentDay::maxTotalCaloriesCarried(lines));
    }

};
