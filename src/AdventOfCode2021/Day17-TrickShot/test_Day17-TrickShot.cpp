#include "Day17-TrickShot.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2021::Day17;

TEST_CLASS(Day17TrickShot)
{
public:

    TEST_METHOD(highestYPositionToHitTarget_SimpleTests)
    {
        const std::string input = "target area: x=20..30, y=-10..-5";

        Assert::AreEqual(45, CurrentDay::highestYPositionToHitTarget(input));
    }

};
