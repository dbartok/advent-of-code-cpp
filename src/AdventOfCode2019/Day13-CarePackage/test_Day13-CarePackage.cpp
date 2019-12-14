#include "Day13-CarePackage.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day13CarePackage)
{
public:

    TEST_METHOD(numBlockTilesWhenGameExists_SimpleTests)
    {
        Assert::AreEqual(1u, AoC::numBlockTilesWhenGameExits({104, 0, 104, 0, 104, 2, 99}));
    }

    TEST_METHOD(scoreAfterLastBlockBroken_SimpleTests)
    {
        Assert::AreEqual(33u, AoC::scoreAfterLastBlockBroken({0, 0, 0, 0, 104, -1, 104, 0, 104, 33, 99}));
    }

};
