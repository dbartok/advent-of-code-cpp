#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <bitset>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class DefragmenterGrid
{
public:
    using DiskGrid = std::vector<std::vector<bool>>;

    DefragmenterGrid(DiskGrid diskGrid) noexcept;
    unsigned numSquaresUsed() const;

private:
    DiskGrid m_diskGrid;

};

}
