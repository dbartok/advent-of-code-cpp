#include "Day12-PassagePathing.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_map>
#include <queue>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day12
{

using Cave = std::string;
using CaveConnection = std::pair<Cave, Cave>;
using Caves = std::vector<Cave>;

namespace
{

Cave START_CAVE = "start";
Cave END_CAVE = "end";

}

class CaveTraverser
{
public:
    CaveTraverser(const std::vector<CaveConnection>& connections)
    {
        for (const auto& connection : connections)
        {
            m_caveConnectedCaves[connection.first].push_back(connection.second);
            m_caveConnectedCaves[connection.second].push_back(connection.first);
        }
    }

    void traverse()
    {
        std::queue<Caves> openCavePathsFromStart;
        openCavePathsFromStart.push({START_CAVE});

        while (!openCavePathsFromStart.empty())
        {
            const Caves currentPath = openCavePathsFromStart.front();
            openCavePathsFromStart.pop();

            const Cave& lastCaveInCurrentPath = currentPath.back();
            const Caves& connectedCaves = m_caveConnectedCaves.at(lastCaveInCurrentPath);

            for (const auto& cave : connectedCaves)
            {
                if (cave == END_CAVE)
                {
                    ++m_numPathsFromStartToEnd;
                }
                else if (canVisitCave(currentPath, cave))
                {
                    auto currentPathExtended{currentPath};
                    currentPathExtended.push_back(cave);
                    openCavePathsFromStart.push(currentPathExtended);
                }
            }
        }
    }

    unsigned getNumPathsFromStartToEnd() const
    {
        return m_numPathsFromStartToEnd;
    }

private:
    std::unordered_map<Cave, Caves> m_caveConnectedCaves;

    unsigned m_numPathsFromStartToEnd = 0;

    static bool canVisitCave(const std::vector<Cave>& pathSoFar, const Cave& cave)
    {
        if (canVisitCaveAnyNumberOfTimes(cave))
        {
            return true;
        }

        return std::find(pathSoFar.cbegin(), pathSoFar.cend(), cave) == pathSoFar.cend();
    }

    static bool canVisitCaveAnyNumberOfTimes(const Cave& cave)
    {
        return std::all_of(cave.cbegin(), cave.cend(), [](const auto& c)
                           {
                               return std::isupper(c);
                           });
    }
};

CaveConnection parseCaveConnectionLine(const std::string& caveConnectionLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, caveConnectionLine, boost::is_any_of("-"));

    return CaveConnection{std::move(tokens.at(0)), std::move(tokens.at(1))};
}

std::vector<CaveConnection> parseCaveConnectionLines(const std::vector<std::string>& caveConnectionLines)
{
    std::vector<CaveConnection> caveConnections;

    for (const auto& line : caveConnectionLines)
    {
        CaveConnection caveConnection = parseCaveConnectionLine(line);
        caveConnections.push_back(std::move(caveConnection));
    }

    return caveConnections;
}

unsigned numPathsWithSmallCavesVisitedAtMostOnce(const std::vector<std::string>& caveConnectionLines)
{
    std::vector<CaveConnection> caveConnections = parseCaveConnectionLines(caveConnectionLines);
    CaveTraverser caveTraverser{std::move(caveConnections)};
    caveTraverser.traverse();
    return caveTraverser.getNumPathsFromStartToEnd();
}

}
}
}