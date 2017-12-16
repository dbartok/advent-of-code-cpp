#include "Day16-PermutationPromenade.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day16PermutationPromenade)
{
public:

    TEST_METHOD(orderAfterDanceMoves_SimpleTests)
    {
        const std::vector<std::string> danceMoveStrings =
        {
            "s1",
            "x3/4",
            "pe/b"
        };

        Assert::AreEqual(std::string{"baedc"}, AoC::orderAfterDanceMoves(danceMoveStrings, 5u));
    }

};
