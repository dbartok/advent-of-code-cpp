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

};
