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

protected:
    virtual bool canVisitLowercaseCave(const std::vector<Cave>& pathSoFar, const Cave& cave) = 0;

    static bool isUppercaseCave(const Cave& cave)
    {
        return std::all_of(cave.cbegin(), cave.cend(), [](const auto& c)
                           {
                               return std::isupper(c);
                           });
    }

private:
    std::unordered_map<Cave, Caves> m_caveConnectedCaves;

    unsigned m_numPathsFromStartToEnd = 0;

    bool canVisitCave(const std::vector<Cave>& pathSoFar, const Cave& cave)
    {
        if (isUppercaseCave(cave))
        {
            return true;
        }

        else
        {
            return canVisitLowercaseCave(pathSoFar, cave);
        }
    }
};

class CaveTraverserWithSingleVisitationLimitLowercase : public CaveTraverser
{
public:
    using CaveTraverser::CaveTraverser;

protected:
    bool canVisitLowercaseCave(const std::vector<Cave>& pathSoFar, const Cave& cave) override
    {
        return std::find(pathSoFar.cbegin(), pathSoFar.cend(), cave) == pathSoFar.cend();
    }
};

class CaveTraverserWithDoubleVisitationLimitForOneLowercase : public CaveTraverser
{
public:
    using CaveTraverser::CaveTraverser;

protected:
    bool canVisitLowercaseCave(const std::vector<Cave>& pathSoFar, const Cave& cave) override
    {
        if (cave == START_CAVE || cave == END_CAVE)
        {
            return false;
        }

        std::unordered_map<Cave, int> caveToNumOccurencesInPath = getLowerCaseCaveToNumOccurencesInPath(pathSoFar);
        const auto caveToNumOccurrencesIter = caveToNumOccurencesInPath.find(cave);
        const int caveCountInPath = caveToNumOccurrencesIter != caveToNumOccurencesInPath.cend() ? caveToNumOccurrencesIter->second : 0;

        if (caveCountInPath == 0)
        {
            return true;
        }
        else if (caveCountInPath == 1)
        {
            const unsigned numLowercaseCavesDuplicatedAlreadyInPath = std::count_if(caveToNumOccurencesInPath.cbegin(), caveToNumOccurencesInPath.cend(), [](const auto& elem)
                                                                                    {
                                                                                        return elem.second == 2;
                                                                                    });
            return numLowercaseCavesDuplicatedAlreadyInPath == 0;
        }
        else
        {
            return false;
        }
    }

private:
    static std::unordered_map<Cave, int> getLowerCaseCaveToNumOccurencesInPath(const std::vector<Cave>& pathSoFar)
    {
        std::unordered_map<Cave, int> caveToNumOccurencesInPath;

        for (const auto& cave : pathSoFar)
        {
            if (!isUppercaseCave(cave))
            {
                ++caveToNumOccurencesInPath[cave];
            }
        }

        return caveToNumOccurencesInPath;
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
    CaveTraverserWithSingleVisitationLimitLowercase caveTraverser{std::move(caveConnections)};
    caveTraverser.traverse();
    return caveTraverser.getNumPathsFromStartToEnd();
}

unsigned numPathsWithSmallCavesVisitedAtMostTwice(const std::vector<std::string>& caveConnectionLines)
{
    std::vector<CaveConnection> caveConnections = parseCaveConnectionLines(caveConnectionLines);
    CaveTraverserWithDoubleVisitationLimitForOneLowercase caveTraverser{std::move(caveConnections)};
    caveTraverser.traverse();
    return caveTraverser.getNumPathsFromStartToEnd();
}

}
}
}