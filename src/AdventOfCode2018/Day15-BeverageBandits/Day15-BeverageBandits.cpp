#include "Day15-BeverageBandits.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash/hash.hpp>

#include <memory>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const int DEFAULT_HIT_POINTS = 200;
const int DEFAULT_ATTACK_POWER = 3;
}

namespace AdventOfCode
{

class Unit;
using UnitSharedPtr = std::shared_ptr<Unit>;

using Coordinates = std::pair<unsigned, unsigned>;

class Unit
{
public:
    Unit(bool faction, Coordinates coordinates, int attackPower)
        : m_faction{faction}
        , m_coordinates{std::move(coordinates)}
        , m_attackPower(attackPower)
        , m_hitPoints{DEFAULT_HIT_POINTS}
    {

    }

    const Coordinates& getCoordinates() const
    {
        return m_coordinates;
    }

    const bool getFaction() const
    {
        return m_faction;
    }

    bool isHostileTowards(const UnitSharedPtr& other)
    {
        return m_faction != other->getFaction();
    }

    bool isDead()
    {
        return m_hitPoints <= 0;
    }

    int getHitPoints()
    {
        return m_hitPoints;
    }

    void attack(const UnitSharedPtr& other)
    {
        other->m_hitPoints -= m_attackPower;
    }

    void setCoordinates(Coordinates coordinates)
    {
        m_coordinates = std::move(coordinates);
    }

private:
    bool m_faction;
    Coordinates m_coordinates;
    int m_attackPower;
    int m_hitPoints;
};


using WallsGridLine = std::vector<bool>;
using WallGrid = std::vector<WallsGridLine>;

using CoordinatesToUnit = std::unordered_map<Coordinates, UnitSharedPtr, boost::hash<Coordinates>>;

struct OriginAndCurrentCoordinates
{
    Coordinates origin;
    Coordinates searchCoordinates;

    OriginAndCurrentCoordinates(Coordinates origin, Coordinates searchCoordinates)
        : origin{origin}
        , searchCoordinates{std::move(searchCoordinates)}
    {

    }
};

class GameManager
{
public:
    GameManager(WallGrid wallGrid, CoordinatesToUnit coordinatesToUnit)
        : m_wallGrid{std::move(wallGrid)}
        , m_coordinatesToUnit{std::move(coordinatesToUnit)}
    {

    }

    void play()
    {
        while (true)
        {
            std::list<UnitSharedPtr> unitsInTurnOrder = getUnitsInTurnOrder();

            for (auto unitIter = unitsInTurnOrder.begin(); unitIter != unitsInTurnOrder.end(); ++unitIter)
            {
                if (isOnlyOneFactionRemaining())
                {
                    return;
                }

                auto unit = *unitIter;
                maybeMove(unit);

                auto attackTarget = getAttackTarget(unit, unit->getCoordinates());
                if (attackTarget)
                {
                    unit->attack(attackTarget);
                    if (attackTarget->isDead())
                    {
                        unitsInTurnOrder.erase(std::remove(unitsInTurnOrder.begin(), unitsInTurnOrder.end(), attackTarget), unitsInTurnOrder.end());
                        unitIter = std::find(unitsInTurnOrder.begin(), unitsInTurnOrder.end(), unit);
                        m_coordinatesToUnit.erase(attackTarget->getCoordinates());
                    }
                }
            }

            ++m_turnsElapsed;
        }
    }

    unsigned getNumTurnsElapsed() const
    {
        return m_turnsElapsed;
    }

    unsigned getTotalHitPointsRemaining() const
    {
        return std::accumulate(m_coordinatesToUnit.cbegin(), m_coordinatesToUnit.cend(), 0, [](int acc, const auto& elem)
                               {
                                   return acc + elem.second->getHitPoints();
                               });
    }

    unsigned getNumElvesAlive() const
    {
        return std::count_if(m_coordinatesToUnit.cbegin(), m_coordinatesToUnit.cend(), [](const auto& elem)
                            {
                                return elem.second->getFaction();
                            });
    }

private:
    WallGrid m_wallGrid;
    CoordinatesToUnit m_coordinatesToUnit;

