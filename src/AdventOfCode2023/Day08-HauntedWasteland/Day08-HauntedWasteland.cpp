#include "Day08-HauntedWasteland.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/math/common_factor_rt.hpp>

#include <unordered_map>
#include <regex>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const std::string FIRST_PART_START_NODE_ID_REGEX = "AAA";
const std::string FIRST_PART_END_NODE_ID_REGEX = "ZZZ";

const std::string SECOND_PART_START_NODE_ID_REGEX = "..A";
const std::string SECOND_PART_END_NODE_ID_REGEX = "..Z";

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day08
{

using NodeID = std::string;

struct Junction
{
    NodeID left;
    NodeID right;
};

using NodeIDToJunction = std::unordered_map<NodeID, Junction>;

class NetworkTraverser
{
public:
    NetworkTraverser(std::string instructions, NodeIDToJunction nodeIDToJunction, NodeID startNodeID, std::string endNodeIDRegex)
        : m_instructions{std::move(instructions)}
        , m_nodeIDToJunction{std::move(nodeIDToJunction)}
        , m_currentNodeID{std::move(startNodeID)}
        , m_endNodeIDRegex{std::move(endNodeIDRegex)}
    {

    }

    int getNumSteps() const
    {
        return m_numSteps;
    }

    void traverse()
    {
        while (!isAtEndpoint())
        {
            executeSingleInstruction();
        }
    }

private:
    const std::string m_instructions;
    const NodeIDToJunction m_nodeIDToJunction;
    const std::string m_endNodeIDRegex;

    NodeID m_currentNodeID;
    int m_numSteps = 0;

    void executeSingleInstruction()
    {
        const char currentInstruction = m_instructions.at(m_numSteps % m_instructions.size());
        const Junction& currentJunction = m_nodeIDToJunction.at(m_currentNodeID);

        if (currentInstruction == 'L')
        {
            m_currentNodeID = currentJunction.left;
        }
        else if (currentInstruction == 'R')
        {
            m_currentNodeID = currentJunction.right;
        }

        ++m_numSteps;
    }

    bool isAtEndpoint() const
    {
        return std::regex_match(m_currentNodeID, std::regex{m_endNodeIDRegex});
    }
};

NodeIDToJunction::value_type parseNetworkLine(const std::string& networkLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, networkLine, boost::is_any_of(" ,=()"), boost::token_compress_on);

    return {std::move(tokens.at(0)), {std::move(tokens.at(1)), std::move(tokens.at(2))}};
}

std::string createInstructions(const std::vector<std::string>& mapLines)
{
    return mapLines.at(0);
}

NodeIDToJunction createNodeIDToJunction(const std::vector<std::string>& mapLines)
{
    NodeIDToJunction nodeIDToJunction;
    for (int i = 2; i < mapLines.size(); ++i)
    {
        NodeIDToJunction::value_type nodeIDAndJunction = parseNetworkLine(mapLines.at(i));
        nodeIDToJunction.insert(std::move(nodeIDAndJunction));
    }
    return nodeIDToJunction;
}

std::vector<NodeID> getStartNodeIDs(const NodeIDToJunction& nodeIDToJunction, const std::string& startNodeIDsRegex)
{
    std::vector<NodeID> startNodeIDs;

    for (const auto& nodeIDAndJunction : nodeIDToJunction)
    {
        const NodeID& nodeID = nodeIDAndJunction.first;
        if (std::regex_match(nodeID, std::regex{startNodeIDsRegex}))
        {
            startNodeIDs.push_back(nodeID);
        }
    }

    return startNodeIDs;
}

int64_t numStepsRequiredToReachEndpoints(const std::vector<std::string>& mapLines, const std::string& startNodeIDsRegex, const std::string& endNodeIDsRegex)
{
    std::string instruction = createInstructions(mapLines);
    NodeIDToJunction nodeIDToJunction = createNodeIDToJunction(mapLines);

    std::vector<NodeID> startNodeIDs = getStartNodeIDs(nodeIDToJunction, startNodeIDsRegex);

    std::vector<int> cycleTimes;

    for (const NodeID& startNodeID : startNodeIDs)
    {
        NetworkTraverser networkTraverser{instruction, nodeIDToJunction, startNodeID, endNodeIDsRegex};
        networkTraverser.traverse();

        cycleTimes.push_back(networkTraverser.getNumSteps());
    }

    return std::accumulate(cycleTimes.cbegin(), cycleTimes.cend(), 1ll, [](int64_t total, int64_t current)
                           {
                               return boost::math::lcm(total, current);
                           });
}

int64_t numStepsRequiredToReachEndpoint(const std::vector<std::string>& mapLines)
{
    return numStepsRequiredToReachEndpoints(mapLines, FIRST_PART_START_NODE_ID_REGEX, FIRST_PART_END_NODE_ID_REGEX);
}

int64_t numStepsRequiredToReachAllEndpointsSimultaneously(const std::vector<std::string>& mapLines)
{
    return numStepsRequiredToReachEndpoints(mapLines, SECOND_PART_START_NODE_ID_REGEX, SECOND_PART_END_NODE_ID_REGEX);
}

}
}
}