#include "Day01-ReportRepair.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <unordered_set>
#include <stdexcept>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

int TARGET_SUM = 2020;

}

namespace AdventOfCode
{

int productOfTwoEntriesWithGivenSum(const std::vector<int>& entries)
{
    std::unordered_set<int> entries_seen_before;

    for (const auto& entry : entries)
    {
        const int counterpart = TARGET_SUM - entry;
        if (entries_seen_before.find(counterpart) != entries_seen_before.cend())
        {
            return entry * counterpart;
        }

        entries_seen_before.insert(entry);
    }

    throw std::runtime_error("Can't find two entries with the given sum.");
}

}
