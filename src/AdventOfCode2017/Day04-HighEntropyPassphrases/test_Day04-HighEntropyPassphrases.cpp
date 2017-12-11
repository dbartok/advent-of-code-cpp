#include "CppUnitTest.h"

#include "Day04-HighEntropyPassphrases.h"

#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day04HighEntropyPassphrases)
{
public:

    TEST_METHOD(numPassphrasesNoDuplicateWords_SimpleTests)
    {
        std::vector<std::string> passphrases = 
        {
            "aa bb cc dd ee",
            "aa bb cc dd aa",
            "aa bb cc dd aaa"
        };

        Assert::AreEqual(2u, AoC::numPassphrasesNoDuplicateWords(passphrases));
    }

    TEST_METHOD(numPassphrasesNoAnagramWords_SimpleTests)
    {
        std::vector<std::string> passphrases =
        {
            "abcde fghij",
            "abcde xyz ecdab",
            "a ab abc abd abf abj",
            "iiii oiii ooii oooi oooo",
            "oiii ioii iioi iiio"
        };

        Assert::AreEqual(3u, AoC::numPassphrasesNoAnagramWords(passphrases));
    }

};
