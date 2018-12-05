#include "Day05-AlchemicalReaction.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day05AlchemicalReaction)
{
public:

    TEST_METHOD(getReducedPolymerString_SimpleTests)
    {
        Assert::AreEqual(std::string{""}.length(), AoC::getReducedPolymerLength("aA"));
        Assert::AreEqual(std::string{""}.length(), AoC::getReducedPolymerLength("abBA"));
        Assert::AreEqual(std::string{"abAB"}.length(), AoC::getReducedPolymerLength("abAB"));
        Assert::AreEqual(std::string{"aabAAB"}.length(), AoC::getReducedPolymerLength("aabAAB"));
        Assert::AreEqual(std::string{"dabCBAcaDA"}.length(), AoC::getReducedPolymerLength("dabAcCaCBAcCcaDA"));
    }


    TEST_METHOD(getShortestLengthRemoveOneUnit_SimpleTests)
    {
        Assert::AreEqual(4u, AoC::getOneUnitRemovedShortestReducedLength("dabAcCaCBAcCcaDA"));
    }

};
