#include "Day14-ExtendedPolymerization.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2021::Day14;

TEST_CLASS(Day14ExtendedPolymerization)
{
public:

    TEST_METHOD(mostAndLeastCommonElementQuantityDifference_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "NNCB",
            "",
            "CH -> B",
            "HH -> N",
            "CB -> H",
            "NH -> C",
            "HB -> C",
            "HC -> B",
            "HN -> C",
            "NN -> C",
            "BH -> H",
            "NC -> B",
            "NB -> B",
            "BN -> B",
            "BB -> N",
            "BC -> B",
            "CC -> N",
            "CN -> C"
        };

        Assert::AreEqual(1588, CurrentDay::mostAndLeastCommonElementQuantityDifference(lines));
    }

};