    unsigned m_turnsElapsed = 0;

    UnitSharedPtr getAttackTarget(const UnitSharedPtr& unit, const Coordinates& unitCoordinates)
    {
        UnitSharedPtr target = nullptr;

        auto nonWallNeighborCoordinates = getNonWallNeighborCoordinates(unitCoordinates);
        for (const auto& coordinates : nonWallNeighborCoordinates)
        {
            auto findResult = m_coordinatesToUnit.find(coordinates);
            if (findResult != m_coordinatesToUnit.end() && unit->isHostileTowards(findResult->second))
            {
                if (!target || findResult->second->getHitPoints() < target->getHitPoints())
                {
                    target = findResult->second;
                }
            }
        }

        return target;
    }

    void maybeMove(UnitSharedPtr& unit)
    {
        auto adjacentTarget = getAttackTarget(unit, unit->getCoordinates());
        if (adjacentTarget)
        {
            return;
        }

        std::vector<Coordinates> allInitialFreeNeighborCoordinates = getAllFreeNeighborCoordinates(unit->getCoordinates());

        std::queue<OriginAndCurrentCoordinates> bfsQueue;
        std::queue<OriginAndCurrentCoordinates> nextBfsQueue;
        std::unordered_set<Coordinates, boost::hash<Coordinates>> visitedSet;
        for (const auto& initialFreeNeighborCoordinates : allInitialFreeNeighborCoordinates)
        {
            nextBfsQueue.push({initialFreeNeighborCoordinates, initialFreeNeighborCoordinates});
            visitedSet.insert(initialFreeNeighborCoordinates);
        }

        std::vector<OriginAndCurrentCoordinates> solutions;

        while (solutions.empty() && !nextBfsQueue.empty())
        {
            std::swap(bfsQueue, nextBfsQueue);

            while (!bfsQueue.empty())
            {
                auto currentSearchItem = bfsQueue.front();
                bfsQueue.pop();

                auto target = getAttackTarget(unit, currentSearchItem.searchCoordinates);
                if (target)
                {
                    solutions.push_back(currentSearchItem);
                }

                auto allFreeNeighborCoordinates = getAllFreeNeighborCoordinates(currentSearchItem.searchCoordinates);

                for (const auto& freeNeighborCoordinates : allFreeNeighborCoordinates)
                {
                    bool notVisited = visitedSet.insert(freeNeighborCoordinates).second;
                    if (notVisited)
                    {
                        nextBfsQueue.push({currentSearchItem.origin, freeNeighborCoordinates});
                    }
                }
            }
        }

        if (!solutions.empty())
        {
            auto best = std::min_element(solutions.begin(), solutions.end(), [](const auto& lhs, const auto& rhs)
                                         {
                                             return lhs.searchCoordinates < rhs.searchCoordinates;
                                         });

            auto moveTarget = best->origin;

            m_coordinatesToUnit.erase(unit->getCoordinates());
            unit->setCoordinates(moveTarget);
            m_coordinatesToUnit.emplace(unit->getCoordinates(), unit);
        }
    }

    std::vector<Coordinates> getAllFreeNeighborCoordinates(const Coordinates& coordinates)
    {
        auto nonWallNeighborCoordinates = getNonWallNeighborCoordinates(coordinates);
        return filterOutUnits(nonWallNeighborCoordinates);
    }

    std::vector<Coordinates> filterOutUnits(const std::vector<Coordinates>& allCoordinates)
    {
        std::vector<Coordinates> noUnitsCoordinates;

        for (const auto& coordinates : allCoordinates)
        {
            if (m_coordinatesToUnit.find(coordinates) == m_coordinatesToUnit.end())
            {
                noUnitsCoordinates.push_back(coordinates);
            }
        }

        return noUnitsCoordinates;
    }

