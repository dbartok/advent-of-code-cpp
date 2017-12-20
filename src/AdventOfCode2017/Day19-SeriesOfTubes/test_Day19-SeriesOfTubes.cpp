#include "Day19-SeriesOfTubes.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day19SeriesOfTubes)
{
public:

    TEST_METHOD(lettersVisited_SimpleTests)
    {
        const std::vector<std::string> diagramLines =
        {
            "     |          ",
            "     |  +--+    ",
            "     A  |  C    ",
            " F---|----E|--+ ",
            "     |  |  |  D ",
            "     +B-+  +--+ ",
            "                "
        };

        Assert::AreEqual(std::string{"ABCDEF"}, AoC::lettersVisited(diagramLines));
    }

};
