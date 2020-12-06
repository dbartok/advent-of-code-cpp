#include "Day06-CustomCustoms.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day06CustomCustoms)
{
public:

    TEST_METHOD(sumAnyoneAnsweredYes_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "abc",
            "",
            "a",
            "b",
            "c",
            "",
            "ab",
            "ac",
            "",
            "a",
            "a",
            "a",
            "a",
            "",
            "b"
        };

        Assert::AreEqual(11, AoC::sumAnyoneAnsweredYes(lines));
    }

};
