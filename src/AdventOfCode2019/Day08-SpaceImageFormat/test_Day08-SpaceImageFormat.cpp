#include "Day08-SpaceImageFormat.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day08SpaceImageFormat)
{
public:

    TEST_METHOD(checksumOfLayerWithMostZeroes_SimpleTests)
    {
        Assert::AreEqual(1u, AoC::checksumOfLayerWithMostZeroes("123456789012", 3u, 2u));
        Assert::AreEqual(6u, AoC::checksumOfLayerWithMostZeroes("003456011212", 3u, 2u));
    }

};
