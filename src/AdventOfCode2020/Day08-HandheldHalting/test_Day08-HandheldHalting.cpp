#include "Day08-HandheldHalting.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day08HandheldHalting)
{
public:

    TEST_METHOD(valueInAccAfterFirstDuplicateInstruction_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "nop +0",
            "acc +1",
            "jmp +4",
            "acc +3",
            "jmp -3",
            "acc -99",
            "acc +1",
            "jmp -4",
            "acc +6"
        };

        Assert::AreEqual(5, AoC::valueInAccAfterFirstDuplicateInstruction(lines));
    }

};
