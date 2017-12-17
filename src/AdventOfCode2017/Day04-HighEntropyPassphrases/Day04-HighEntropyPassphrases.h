#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned numPassphrasesNoDuplicateWords(const std::vector<std::string>& passphrase);
unsigned numPassphrasesNoAnagramWords(const std::vector<std::string>& passphrases);

}
