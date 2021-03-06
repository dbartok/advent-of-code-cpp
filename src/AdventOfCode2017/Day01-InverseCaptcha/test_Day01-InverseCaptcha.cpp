#include "Day01-InverseCaptcha.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day01InverseCaptcha)
{
public:

    TEST_METHOD(sumOfDigitsMatchNextDigit_SimpleTests)
    {
        Assert::AreEqual(3u, AoC::sumOfDigitsMatchNextDigit("1122"));
        Assert::AreEqual(4u, AoC::sumOfDigitsMatchNextDigit("1111"));
        Assert::AreEqual(0u, AoC::sumOfDigitsMatchNextDigit("1234"));
        Assert::AreEqual(9u, AoC::sumOfDigitsMatchNextDigit("91212129"));
    }

    TEST_METHOD(sumOfDigitsMatchHalfwayRoundDigit_SimpleTests)
    {
        Assert::AreEqual(6u, AoC::sumOfDigitsMatchHalfwayRoundDigit("1212"));
        Assert::AreEqual(0u, AoC::sumOfDigitsMatchHalfwayRoundDigit("1221"));
        Assert::AreEqual(4u, AoC::sumOfDigitsMatchHalfwayRoundDigit("123425"));
        Assert::AreEqual(12u, AoC::sumOfDigitsMatchHalfwayRoundDigit("123123"));
        Assert::AreEqual(4u, AoC::sumOfDigitsMatchHalfwayRoundDigit("12131415"));
    }

};
