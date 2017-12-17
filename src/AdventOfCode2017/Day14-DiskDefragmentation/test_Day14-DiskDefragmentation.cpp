#include "Day14-DiskDefragmentation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day14DiskDefragmentation)
{
public:

    TEST_METHOD(numSquaresUsed_SimpleTests)
    {
        Assert::AreEqual(8108u, AoC::numSquaresUsed(m_keyString));
    }

    TEST_METHOD(numRegionsOfAdjcacentSquares_SimpleTests)
    {
        Assert::AreEqual(1242u, AoC::numRegionsOfAdjcacentSquares(m_keyString));
    }

private:
    const std::string m_keyString = "flqrgnkx";
};
