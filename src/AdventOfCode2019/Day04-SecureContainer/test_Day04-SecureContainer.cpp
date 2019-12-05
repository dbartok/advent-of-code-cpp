#include "Day04-SecureContainer.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day04SecureContainer)
{
public:

    TEST_METHOD(numDifferentValidPasswordsSimple_SimpleTests)
    {
        Assert::AreEqual(1u, AoC::numDifferentValidPasswordsSimple(111111, 111111));
        Assert::AreEqual(0u, AoC::numDifferentValidPasswordsSimple(223450, 223450));
        Assert::AreEqual(0u, AoC::numDifferentValidPasswordsSimple(123789, 123789));
        Assert::AreEqual(8u, AoC::numDifferentValidPasswordsSimple(111121, 111132));
    }

    TEST_METHOD(numDifferentValidPasswordsLargerGroupIsNotMatching_SimpleTests)
    {
        Assert::AreEqual(1u, AoC::numDifferentValidPasswordsLargerGroupIsNotMatching(112233, 112233));
        Assert::AreEqual(0u, AoC::numDifferentValidPasswordsLargerGroupIsNotMatching(123444, 123444));
        Assert::AreEqual(1u, AoC::numDifferentValidPasswordsLargerGroupIsNotMatching(111122, 111122));
        Assert::AreEqual(0u, AoC::numDifferentValidPasswordsLargerGroupIsNotMatching(234448, 234448));
        Assert::AreEqual(1u, AoC::numDifferentValidPasswordsLargerGroupIsNotMatching(234456, 234456));
        Assert::AreEqual(2u, AoC::numDifferentValidPasswordsLargerGroupIsNotMatching(111121, 111134));
    }

};
