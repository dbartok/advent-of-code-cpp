#include "Day16-TheFloorWillBeLava.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2023::Day16;

TEST_CLASS(Day16TheFloorWillBeLava)
{
public:

    TEST_METHOD(numEnergizedTiles_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            ".|...\\....",
            "|.-.\\.....",
            ".....|-...",
            "........|.",
            "..........",
            ".........\\",
            "..../.\\\\..",
            ".-.-/..|..",
            ".|....-|.\\",
            "..//.|...."
        };

        Assert::AreEqual(46, CurrentDay::numEnergizedTiles(lines));
    }

};
