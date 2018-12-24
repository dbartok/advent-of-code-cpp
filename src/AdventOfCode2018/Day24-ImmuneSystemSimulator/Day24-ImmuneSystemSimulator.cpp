#include "Day24-ImmuneSystemSimulator.h"

#include <AdventOfCodeCommon/Utils.h>
#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using DamageType = std::string;
using DamageTypes = std::unordered_set<std::string>;

enum class Faction
{
    IMMUNE_SYSTEM,
    INFECTION,
};

using UnitGroupIDType = int;

class UnitGroup
{
public:
    UnitGroup(
        Faction faction,
        int numUnits,
        int hitPoints,
        DamageTypes weaknesses,
        DamageTypes immunities,
        int attackDamage,
        DamageType attackType,
        int initiative)
        : m_faction{faction}
        , m_numUnits{numUnits}
        , m_hitPoints{hitPoints}
        , m_weaknesses{std::move(weaknesses)}
        , m_immunities{std::move(immunities)}
        , m_attackDamage{attackDamage}
        , m_attackType{std::move(attackType)}
        , m_initiative{initiative}
    {

    }

    int getInitiative() const
    {
        return m_initiative;
    }

    UnitGroupIDType getID() const
    {
        // initiative is also a unique ID
        return m_initiative;
    }

    int getNumUnits() const
    {
        return m_numUnits;
    }

    Faction getFaction() const
    {
        return m_faction;
    }

    bool isHostileTowards(const UnitGroup& other) const
    {
        return m_faction != other.m_faction;
    }

    bool wouldDealDamageTo(const UnitGroup& target) const
    {
        return target.m_immunities.find(m_attackType) == target.m_immunities.end();
    }

    int getEffectivePower() const
    {
        return m_numUnits * m_attackDamage;
    }

    int numPossibleDamageTo(const UnitGroup& target) const
    {
        if (!wouldDealDamageTo(target))
        {
            throw std::runtime_error("Cannot calculate possible damage because target is immune.");
        }

        int damage = getEffectivePower();

        if (target.m_weaknesses.find(m_attackType) != target.m_weaknesses.end())
        {
            damage *= 2;
        }

        return damage;
    }

    bool isEmpty() const
    {
        return m_numUnits <= 0;
    }

    void attack(UnitGroup& target) const
    {
        int damage = numPossibleDamageTo(target);
        int killedUnits = damage / target.m_hitPoints;
        target.m_numUnits -= killedUnits;
    }

private:
    Faction m_faction;
    int m_numUnits;
    int m_hitPoints;
    DamageTypes m_weaknesses;
    DamageTypes m_immunities;
    int m_attackDamage;
    DamageType m_attackType;
    int m_initiative;
};

class UnitGroupsParser
{
public:
    UnitGroupsParser(std::vector<std::string> armiesLines)
        : m_armiesLines{std::move(armiesLines)}
    {

    }

    void parse()
    {
        while (m_nextLineIndex < m_armiesLines.size())
        {
            std::string currentLine = m_armiesLines[m_nextLineIndex++];

            if (currentLine.empty())
            {
                m_currentFaction = Faction::INFECTION;
                ++m_nextLineIndex;
            }
            else
            {
                parseArmyLine(currentLine);
            }
        }
    }

    std::vector<UnitGroup> getUnitGroups() const
    {
        return m_unitGroups;
    }

private:
    std::vector<std::string> m_armiesLines;
    size_t m_nextLineIndex = 1;
    Faction m_currentFaction = Faction::IMMUNE_SYSTEM;

    std::vector<UnitGroup> m_unitGroups;

    void parseArmyLine(const std::string& armyLine)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, armyLine, boost::is_any_of(" "));
        int numUnits = std::stoi(tokens[0]);
        int hitPoints = std::stoi(tokens[4]);
        DamageTypes weaknesses = parseDamageTypes(armyLine, "weak");
        DamageTypes immunities = parseDamageTypes(armyLine, "immune");
        int attackDamage = std::stoi(*(tokens.crbegin() + 5));
        DamageType attackType = *(tokens.crbegin() + 4);
        int initiative = std::stoi(*tokens.crbegin());

        m_unitGroups.emplace_back(m_currentFaction, numUnits, hitPoints, weaknesses, immunities, attackDamage, attackType, initiative);
    }

    DamageTypes parseDamageTypes(const std::string& armyLine, const std::string& name)
    {
        std::regex damageTypeRegex(name + R"( to ((?:[a-z]+(?:, )?)+)[;\)])");
        std::smatch matchResults;
        std::regex_search(armyLine, matchResults, damageTypeRegex);

        if (matchResults.size() == 0)
        {
            return {};
        }

        std::string damageTypesString = matchResults[1];
        DamageTypes damageTypes;
        boost::split(damageTypes, damageTypesString, boost::is_any_of(" ,"), boost::token_compress_on);

        return damageTypes;
    }
};

using UnitGroupIDToUnitGroupIDMap = std::unordered_map<UnitGroupIDType, UnitGroupIDType>;
using UnitGroupIDToUnitGroupMap = std::unordered_map<UnitGroupIDType, UnitGroup>;

struct Targeting
{
    UnitGroupIDType attackerID;
    UnitGroupIDType targetID;

    Targeting(UnitGroupIDType attackerID, UnitGroupIDType targetID)
        : attackerID{attackerID}
        , targetID{targetID}
    {

    }
};


class ImmuneFightSimulator
{
public:
    ImmuneFightSimulator(std::vector<UnitGroup> unitGroups)
        : m_unitGroupMap{}
    {
        for (auto& unitGroup : unitGroups)
        {
            UnitGroupIDType unitGroupID = unitGroup.getID();
            m_unitGroupMap.emplace(unitGroupID, std::move(unitGroup));
        }
    }

