#include "Day20-ARegularMap.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day20ARegularMap)
{
public:

    TEST_METHOD(distanceToFarthestRoom_SimpleTests)
    {
        Assert::AreEqual(3u, AoC::distanceToFarthestRoom("^WNE$"));
        Assert::AreEqual(10u, AoC::distanceToFarthestRoom("^ENWWW(NEEE|SSE(EE|N))$"));
        Assert::AreEqual(18u, AoC::distanceToFarthestRoom("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$"));
        Assert::AreEqual(23u, AoC::distanceToFarthestRoom("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$"));
        Assert::AreEqual(31u, AoC::distanceToFarthestRoom("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$"));
    }

};
