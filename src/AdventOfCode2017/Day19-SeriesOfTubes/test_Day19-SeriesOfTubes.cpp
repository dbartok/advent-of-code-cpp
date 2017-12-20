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
        Assert::AreEqual(std::string{"ABCDEF"}, AoC::lettersVisited(m_diagramLines));
    }

    TEST_METHOD(totalNumStepsForPacket_SimpleTests)
    {
        Assert::AreEqual(38u, AoC::totalNumStepsForPacket(m_diagramLines));
    }

private:
    const std::vector<std::string> m_diagramLines =
    {
        "     |          ",
        "     |  +--+    ",
        "     A  |  C    ",
        " F---|----E|--+ ",
        "     |  |  |  D ",
        "     +B-+  +--+ ",
        "                "
    };
};
