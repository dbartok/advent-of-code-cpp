#include "Day04-HighEntropyPassphrases.h"

#include "../../Common/Utils.h"

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <tuple>
#include <functional>
#include <algorithm>

namespace AdventOfCode
{

bool areNoDuplicateWords(const std::vector<std::string>& words)
{
    std::unordered_set<std::string> wordOccurences;

    for (const auto& word : words)
    {
        bool insertionTookPlace;
        std::tie(std::ignore, insertionTookPlace) = wordOccurences.insert(word);

        // Element already in set
        if (!insertionTookPlace)
        {
            return false;
        }
    }

    return true;
}

bool areNoAnagramWords(const std::vector<std::string>& words)
{
    // This solution is very simple because it reuses areNoDuplicateWords, and is also efficient as long as the words themselves are short
    // However, it could be made more efficient if the words are long - sorting the letters in the words is O(n*log(n)) for each individual word, where n is the length of the given word
    // An O(n) solution for each word is possible by creating a hashmap of their letter occurences instead
    // In the second step, looking for duplicates in the vector of hashmaps instead of the actual words is still linear with the number of words

    std::vector<std::string> wordsWithLettersSorted;
    wordsWithLettersSorted.reserve(words.size());
    std::transform(words.begin(), words.end(), std::back_inserter(wordsWithLettersSorted), [](std::string s)
                   {
                       std::sort(s.begin(), s.end());
                       return s;
                   });

    return areNoDuplicateWords(wordsWithLettersSorted);
}

unsigned numPassphrasesMatchingVerifier(const std::vector<std::string>& passphrases, std::function<bool(const std::vector<std::string>&)> verifierFunc)
{
    unsigned numCorrectPassphrases = 0;

    for (const auto& passphrase : passphrases)
    {
        std::vector<std::string> words;
        Utils::splitStringIntoTypedVector(passphrase, words);

        if (verifierFunc(words))
        {
            ++numCorrectPassphrases;
        }
    }

    return numCorrectPassphrases;
}

unsigned numPassphrasesNoDuplicateWords(const std::vector<std::string>& passphrases)
{
    return numPassphrasesMatchingVerifier(passphrases, areNoDuplicateWords);
}

unsigned numPassphrasesNoAnagramWords(const std::vector<std::string>& passphrases)
{
    return numPassphrasesMatchingVerifier(passphrases, areNoAnagramWords);
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string lineBuffer;
    std::vector<std::string> lines;
    while (std::getline(fileIn, lineBuffer))
    {
        lines.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << AoC::numPassphrasesNoDuplicateWords(lines) << std::endl;
    std::cout << "Second part: " << AoC::numPassphrasesNoAnagramWords(lines) << std::endl;
}
