#include "Day07-TheSumOfItsParts.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day07TheSumOfItsParts)
{
public:

    TEST_METHOD(topologicalOrderOfInstructions_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "Step C must be finished before step A can begin.",
            "Step C must be finished before step F can begin.",
            "Step A must be finished before step B can begin.",
            "Step A must be finished before step D can begin.",
            "Step B must be finished before step E can begin.",
            "Step D must be finished before step E can begin.",
            "Step F must be finished before step E can begin."
        };

        Assert::AreEqual({"CABDFE"}, AoC::topologicalOrderOfInstructions(lines));
    }

};
