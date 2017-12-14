#include "Day09-StreamProcessing.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day09StreamProcessing)
{
public:

    TEST_METHOD(totalScoreForAllGroups_SimpleTests)
    {
        Assert::AreEqual(1u, AoC::totalScoreForAllGroups("{}"));
        Assert::AreEqual(6u, AoC::totalScoreForAllGroups("{{{}}}"));
        Assert::AreEqual(5u, AoC::totalScoreForAllGroups("{{},{}}"));
        Assert::AreEqual(16u, AoC::totalScoreForAllGroups("{{{},{},{{}}}}"));
        Assert::AreEqual(1u, AoC::totalScoreForAllGroups("{<a>,<a>,<a>,<a>}"));
        Assert::AreEqual(9u, AoC::totalScoreForAllGroups("{{<ab>},{<ab>},{<ab>},{<ab>}}"));
        Assert::AreEqual(9u, AoC::totalScoreForAllGroups("{{<!!>},{<!!>},{<!!>},{<!!>}}"));
        Assert::AreEqual(3u, AoC::totalScoreForAllGroups("{{<a!>},{<a!>},{<a!>},{<ab>}}"));
    }

    TEST_METHOD(totalGarbageCount_SimpleTests)
    {
        Assert::AreEqual(0u, AoC::totalGarbageCount("<>"));
        Assert::AreEqual(17u, AoC::totalGarbageCount("<random characters>"));
        Assert::AreEqual(3u, AoC::totalGarbageCount("<<<<>"));
        Assert::AreEqual(2u, AoC::totalGarbageCount("<{!>}>"));
        Assert::AreEqual(0u, AoC::totalGarbageCount("<!!>"));
        Assert::AreEqual(0u, AoC::totalGarbageCount("<!!!>>"));
        Assert::AreEqual(10u, AoC::totalGarbageCount("<{o\"i!a,<{i<a>"));
    }

};