    std::vector<Coordinates> getNonWallNeighborCoordinates(const Coordinates& coordinates)
    {
        std::vector<Coordinates> neighborCoordinates;

        if (coordinates.second > 0)
        {
            neighborCoordinates.emplace_back(coordinates.first - 1, coordinates.second);
        }

        if (coordinates.first > 0)
        {
            neighborCoordinates.emplace_back(coordinates.first, coordinates.second - 1);
        }

        if (coordinates.first < m_wallGrid[coordinates.second].size() - 1)
        {
            neighborCoordinates.emplace_back(coordinates.first, coordinates.second + 1);
        }

        if (coordinates.second < m_wallGrid.size() - 1)
        {
            neighborCoordinates.emplace_back(coordinates.first + 1, coordinates.second);
        }

        return filterOutWalls(neighborCoordinates);
    }

    std::vector<Coordinates> filterOutWalls(const std::vector<Coordinates>& allCoordinates)
    {
        std::vector<Coordinates> noWallCoordinates;

        for (const auto& coordinates : allCoordinates)
        {
            if (!m_wallGrid[coordinates.first][coordinates.second])
            {
                noWallCoordinates.push_back(coordinates);
            }
        }

        return noWallCoordinates;
    }

    std::list<UnitSharedPtr> getUnitsInTurnOrder()
    {
        std::vector<UnitSharedPtr> units;

        for (const auto& elem : m_coordinatesToUnit)
        {
            units.push_back(elem.second);
        }

        std::sort(units.begin(), units.end(), [](const UnitSharedPtr& lhs, const UnitSharedPtr& rhs) {return lhs->getCoordinates() < rhs->getCoordinates(); });

        return {units.begin(), units.end()};
    }

    bool isOnlyOneFactionRemaining() const
    {
       return Utils::allElementsEqual(m_coordinatesToUnit.cbegin(), m_coordinatesToUnit.cend(), [](const auto& lhs, const auto& rhs)
                                {
                                    return lhs.second->getFaction() == rhs.second->getFaction();
                                });
    }
};

GameManager parseGameManager(const std::vector<std::string>& mapLines, int elfAttackPower = DEFAULT_ATTACK_POWER)
{
    WallGrid wallGrid;
    CoordinatesToUnit coordinatesToUnit;

    for (int j = 0; j < mapLines.size(); ++j)
    {
        WallsGridLine wallGridLine;
        for (int i = 0; i < mapLines[j].size(); ++i)
        {
            char field = mapLines[j][i];
            wallGridLine.push_back(field == '#');

            if (field == 'E' || field == 'G')
            {
                Coordinates coordinates{j, i};
                bool isElf = (field == 'E');
                int attackPower = isElf ? elfAttackPower : DEFAULT_ATTACK_POWER;

                UnitSharedPtr unitSharedPtr = std::make_shared<Unit>(isElf, coordinates, attackPower);

                coordinatesToUnit.emplace(std::move(coordinates), std::move(unitSharedPtr));
            }
        }
        wallGrid.push_back(std::move(wallGridLine));
    }

    return GameManager{std::move(wallGrid), std::move(coordinatesToUnit)};
}


unsigned outcomeOfCombat(const std::vector<std::string>& mapLines)
{
    GameManager gameManager = parseGameManager(mapLines);

    gameManager.play();

    return gameManager.getNumTurnsElapsed() * gameManager.getTotalHitPointsRemaining();
}

unsigned outcomeOfCombatIfElvesBarelyWin(const std::vector<std::string>& mapLines)
{
    unsigned elfAttackPower = DEFAULT_ATTACK_POWER;

    while (true)
    {
        ++elfAttackPower;
        GameManager gameManager = parseGameManager(mapLines, elfAttackPower);

        unsigned numTotalElves = gameManager.getNumElvesAlive();

        gameManager.play();

        if (gameManager.getNumElvesAlive() == numTotalElves)
        {
            return gameManager.getNumTurnsElapsed() * gameManager.getTotalHitPointsRemaining();
        }
    }
}

}
