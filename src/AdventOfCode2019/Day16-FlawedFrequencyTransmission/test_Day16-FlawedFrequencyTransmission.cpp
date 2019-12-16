#include "Day16-FlawedFrequencyTransmission.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day16FlawedFrequencyTransmission)
{
public:

    TEST_METHOD(firstEightDigitsOfFinalOutput_SimpleTests)
    {
        Assert::AreEqual(std::string{"24176176"}, AoC::firstEightDigitsOfFinalOutput("80871224585914546619083218645595"));
        Assert::AreEqual(std::string{"73745418"}, AoC::firstEightDigitsOfFinalOutput("19617804207202209144916044189917"));
        Assert::AreEqual(std::string{"52432133"}, AoC::firstEightDigitsOfFinalOutput("69317163492948606335995924319873"));
    }

};
