#include "Day12-HotSprings.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

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

    int getNumPossibleArragements() const
    {
        return getNumPossibleArragementsRecursive(0, 0, 0);
    }

private:
    std::string m_springConditions;
    std::vector<int> m_damagedSpringGroupSizes;

    int getNumPossibleArragementsRecursive(size_t currentPosition, size_t currentGroupIndex, size_t currentGroupExistingSize) const
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

        int numPossibleArragements = 0;
        const char currentSpringCondition = m_springConditions.at(currentPosition);

        if (currentSpringCondition == OPERATIONAL || currentSpringCondition == UNKNOWN)
        {
            if (currentGroupExistingSize == 0)
            {
                numPossibleArragements += getNumPossibleArragementsRecursive(currentPosition + 1, currentGroupIndex, currentGroupExistingSize);
            }
            else if (currentGroupExistingSize == m_damagedSpringGroupSizes.at(currentGroupIndex))
            {
                numPossibleArragements += getNumPossibleArragementsRecursive(currentPosition + 1, currentGroupIndex + 1, 0);
            }
        }

        if (currentSpringCondition == DAMAGED || currentSpringCondition == UNKNOWN)
        {
            if (currentGroupIndex < m_damagedSpringGroupSizes.size() && currentGroupExistingSize <= m_damagedSpringGroupSizes.at(currentGroupIndex))
            {
                numPossibleArragements += getNumPossibleArragementsRecursive(currentPosition + 1, currentGroupIndex, currentGroupExistingSize + 1);
            }
        }

        return numPossibleArragements;
    }
};

SpringConditionSolver parseSpringConditionsLine(const std::string& springConditionsLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, springConditionsLine, boost::is_any_of(" "));

    std::vector<int> damagedSpringGroupSizes;
    AdventOfCode::Utils::splitStringIntoTypedVector(tokens.at(1), damagedSpringGroupSizes, ',');

    return {std::move(tokens.at(0)), std::move(damagedSpringGroupSizes)};
}

std::vector<SpringConditionSolver> parseSpringConditionsLines(const std::vector<std::string>& springConditionsLines)
{
    std::vector<SpringConditionSolver> springConditionSolvers;

    for (const auto& springConditionsLine : springConditionsLines)
    {
        SpringConditionSolver springConditionSolver = parseSpringConditionsLine(springConditionsLine);
        springConditionSolvers.push_back(std::move(springConditionSolver));
    }

    return springConditionSolvers;
}

int sumOfAllPossibleArrangements(const std::vector<std::string>& springConditionsLines)
{
    std::vector<SpringConditionSolver> springConditionSolvers = parseSpringConditionsLines(springConditionsLines);

    return std::accumulate(springConditionSolvers.cbegin(), springConditionSolvers.cend(), 0, [](int acc, const auto& solver)
                           {
                               return acc + solver.getNumPossibleArragements();
                           });
}

}
}
}