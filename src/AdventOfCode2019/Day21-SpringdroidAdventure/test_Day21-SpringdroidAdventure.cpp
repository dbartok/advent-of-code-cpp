#include "Day21-SpringdroidAdventure.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day21SpringdroidAdventure)
{
public:

    TEST_METHOD(amountOfHullDamage_SimpleTests)
    {
        AoC::IntcodeNumberType mem = 1000; // Some high memory address
        Assert::AreEqual(10000ll, AoC::amountOfHullDamage(
            {
                3, mem, 104, 10000, 99
            }));
    }

};
