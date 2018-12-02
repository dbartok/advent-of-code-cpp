#include "Day02-InventoryManagement.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <unordered_map>
#include <algorithm>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned countWordsWithNRepeatingChars(const std::vector<std::string>& words, unsigned n)
{
    unsigned count = 0;
    std::for_each(words.cbegin(), words.cend(), [&count, n](const std::string& word)
                  {
                      std::unordered_map<char, unsigned> charCounts;
                      std::for_each(word.cbegin(), word.cend(), [&charCounts](char c)
                                    {
                                        ++charCounts[c];
                                    });

                      const bool matches = std::any_of(charCounts.cbegin(), charCounts.cend(), [n](const auto& element) noexcept
                                                 {
                                                     return element.second == n;
                                                 });

                      if (matches)
                      {
                          ++count;
                      }
                  });
    return count;
}

unsigned checksumTwoOrThreeOfAnyLetter(const std::vector<std::string>& words)
{

    const unsigned numWordsWithTwoRepeatingLetters = countWordsWithNRepeatingChars(words, 2);
    const unsigned numWordsWithThreeRepeatingLetters = countWordsWithNRepeatingChars(words, 3);

    return numWordsWithTwoRepeatingLetters * numWordsWithThreeRepeatingLetters;
}

}
