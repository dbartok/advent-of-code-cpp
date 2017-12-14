#include "DefragmenterGrid.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

DefragmenterGrid::DefragmenterGrid(DiskGrid diskGrid) noexcept
    : m_diskGrid{std::move(diskGrid)}
{

}

unsigned DefragmenterGrid::numSquaresUsed() const
{
    return std::accumulate(m_diskGrid.cbegin(), m_diskGrid.cend(), 0u, [](unsigned totalSum, const auto& row)
                           {
                               return totalSum + std::accumulate(row.cbegin(), row.cend(), 0u, [](unsigned rowSum, bool cell) noexcept
                                                                 {
                                                                     if (cell)
                                                                     {
                                                                         ++rowSum;
                                                                     }

                                                                     return rowSum;
                                                                 });
                           });
}

}
