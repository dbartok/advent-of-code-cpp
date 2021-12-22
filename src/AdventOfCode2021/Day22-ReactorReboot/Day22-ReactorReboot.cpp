#include "Day22-ReactorReboot.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include <regex>
#include <map>
#include <cassert>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int INITIALIZATION_AREA_SIZE = 50;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day22
{

using CubeState = bool;
using Interval = std::pair<int, int>;

struct RebootStep
{
    CubeState state;
    std::vector<Interval> intervalDimensions;

    bool isInitializationStep() const
    {
        return std::all_of(intervalDimensions.cbegin(), intervalDimensions.cend(), [](const auto& intervalDimension)
                           {
                               return intervalDimension.first >= -INITIALIZATION_AREA_SIZE && intervalDimension.first <= INITIALIZATION_AREA_SIZE &&
                                   intervalDimension.second >= -INITIALIZATION_AREA_SIZE && intervalDimension.second <= INITIALIZATION_AREA_SIZE;
                           });
    }
};

struct Node
{
public:
    void insertRebootStep(const RebootStep& rebootStep)
    {
        assert(!rebootStep.intervalDimensions.empty());

        const Interval& currentInterval = rebootStep.intervalDimensions.front();

        splitAt(currentInterval.first);
        splitAt(currentInterval.second + 1);

        std::vector<Node*> nodesInRange = getNodesInRange(currentInterval);

        std::vector<Interval> reducedIntervalDimensions = {rebootStep.intervalDimensions.cbegin() + 1, rebootStep.intervalDimensions.cend()};
        for (auto nodeToModify : nodesInRange)
        {
            if (reducedIntervalDimensions.empty())
            {
                nodeToModify->m_state = rebootStep.state;
            }
            else
            {
                assert(!nodeToModify->m_state.has_value());
                nodeToModify->insertRebootStep(RebootStep{rebootStep.state, reducedIntervalDimensions});
            }
        }
    }

    uint64_t getNumCubesOn() const
    {
        assert(!m_state.has_value());
        return getNumCubesOnRecursive(1);
    }

private:
    std::map<int, Node> m_intervals;
    boost::optional<CubeState> m_state;

    void splitAt(int threshold)
    {
        auto upperBoundIter = m_intervals.upper_bound(threshold);
        Node newNode;

        // This isn't the first node or last node
        if (upperBoundIter != m_intervals.cbegin())
        {
            const auto prevNodeIter = std::prev(upperBoundIter);
            newNode = prevNodeIter->second;
        }

        m_intervals.emplace(threshold, std::move(newNode));
    }

    std::vector<Node*> getNodesInRange(const Interval& interval)
    {
        std::vector<Node*> nodesInRange;

        const auto beginIter = m_intervals.find(interval.first);
        const auto endIter = m_intervals.find(interval.second + 1);

        assert(beginIter != m_intervals.cend() && endIter != m_intervals.cend());

        for (auto nodeIter = beginIter; nodeIter != endIter; ++nodeIter)
        {
            nodesInRange.push_back(&(nodeIter->second));
        }

        return nodesInRange;
    }

    uint64_t getNumCubesOnRecursive(uint64_t multiplyingFactor) const
    {
        if (m_intervals.empty())
        {
            return 0;
        }
        assert(m_intervals.size() != 1);

        uint64_t totalNumCubesOn = 0;

        for (auto nodeIter = m_intervals.cbegin(); std::next(nodeIter) != m_intervals.cend(); ++nodeIter)
        {
            const int intervalLength = std::next(nodeIter)->first - nodeIter->first;
            const Node& childNode = nodeIter->second;
            if (childNode.m_state.has_value())
            {
                assert(childNode.m_intervals.empty());
                assert(intervalLength > 0);
                totalNumCubesOn += multiplyingFactor * intervalLength * childNode.m_state.value();
            }
            else
            {
                auto multiplyingFactorCopy{multiplyingFactor * intervalLength};
                totalNumCubesOn += childNode.getNumCubesOnRecursive(multiplyingFactorCopy);
            }
        }

        return totalNumCubesOn;
    }
};

class SubmarineReactorRebooter
{
public:

    SubmarineReactorRebooter(std::vector<RebootStep> rebootSteps)
        : m_rebootSteps{std::move(rebootSteps)}
        , m_rootNode{}
    {

    }

    void executeRebootInitializationProcedure()
    {
        for (const auto& rebootStep : m_rebootSteps)
        {
            if (rebootStep.isInitializationStep())
            {
                executeRebootStep(rebootStep);
            }
        }
    }

    void executeRebootFullRebootProcedure()
    {
        for (const auto& rebootStep : m_rebootSteps)
        {
            m_rootNode.insertRebootStep(rebootStep);
        }
    }

    uint64_t getNumCubesOn() const
    {
        return m_rootNode.getNumCubesOn();
    }

private:
    std::vector<RebootStep> m_rebootSteps;

    Node m_rootNode;

    void executeRebootStep(const RebootStep& rebootStep)
    {
        m_rootNode.insertRebootStep(rebootStep);
    }
};

RebootStep parseRebootStepLine(const std::string& rebootStepLine)
{
    std::regex rebootStepLineRegex(R"(((?:on)|(?:off)) x=(-?[0-9]+)..(-?[0-9]+),y=(-?[0-9]+)..(-?[0-9]+),z=(-?[0-9]+)..(-?[0-9]+))");
    std::smatch rebootStepLineMatch;
    std::regex_match(rebootStepLine, rebootStepLineMatch, rebootStepLineRegex);

    const bool state = rebootStepLineMatch[1].str() == "on";
    const int fromX = std::stoi(rebootStepLineMatch[2].str());
    const int toX = std::stoi(rebootStepLineMatch[3].str());
    const int fromY = std::stoi(rebootStepLineMatch[4].str());
    const int toY = std::stoi(rebootStepLineMatch[5].str());
    const int fromZ = std::stoi(rebootStepLineMatch[6].str());
    const int toZ = std::stoi(rebootStepLineMatch[7].str());

    Interval intervalX{fromX, toX};
    Interval intervalY{fromY, toY};
    Interval intervalZ{fromZ, toZ};

    return RebootStep{state, {std::move(intervalX), std::move(intervalY), std::move(intervalZ)}};
}

std::vector<RebootStep> parseRebootStepLines(const std::vector<std::string>& rebootStepLines)
{
    std::vector<RebootStep> rebootSteps;

    for (const auto& line : rebootStepLines)
    {
        RebootStep rebootStep = parseRebootStepLine(line);
        rebootSteps.push_back(std::move(rebootStep));
    }

    return rebootSteps;
}

uint64_t numCubesOnAfterInititializationProcedure(const std::vector<std::string>& rebootStepLines)
{
    std::vector<RebootStep> rebootSteps = parseRebootStepLines(rebootStepLines);
    SubmarineReactorRebooter rebooter{std::move(rebootSteps)};
    rebooter.executeRebootInitializationProcedure();
    return rebooter.getNumCubesOn();
}

uint64_t numCubesOnAfterFullRebootProcedure(const std::vector<std::string>& rebootStepLines)
{
    std::vector<RebootStep> rebootSteps = parseRebootStepLines(rebootStepLines);
    SubmarineReactorRebooter rebooter{std::move(rebootSteps)};
    rebooter.executeRebootFullRebootProcedure();
    return rebooter.getNumCubesOn();
}

}
}
}