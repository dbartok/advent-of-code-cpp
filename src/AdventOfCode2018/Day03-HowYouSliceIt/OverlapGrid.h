#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct Rectangle;
using Rectangles = std::vector<Rectangle>;

struct Rectangle
{
    unsigned topLeftX;
    unsigned topLeftY;
    unsigned width;
    unsigned height;

    Rectangle(unsigned topLeftX, unsigned topLeftY, unsigned width, unsigned height) noexcept;
};

class OverlapGrid
{
public:
    OverlapGrid(size_t size);

    void addRectangle(const Rectangle& rectangle);

    unsigned getOverlapSize() const;

private:
    using Grid = std::vector<std::vector<int>>;

    Grid m_grid;
};

}