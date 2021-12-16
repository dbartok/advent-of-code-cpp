#include "Day16-PacketDecoder.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2021::Day16;

TEST_CLASS(Day16PacketDecoder)
{
public:

    TEST_METHOD(sumOfAllVersionNumbersInPackets_SimpleTests)
    {
        Assert::AreEqual(16, CurrentDay::sumOfAllVersionNumbersInPackets("8A004A801A8002F478"));
        Assert::AreEqual(12, CurrentDay::sumOfAllVersionNumbersInPackets("620080001611562C8802118E34"));
        Assert::AreEqual(23, CurrentDay::sumOfAllVersionNumbersInPackets("C0015000016115A2E0802F182340"));
        Assert::AreEqual(31, CurrentDay::sumOfAllVersionNumbersInPackets("A0016C880162017C3686B18A3D4780"));
    }

};
