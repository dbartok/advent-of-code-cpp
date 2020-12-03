#include "Day03-TobogganTrajectory.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>

#include <array>
__END_LIBRARIES_DISABLE_WARNINGS

using Vector2D = Eigen::Matrix<int64_t, 2, 1>;
using TreeGrid = std::vector<std::string>;

namespace
{

const char TREE = '#';
const Vector2D STANDARD_SLOPE{3, 1};
const std::array<Vector2D, 5> ALL_SLOPES{STANDARD_SLOPE, Vector2D{1, 1}, Vector2D{5, 1}, Vector2D{7, 1}, Vector2D{1, 2}};

}

namespace AdventOfCode
{

class SlopeChecker
{
public:
    SlopeChecker(TreeGrid treeGrid)
        : m_treeGrid{std::move(treeGrid)}
    {

    }

    int64_t getNumTreesForSlope(const Vector2D& slope) const
    {
        Vector2D currentCoords{0, 0};
        int64_t numTrees = 0;

        while (currentCoords[1] < m_treeGrid.size())
        {
            if (m_treeGrid.at(currentCoords[1]).at(currentCoords[0]) == TREE)
            {
                ++numTrees;
            }

            currentCoords += slope;
            currentCoords[0] %= m_treeGrid.at(0).size();
        }

        return numTrees;
    }

private:
    TreeGrid m_treeGrid;
};

int64_t numTreesEncounteredSingleSlope(const std::vector<std::string>& gridLines)
{
    SlopeChecker slopeChecker{gridLines};
    return slopeChecker.getNumTreesForSlope(STANDARD_SLOPE);
}

int64_t numTreesEncounteredMultipleSlopes(const std::vector<std::string>& gridLines)
{
    SlopeChecker slopeChecker{gridLines};
    int64_t productOfNumTreesOnAllSlopes = 1;

    for (const auto& slope : ALL_SLOPES)
    {
        const int64_t currentNumTrees = slopeChecker.getNumTreesForSlope(slope);
        productOfNumTreesOnAllSlopes *= currentNumTrees;
    }

    return productOfNumTreesOnAllSlopes;
}

}
