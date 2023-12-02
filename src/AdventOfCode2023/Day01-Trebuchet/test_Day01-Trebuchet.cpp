#include "Day01-Trebuchet.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day01;

TEST_CLASS(Day01Trebuchet)
{
public:

    TEST_METHOD(sumOfAllCalibrationValues_SimpleTests)
    {
        const std::vector<std::string>& lines =
        {
            "1abc2",
            "pqr3stu8vwx",
            "a1b2c3d4e5f",
            "treb7uchet"
        };

        Assert::AreEqual(142, CurrentDay::sumOfAllCalibrationValues(lines));
    }

};
