#include "Day20-GrovePositioningSystem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2022::Day20;

TEST_CLASS(Day20GrovePositioningSystem)
{
public:

    TEST_METHOD(sumOfGroveCoordinates_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "1",
            "2",
            "-3",
            "3",
            "-2",
            "0",
            "4"
        };

        Assert::AreEqual(3, CurrentDay::sumOfGroveCoordinates(lines));
    }

};
