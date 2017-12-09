#include "CppUnitTest.h"

#include "Day9-StreamProcessing.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day9StreamProcessing)
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

};
