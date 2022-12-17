#include "Day17-PyroclasticFlow.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2022::Day17;

TEST_CLASS(Day17PyroclasticFlow)
{
public:

    TEST_METHOD(towerHeightAfterRocksStoppedFalling_SimpleTests)
    {
        const std::string jetPattern = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";

        Assert::AreEqual(3068, CurrentDay::towerHeightAfterRocksStoppedFalling(jetPattern));
    }

};
