#include "Day17-ConwayCubes.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day17ConwayCubes)
{
public:

    TEST_METHOD(numCubesInActiveStateAfterBootCycle_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            ".#.",
            "..#",
            "###"
        };

        Assert::AreEqual(112, AoC::numCubesInActiveStateAfterBootCycle(lines));
    }

};
