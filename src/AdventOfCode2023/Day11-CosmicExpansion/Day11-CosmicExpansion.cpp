#include "Day11-CosmicExpansion.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const char GALAXY = '#';

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day11
{

class GalaxyImageAnalyzer
{
public:
    GalaxyImageAnalyzer(std::vector<std::string> image)
        : m_image{std::move(image)}
        , m_height{m_image.size()}
        , m_width{m_image.front().size()}
    {

    }

    void analyze()
    {
        for (int j = 0; j < m_height; ++j)
        {
            m_emptyRowCoordinates.insert(j);
        }

        for (int i = 0; i < m_height; ++i)
        {
            m_emptyColumnCoordinates.insert(i);
        }

        for (int i = 0; i < m_width; ++i)
        {
            for (int j = 0; j < m_height; ++j)
            {
                if (m_image.at(j).at(i) == GALAXY)
                {
                    m_galaxyCoordinates.push_back({i, j});
                    m_emptyRowCoordinates.erase(j);
                    m_emptyColumnCoordinates.erase(i);
                }
            }
        }
    }

    int getSumOfAllShortestPathsBetweenAllPairs() const
    {
        int sumOfAllShortestPathsBetweenAllPairs = 0;

        for (auto firstGalaxyCoordinatesIter = m_galaxyCoordinates.cbegin(); firstGalaxyCoordinatesIter != m_galaxyCoordinates.cend(); ++firstGalaxyCoordinatesIter)
        {
            for (auto secondGalaxyCoordinatesIter = firstGalaxyCoordinatesIter + 1; secondGalaxyCoordinatesIter != m_galaxyCoordinates.cend(); ++secondGalaxyCoordinatesIter)
            {
                const int distance = getDistanceBetween(*firstGalaxyCoordinatesIter, *secondGalaxyCoordinatesIter);
                sumOfAllShortestPathsBetweenAllPairs += distance;
            }
        }

        return sumOfAllShortestPathsBetweenAllPairs;
    }

private:
    using Coordinates = std::pair<int, int>;

    std::vector<std::string> m_image;
    size_t m_width;
    size_t m_height;

    std::vector<Coordinates> m_galaxyCoordinates;
    std::unordered_set<int> m_emptyRowCoordinates;
    std::unordered_set<int> m_emptyColumnCoordinates;

    int getDistanceBetween(const Coordinates& firstPoint, const Coordinates& secondPoint) const
    {
        int numStepsWithExtendedLength = 0;

        for (int j = std::min(firstPoint.second, secondPoint.second); j <= std::max(firstPoint.second, secondPoint.second); ++j)
        {
            if (m_emptyRowCoordinates.count(j))
            {
                ++numStepsWithExtendedLength;
            }
        }

        for (int i = std::min(firstPoint.first, secondPoint.first); i <= std::max(firstPoint.first, secondPoint.first); ++i)
        {
            if (m_emptyColumnCoordinates.count(i))
            {
                ++numStepsWithExtendedLength;
            }
        }

        return std::abs(firstPoint.first - secondPoint.first) + std::abs(firstPoint.second - secondPoint.second) + numStepsWithExtendedLength;
    }
};

int sumOfShortestPathsBetweenAllPairs(const std::vector<std::string>& imageLines)
{
    GalaxyImageAnalyzer galaxyImageAnalyzer{imageLines};

    galaxyImageAnalyzer.analyze();

    return galaxyImageAnalyzer.getSumOfAllShortestPathsBetweenAllPairs();
}

}
}
}