#include "Day19-TractorBeam.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day19TractorBeam)
{
public:

    TEST_METHOD(numPointsAffectedByBeam_SimpleTests)
    {
        AoC::IntcodeNumberType mem = 1000; // Some high memory address
        Assert::AreEqual(2500, AoC::numPointsAffectedByBeam(
            {
                3, mem, 3, mem, 104, 1, 99
            }));
    }

    TEST_METHOD(squareClosestPointChecksum_SimpleTests)
    {
        AoC::IntcodeNumberType mem = 1000; // Some high memory address
        Assert::AreEqual(0, AoC::squareClosestPointChecksum(
            {
                3, mem, 3, mem, 104, 1, 99
            }));
    }

};
