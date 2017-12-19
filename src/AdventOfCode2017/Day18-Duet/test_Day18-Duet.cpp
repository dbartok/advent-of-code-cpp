#include "Day18-Duet.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day18Duet)
{
public:

    TEST_METHOD(valueOfFirstRecoveredFrequency_SimpleTests)
    {
        std::vector<std::string> instructionStrings =
        {
            "set a 1",
            "add a 2",
            "mul a a",
            "mod a 5",
            "snd a",
            "set a 0",
            "rcv a",
            "jgz a -1",
            "set a 1",
            "jgz a -2"
        };

        Assert::AreEqual(4, AoC::valueOfFirstRecoveredFrequency(instructionStrings));
    }

};
