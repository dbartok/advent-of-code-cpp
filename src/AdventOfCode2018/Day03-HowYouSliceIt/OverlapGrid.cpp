#include "OverlapGrid.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

Rectangle::Rectangle(unsigned topLeftX, unsigned topLeftY, unsigned width, unsigned height) noexcept
    : topLeftX{topLeftX}
    , topLeftY{topLeftY}
    , width{width}
    , height{height}
{

}

OverlapGrid::OverlapGrid(size_t size)
    : m_grid(size, std::vector<int>(size, 0))
{

}

void OverlapGrid::addRectangle(const Rectangle& rectangle)
{
    for (unsigned i = rectangle.topLeftX; i < rectangle.topLeftX + rectangle.width; ++i)
    {
        for (unsigned j = rectangle.topLeftY; j < rectangle.topLeftY + rectangle.height; ++j)
        {
            ++m_grid[j][i];
        }
    }
}

unsigned OverlapGrid::getOverlapSize() const
{
    return std::accumulate(m_grid.cbegin(), m_grid.cend(), 0u, [](unsigned acc, const std::vector<int>& row)
                           {
                               return acc + std::count_if(row.cbegin(), row.cend(), [](int i) noexcept
                                                    {
                                                        return i >= 2;
                                                    });
                           });
}

}