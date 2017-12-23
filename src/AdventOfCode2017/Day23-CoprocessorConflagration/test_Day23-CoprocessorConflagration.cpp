#include "Day23-CoprocessorConflagration.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day23CoprocessorConflagration)
{
public:

    TEST_METHOD(numTimesMultInvoked_OneRegisterSimpleLoop_MultIsInvokedTwice)
    {
        std::vector<std::string> instructionStrings =
        {
            "set a 4",
            "add a 2",
            "mul a a",
            "sub a 2",
            "jnz a -1",
            "mul a 6"
        };

        Assert::AreEqual(2u, AoC::numTimesMultInvoked(instructionStrings));
    }

};
