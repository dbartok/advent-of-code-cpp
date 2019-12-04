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

    TEST_METHOD(numDifferentValidPasswords_SimpleTests)
    {
        Assert::AreEqual(1u, AoC::numDifferentValidPasswords(111111, 111111));
        Assert::AreEqual(0u, AoC::numDifferentValidPasswords(223450, 223450));
        Assert::AreEqual(0u, AoC::numDifferentValidPasswords(123789, 123789));
        Assert::AreEqual(8u, AoC::numDifferentValidPasswords(111121, 111132));
    }

};
