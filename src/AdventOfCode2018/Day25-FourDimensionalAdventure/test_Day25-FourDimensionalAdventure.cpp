#include "Day25-FourDimensionalAdventure.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day25FourDimensionalAdventure)
{
public:

    TEST_METHOD(numConstellationsFormed_PointsCloseToOrigin)
    {
        std::vector<std::string> lines =
        {
            "0,0,0,0",
            "3,0,0,0",
            "0,3,0,0",
            "0,0,3,0",
            "0,0,0,3",
            "0,0,0,6",
            "9,0,0,0",
            "12,0,0,0"
        };

        Assert::AreEqual(2u, AoC::numConstellationsFormed(lines));
    }

    TEST_METHOD(numConstellationsFormed_NegativePointsLessConnected)
    {
        std::vector<std::string> lines =
        {
            "-1,2,2,0",
            "0,0,2,-2",
            "0,0,0,-2",
            "-1,2,0,0",
            "-2,-2,-2,2",
            "3,0,2,-1",
            "-1,3,2,2",
            "-1,0,-1,0",
            "0,2,1,-2",
            "3,0,0,0"
        };

        Assert::AreEqual(4u, AoC::numConstellationsFormed(lines));
    }

    TEST_METHOD(numConstellationsFormed_NegativePointsMoreConnected)
    {
        std::vector<std::string> lines =
        {
            "1,-1,0,1",
            "2,0,-1,0",
            "3,2,-1,0",
            "0,0,3,1",
            "0,0,-1,-1",
            "2,3,-2,0",
            "-2,2,0,0",
            "2,-2,0,-1",
            "1,-1,0,-1",
            "3,2,0,2"
        };

        Assert::AreEqual(3u, AoC::numConstellationsFormed(lines));
    }

    TEST_METHOD(numConstellationsFormed_PointsDisconnected)
    {
        std::vector<std::string> lines =
        {
            "1,-1,-1,-2",
            "-2,-2,0,1",
            "0,2,1,3",
            "-2,3,-2,1",
            "0,2,3,-2",
            "-1,-1,1,-2",
            "0,-2,-1,0",
            "-2,2,3,-1",
            "1,2,2,0",
            "-1,-2,0,-2",
        };

        Assert::AreEqual(8u, AoC::numConstellationsFormed(lines));
    }

};
