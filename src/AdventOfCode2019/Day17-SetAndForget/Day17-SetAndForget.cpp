#include "Day17-SetAndForget.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<int, int>;
using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

class ScaffoldAnalyzer
{
public:
    ScaffoldAnalyzer(CoordinatesSet scaffolds)
        : m_scaffolds{std::move(scaffolds)}
    {

    }

    int getSumOfAlignmentParameters() const
    {
        int sum = 0;

        for (const auto& scaffold : m_scaffolds)
        {
            std::vector<Coordinates> neighborScaffolds = getNeighborScaffolds(scaffold);
            if (neighborScaffolds.size() == 4)
            {
                sum += (scaffold.first * scaffold.second);
            }
        }

        return sum;
    }

private:
    CoordinatesSet m_scaffolds;

    std::vector<Coordinates> getNeighborScaffolds(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> neighbors
        {
            {coordinates.first - 1, coordinates.second},
            {coordinates.first + 1, coordinates.second},
            {coordinates.first, coordinates.second - 1},
            {coordinates.first, coordinates.second + 1},
        };

        neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [this](const auto& coord)
                                       {
                                           return m_scaffolds.find(coord) == m_scaffolds.cend();
                                       }), neighbors.end());

        return neighbors;
    }
};

CoordinatesSet createScaffolds(const std::vector<IntcodeNumberType>& outputs)
{
    CoordinatesSet scaffolds;

    int i = 0;
    int j = 0;
    for (auto output : outputs)
    {
        if (output == '\n')
        {
            i = 0;
            ++j;
            continue;
        }

        if (output == '#')
        {
            scaffolds.insert({i, j});
        }
        ++i;
    }

    return scaffolds;
}

int sumOfAlignmentParameters(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};
    interpreter.execute();

    CoordinatesSet scaffolds = createScaffolds(interpreter.getOutputs());

    ScaffoldAnalyzer analyzer{std::move(scaffolds)};

    return analyzer.getSumOfAlignmentParameters();
}

}
