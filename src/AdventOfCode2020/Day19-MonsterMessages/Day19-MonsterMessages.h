#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int numMessagesMatchingRuleZero(const std::vector<std::string>& ruleAndMessageLines);
int numMessagesMatchingRuleZeroWithInfiniteRules(const std::vector<std::string>& ruleAndMessageLines);

}
