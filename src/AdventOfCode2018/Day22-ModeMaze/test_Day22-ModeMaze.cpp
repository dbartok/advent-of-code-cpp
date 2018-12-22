#include "Day22-ModeMaze.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day22ModeMaze)
{
public:

    TEST_METHOD(solution1_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "depth: 510",
            "target: 10,10"
        };

        Assert::AreEqual(114u, AoC::riskLevelOfSmallestArea(lines));
    }

};
