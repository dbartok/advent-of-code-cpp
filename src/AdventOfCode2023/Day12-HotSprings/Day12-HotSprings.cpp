#include "Day12-HotSprings.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

#include <numeric>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned SECOND_PART_FOLD_FACTOR = 5;

const char OPERATIONAL = '.';
const char DAMAGED = '#';
const char UNKNOWN = '?';

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day12
{

class SpringConditionSolver
{
public:
    SpringConditionSolver(std::string springConditions, std::vector<int> damagedSpringGroupSizes)
        : m_springConditions{std::move(springConditions)}
        , m_damagedSpringGroupSizes{std::move(damagedSpringGroupSizes)}
    {

    }

    int64_t getNumPossibleArragements()
    {
        return getNumPossibleArragementsRecursiveMemoized(0, 0, 0);
    }

private:
    using MemoArgs = std::tuple<size_t, size_t, size_t>;

    std::string m_springConditions;
    std::vector<int> m_damagedSpringGroupSizes;

    std::unordered_map<MemoArgs, int64_t, boost::hash<MemoArgs>> m_argsToMemoizedResult;

    int64_t getNumPossibleArragementsRecursiveMemoized(size_t currentPosition, size_t currentGroupIndex, size_t currentGroupExistingSize)
    {
        MemoArgs memoArgs{currentPosition, currentGroupIndex, currentGroupExistingSize};
        const auto findResult = m_argsToMemoizedResult.find(memoArgs);
        if (findResult != m_argsToMemoizedResult.cend())
        {
            return findResult->second;
        }
        else
        {
            const int64_t result = getNumPossibleArragementsRecursive(currentPosition, currentGroupIndex, currentGroupExistingSize);
            m_argsToMemoizedResult.emplace(std::move(memoArgs), result);
            return result;
        }
    }

    int64_t getNumPossibleArragementsRecursive(size_t currentPosition, size_t currentGroupIndex, size_t currentGroupExistingSize)
    {
        if (currentPosition == m_springConditions.size())
        {
            const bool wereAllGroupsFoundAndNoNewGroupsStarted = (currentGroupIndex == m_damagedSpringGroupSizes.size() && currentGroupExistingSize == 0);
            const bool wereAllButLastGroupFoundAndCurrentGroupIsLastGroup = (currentGroupIndex == m_damagedSpringGroupSizes.size() - 1 && currentGroupExistingSize == m_damagedSpringGroupSizes.at(currentGroupIndex));

            if (wereAllGroupsFoundAndNoNewGroupsStarted || wereAllButLastGroupFoundAndCurrentGroupIsLastGroup)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        int64_t numPossibleArragements = 0;
        const char currentSpringCondition = m_springConditions.at(currentPosition);

        if (currentSpringCondition == OPERATIONAL || currentSpringCondition == UNKNOWN)
        {
            if (currentGroupExistingSize == 0)
            {
                numPossibleArragements += getNumPossibleArragementsRecursiveMemoized(currentPosition + 1, currentGroupIndex, currentGroupExistingSize);
            }
            else if (currentGroupExistingSize == m_damagedSpringGroupSizes.at(currentGroupIndex))
            {
                numPossibleArragements += getNumPossibleArragementsRecursiveMemoized(currentPosition + 1, currentGroupIndex + 1, 0);
            }
        }

        if (currentSpringCondition == DAMAGED || currentSpringCondition == UNKNOWN)
        {
            if (currentGroupIndex < m_damagedSpringGroupSizes.size() && currentGroupExistingSize <= m_damagedSpringGroupSizes.at(currentGroupIndex))
            {
                numPossibleArragements += getNumPossibleArragementsRecursiveMemoized(currentPosition + 1, currentGroupIndex, currentGroupExistingSize + 1);
            }
        }

        return numPossibleArragements;
    }
};

SpringConditionSolver parseSpringConditionsLine(const std::string& springConditionsLine, unsigned foldFactor)
{
    std::vector<std::string> tokens;
    boost::split(tokens, springConditionsLine, boost::is_any_of(" "));

    std::vector<int> damagedSpringGroupSizes;
    AdventOfCode::Utils::splitStringIntoTypedVector(tokens.at(1), damagedSpringGroupSizes, ',');

    std::string unfoldedSpringConditions;
    std::vector<int> unfoldedDamagedSpringGroupSizes;

    for (unsigned i = 1; i <= foldFactor; ++i)
    {
        unfoldedSpringConditions += tokens.at(0);
        if (i != foldFactor)
        {
            unfoldedSpringConditions += "?";
        }

        std::copy(damagedSpringGroupSizes.cbegin(), damagedSpringGroupSizes.cend(), std::back_inserter(unfoldedDamagedSpringGroupSizes));
    }

    return {std::move(unfoldedSpringConditions), std::move(unfoldedDamagedSpringGroupSizes)};
}

std::vector<SpringConditionSolver> parseSpringConditionsLines(const std::vector<std::string>& springConditionsLines, unsigned foldFactor = 1)
{
    std::vector<SpringConditionSolver> springConditionSolvers;

    for (const auto& springConditionsLine : springConditionsLines)
    {
        SpringConditionSolver springConditionSolver = parseSpringConditionsLine(springConditionsLine, foldFactor);
        springConditionSolvers.push_back(std::move(springConditionSolver));
    }

    return springConditionSolvers;
}

int64_t sumOfAllPossibleArrangements(const std::vector<std::string>& springConditionsLines)
{
    std::vector<SpringConditionSolver> springConditionSolvers = parseSpringConditionsLines(springConditionsLines);

    return std::accumulate(springConditionSolvers.begin(), springConditionSolvers.end(), 0ll, [](int64_t acc, auto& solver)
                           {
                               return acc + solver.getNumPossibleArragements();
                           });
}

int64_t sumOfAllPossibleArrangementsUnfolded(const std::vector<std::string>& springConditionsLines)
{
    std::vector<SpringConditionSolver> springConditionSolvers = parseSpringConditionsLines(springConditionsLines, SECOND_PART_FOLD_FACTOR);

    return std::accumulate(springConditionSolvers.begin(), springConditionSolvers.end(), 0ll, [](int64_t acc, auto& solver)
                           {
                               return acc + solver.getNumPossibleArragements();
                           });
}

}
}
}