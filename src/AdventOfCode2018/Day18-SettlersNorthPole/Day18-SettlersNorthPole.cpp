#include "Day18-SettlersNorthPole.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const unsigned NUM_CYCLES_SHORT = 10;
const unsigned NUM_CYCLES_LONG = 1'000'000'000;
}

namespace AdventOfCode
{

enum class Acre
{
    OPEN,
    TREE,
    LUMBERYARD,
};

using LumberAreaRow = std::vector<Acre>;
using LumberArea = std::vector<LumberAreaRow>;

class LumberLandscapeSimulator
{
public:
    LumberLandscapeSimulator(LumberArea initialArea)
        : m_lumberArea{std::move(initialArea)}
        , m_width{0}
        , m_height{m_lumberArea.size()}
    {
        bool isRectangle = Utils::allElementsEqual(m_lumberArea.cbegin(), m_lumberArea.cend(), [](const LumberAreaRow& lhs, const LumberAreaRow& rhs)
                                                   {
                                                       return lhs.size() == rhs.size();
                                                   });

        if (!isRectangle)
        {
            throw std::runtime_error("Area needs to be a rectangle.");
        }

        if (m_height > 0)
        {
            m_width = m_lumberArea.front().size();
        }
    }

    void run(unsigned numIterations)
    {
        for (unsigned i = 0; i < numIterations; ++i)
        {
            m_previousResults.push_back(m_lumberArea);

            executeCycle();

            auto findResult = std::find(m_previousResults.cbegin(), m_previousResults.cend(), m_lumberArea);
            if (findResult != m_previousResults.cend())
            {
                unsigned cycleLength = std::distance(findResult, m_previousResults.cend());
                unsigned remainingIterations = numIterations - i - 1;
                unsigned resultPosInCycle = remainingIterations % cycleLength;
                auto finalPositionIter = findResult + resultPosInCycle;

                m_lumberArea = *finalPositionIter;
                break;
            }
        }
    }

    unsigned getResourceValue() const
    {
        return countAll(Acre::TREE) * countAll(Acre::LUMBERYARD);
    }

private:
    LumberArea m_lumberArea;
    unsigned m_width;
    unsigned m_height;

    std::vector<LumberArea> m_previousResults;

    void executeCycle()
    {
        LumberArea newLumberArea(m_height, LumberAreaRow{m_width, Acre::OPEN});

        for (unsigned j = 0; j < m_lumberArea.size(); ++j)
        {
            for (unsigned i = 0; i < m_lumberArea.size(); ++i)
            {
                newLumberArea[j][i] = getNewAcreAt(i, j);
            }
        }

        m_lumberArea = newLumberArea;
    }

    Acre getNewAcreAt(unsigned x, unsigned y)
    {
        std::vector<Acre> neighbors = getNeighborsAt(x, y);
        unsigned treeCount = std::count(neighbors.cbegin(), neighbors.cend(), Acre::TREE);
        unsigned lumberyardCount = std::count(neighbors.cbegin(), neighbors.cend(), Acre::LUMBERYARD);

        Acre currentAcre = m_lumberArea[y][x];

        if (currentAcre == Acre::OPEN)
        {
            return treeCount >= 3 ? Acre::TREE : Acre::OPEN;
        }
        else if (currentAcre == Acre::TREE)
        {
            return lumberyardCount >= 3 ? Acre::LUMBERYARD : Acre::TREE;
        }
        else
        {
            return (treeCount > 0 && lumberyardCount > 0) ? Acre::LUMBERYARD : Acre::OPEN;
        }
    }

    std::vector<Acre> getNeighborsAt(unsigned x, unsigned y)
    {
        std::vector<Acre> neighbors;

        unsigned iBegin = (x > 0) ? (x - 1) : 0;
        unsigned jBegin = (y > 0) ? (y - 1) : 0;

        unsigned iEnd = (x < m_width - 1) ? (x + 1) : (m_width - 1);
        unsigned jEnd = (y < m_height - 1) ? (y + 1) : (m_height - 1);

        for (int j = jBegin; j <= jEnd; ++j)
        {
            for (int i = iBegin; i <= iEnd; ++i)
            {
                if (i == x && j == y)
                {
                    continue;
                }

                neighbors.push_back(m_lumberArea[j][i]);
            }
        }

        return neighbors;
    }

    unsigned countAll(Acre acreToCount) const
    {
        return std::accumulate(m_lumberArea.cbegin(), m_lumberArea.cend(), 0u, [acreToCount](unsigned acc, const LumberAreaRow& row)
                               {
                                   return acc + std::count(row.cbegin(), row.cend(), acreToCount);
                               });
    }
};

Acre parseAcre(char acreChar)
{
    if (acreChar == '|')
    {
        return Acre::TREE;
    }
    else if (acreChar == '.')
    {
        return Acre::OPEN;
    }
    else if (acreChar == '#')
    {
        return Acre::LUMBERYARD;
    }
    else
    {
        throw std::runtime_error("Unknown acre character: " + acreChar);
    }
}

LumberAreaRow parseLumberAreaRow(const std::string& lumberAreaLine)
{
    LumberAreaRow lumberAreaRow;

    for (const auto& c : lumberAreaLine)
    {
        lumberAreaRow.push_back(parseAcre(c));
    }

    return lumberAreaRow;
}

LumberArea parseLumberArea(const std::vector<std::string>& lumberAreaLines)
{
    LumberArea lumberArea;

    for (const auto& line : lumberAreaLines)
    {
        lumberArea.push_back(parseLumberAreaRow(line));
    }

    return lumberArea;
}

unsigned totalResourceValueAfterShortTime(const std::vector<std::string>& lumberAreaLines)
{
    LumberArea initialArea = parseLumberArea(lumberAreaLines);

    LumberLandscapeSimulator lumberLandscapeSimulator{std::move(initialArea)};
    lumberLandscapeSimulator.run(NUM_CYCLES_SHORT);

    return lumberLandscapeSimulator.getResourceValue();
}

unsigned totalResourceValueAfterLongTime(const std::vector<std::string>& lumberAreaLines)
{
    LumberArea initialArea = parseLumberArea(lumberAreaLines);

    LumberLandscapeSimulator lumberLandscapeSimulator{std::move(initialArea)};
    lumberLandscapeSimulator.run(NUM_CYCLES_LONG);

    return lumberLandscapeSimulator.getResourceValue();
}

}
