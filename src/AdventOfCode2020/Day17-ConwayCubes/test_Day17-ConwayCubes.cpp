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

    TEST_METHOD(numCubesInActiveStateAfterBootCycle3D_SimpleTests)
    {
        Assert::AreEqual(112, AoC::numCubesInActiveStateAfterBootCycle3D(m_lines));
    }

    TEST_METHOD(numCubesInActiveStateAfterBootCycle4D_SimpleTests)
    {
        Assert::AreEqual(848, AoC::numCubesInActiveStateAfterBootCycle4D(m_lines));
    }

private:
    std::vector<std::string> m_lines =
    {
        ".#.",
        "..#",
        "###"
    };

};
