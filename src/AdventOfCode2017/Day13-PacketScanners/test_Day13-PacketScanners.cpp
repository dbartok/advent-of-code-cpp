#include "CppUnitTest.h"

#include "Day13-PacketScanners.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day13PacketScanners)
{
public:

    TEST_METHOD(severityOfWholeTrip_SimpleTests)
    {
        Assert::AreEqual(24u, AoC::severityOfWholeTrip(m_scannerRangeLines));
    }

    TEST_METHOD(smallestDelayNotToGetCaught_SimpleTests)
    {
        Assert::AreEqual(10u, AoC::smallestDelayNotToGetCaught(m_scannerRangeLines));
    }

private:
    std::vector<std::string> m_scannerRangeLines =
    {
        "0 : 3",
        "1 : 2",
        "4 : 4",
        "6 : 4"
    };
};
