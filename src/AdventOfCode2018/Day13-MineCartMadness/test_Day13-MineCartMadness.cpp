#include "Day13-MineCartMadness.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day13MineCartMadness)
{
public:

    TEST_METHOD(locationOfFirstCollision_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            R"(/->-\        )",
            R"(|   |  /----\)",
            R"(| /-+--+-\  |)",
            R"(| | |  | v  |)",
            R"(\-+-/  \-+--/)",
            R"(  \------/   )"
        };

        Assert::AreEqual(std::string{"7,3"}, AoC::locationOfFirstCollision(lines));
    }

};
