#include "Day12-RainRisk.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day12RainRisk)
{
public:

    TEST_METHOD(manhattanDistanceToEndLocation_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "F10",
            "N3",
            "F7",
            "R90",
            "F11"
        };

        Assert::AreEqual(25, AoC::manhattanDistanceToEndLocation(lines));
    }

};
