#pragma once

#include <vector>
#include <string>

namespace AdventOfCode
{

unsigned numPassphrasesNoDuplicateWords(const std::vector<std::string>& passphrase);
unsigned numPassphrasesNoAnagramWords(const std::vector<std::string>& passphrases);

}
