#include "Day06-TuningTrouble.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2022::Day06;

TEST_CLASS(Day06TuningTrouble)
{
public:

    TEST_METHOD(numCharactersProcessedBeforeStartOfPacketMarker_SimpleTests)
    {
        Assert::AreEqual(7, CurrentDay::numCharactersProcessedBeforeStartOfPacketMarker("mjqjpqmgbljsphdztnvjfqwrcgsmlb"));
        Assert::AreEqual(5, CurrentDay::numCharactersProcessedBeforeStartOfPacketMarker("bvwbjplbgvbhsrlpgdmjqwftvncz"));
        Assert::AreEqual(6, CurrentDay::numCharactersProcessedBeforeStartOfPacketMarker("nppdvjthqldpwncqszvftbrmjlhg"));
        Assert::AreEqual(10, CurrentDay::numCharactersProcessedBeforeStartOfPacketMarker("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg"));
        Assert::AreEqual(11, CurrentDay::numCharactersProcessedBeforeStartOfPacketMarker("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"));
    }

};
