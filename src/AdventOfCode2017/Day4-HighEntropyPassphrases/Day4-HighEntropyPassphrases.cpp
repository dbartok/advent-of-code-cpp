#include "Day4-HighEntropyPassphrases.h"

#include "../../Common/Utils.h"

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <tuple>

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

unsigned numPassphrasesNoDuplicateWords(const std::vector<std::string>& passphrases)
{
    unsigned numCorrectPassphrases = 0;
    
    for (const auto& passphrase : passphrases)
    {
        std::vector<std::string> words;
        Utils::splitStringIntoVector(passphrase, words);

        if (areNoDuplicateWords(words))
        {
            ++numCorrectPassphrases;
        }
    }

    return numCorrectPassphrases;
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
}
