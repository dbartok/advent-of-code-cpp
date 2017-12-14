#include "Day14-DiskDefragmentation.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

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
        Assert::AreEqual(8108u, AoC::numSquaresUsed("flqrgnkx"));
    }

};
