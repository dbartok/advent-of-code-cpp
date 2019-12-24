#include "Day23-CategorySix.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day23CategorySix)
{
public:

    TEST_METHOD(secondPartOfFirstPacketAtSpecialAddress_SimpleTests)
    {
        AoC::IntcodeNumberType mem = 1000; // Some high memory address
        Assert::AreEqual(55ll, AoC::secondPartOfFirstPacketAtSpecialAddress(
            {
                3, mem,                                           // Read network address
                104, 255, 104, 54, 104, 55,                       // Write to special address
                99
            }));
    }

    TEST_METHOD(secondPartOfFirstDuplicateMessageByNat_SimpleTests)
    {
        AoC::IntcodeNumberType mem = 1000; // Some high memory address
        Assert::AreEqual(55ll, AoC::secondPartOfFirstDuplicateMessageByNat(
            {
                3, mem,                                           // Read network address
                104, 255, 104, 54, 104, 55, 3, mem,               // Write to NAT and wait
                104, 255, 104, 54, 104, 55, 3, mem,               // Write to NAT and wait
                99
            }));
    }

};