    void run()
    {
        while (!isOnlyOneFactionRemaining())
        {
            std::vector<Targeting> targetings = selectTargets();

            executeAttacks(targetings);

            removeEmptyUnitGroups();
        }
    }

    unsigned getTotalRemainingNumUnits() const
    {
        return std::accumulate(m_unitGroupMap.cbegin(), m_unitGroupMap.cend(), 0u, [](int acc, const auto& elem)
                               {
                                   return acc + elem.second.getNumUnits();
                               });
    }

private:
    UnitGroupIDToUnitGroupMap m_unitGroupMap;

    bool isOnlyOneFactionRemaining() const
    {
        return Utils::allElementsEqual(m_unitGroupMap.cbegin(), m_unitGroupMap.cend(), [](const auto& lhs, const auto& rhs)
                                       {
                                           return lhs.second.getFaction() == rhs.second.getFaction();
                                       });
    }

    std::vector<Targeting> selectTargets()
    {
        std::vector<UnitGroupIDType> targetingOrder = getTargetingOrder();

        UnitGroupIDToUnitGroupIDMap targetToAttacker;

        for (const auto attackerID : targetingOrder)
        {
            const UnitGroup& attacker = m_unitGroupMap.at(attackerID);
            auto target = selectTarget(attacker, targetToAttacker);

            if (target.is_initialized())
            {
                targetToAttacker.emplace(target.get().getID(), attacker.getID());
            }
        }

        std::vector<Targeting> targetings;

        for (const auto& targetToAttackerElem : targetToAttacker)
        {
            targetings.emplace_back(targetToAttackerElem.second, targetToAttackerElem.first);
        };

        return targetings;
    }

    void removeEmptyUnitGroups()
    {
        for (auto iter = m_unitGroupMap.begin(); iter != m_unitGroupMap.end();)
        {
            if (iter->second.isEmpty())
            {
                iter = m_unitGroupMap.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }

    void executeAttacks(std::vector<Targeting> targetings)
    {
        std::sort(targetings.begin(), targetings.end(), [this](const auto& lhs, const auto& rhs)
                  {
                      const auto& lhsAttacker = this->m_unitGroupMap.at(lhs.attackerID);
                      const auto& rhsAttacker = this->m_unitGroupMap.at(rhs.attackerID);

                      return lhsAttacker.getInitiative() > rhsAttacker.getInitiative();
                  });

        for (const auto& targeting : targetings)
        {
            const UnitGroup& attacker = m_unitGroupMap.at(targeting.attackerID);
            UnitGroup& target = m_unitGroupMap.at(targeting.targetID);

            if (!attacker.isEmpty())
            {
                attacker.attack(target);
            }
        }
    }

    std::vector<UnitGroupIDType> getTargetingOrder()
    {
        std::vector<UnitGroupIDType> targetingOrder;

        for (const auto& elem : m_unitGroupMap)
        {
            targetingOrder.push_back(elem.first);
        }

        std::sort(targetingOrder.begin(), targetingOrder.end(), [this](const auto& lhs, const auto& rhs)
                  {
                      const UnitGroup& lhsUnitGroup = this->m_unitGroupMap.at(lhs);
                      const UnitGroup& rhsUnitGroup = this->m_unitGroupMap.at(rhs);

                      if (lhsUnitGroup.getEffectivePower() != rhsUnitGroup.getEffectivePower())
                      {
                          return lhsUnitGroup.getEffectivePower() > rhsUnitGroup.getEffectivePower();
                      }

                      return lhsUnitGroup.getInitiative() > rhsUnitGroup.getInitiative();
                  });

        return targetingOrder;
    }


    boost::optional<UnitGroup> selectTarget(const UnitGroup& attacker, const UnitGroupIDToUnitGroupIDMap& targetToAttacker)
    {
        boost::optional<UnitGroup> bestTarget;

        for (const auto& potentialTargetElem : m_unitGroupMap)
        {
            const UnitGroup& potentialTarget = potentialTargetElem.second;

            if (targetToAttacker.find(potentialTarget.getID()) != targetToAttacker.end())
            {
                continue;
            }

            bool isValidTarget = attacker.isHostileTowards(potentialTarget) && attacker.wouldDealDamageTo(potentialTarget);
            if (!isValidTarget)
            {
                continue;
            }

            if (isBetterTarget(attacker, bestTarget, potentialTarget))
            {
                bestTarget = potentialTarget;
            }
        }

        return bestTarget;
    }

    bool isBetterTarget(const UnitGroup& attacker, const boost::optional<UnitGroup>& bestTargetOpt, const UnitGroup& potentialTarget)
    {
        if (!bestTargetOpt.is_initialized())
        {
            return true;
        }

        const auto& bestTarget = bestTargetOpt.get();

        int potentialDamage = attacker.numPossibleDamageTo(potentialTarget);
        int bestDamage = attacker.numPossibleDamageTo(bestTarget);
        if (potentialDamage != bestDamage)
        {
            return potentialDamage > bestDamage;
        }

        int potentialEffectivePower = potentialTarget.getEffectivePower();
        int bestEffectivePower = bestTarget.getEffectivePower();
        if (potentialEffectivePower != bestEffectivePower)
        {
            return potentialEffectivePower > bestEffectivePower;
        }

        return potentialTarget.getInitiative() > bestTarget.getInitiative();
    }
};

unsigned numUnitsOfWinningArmy(const std::vector<std::string>& armiesLines)
{
    UnitGroupsParser unitGroupsParser(armiesLines);
    unitGroupsParser.parse();

    ImmuneFightSimulator immuneFightSimulator{unitGroupsParser.getUnitGroups()};
    immuneFightSimulator.run();

    return immuneFightSimulator.getTotalRemainingNumUnits();
}

}
