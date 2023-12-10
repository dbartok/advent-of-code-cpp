#include "Day08-HauntedWasteland.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const std::string START_NODE_ID = "AAA";
const std::string END_NODE_ID = "ZZZ";

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
    NetworkTraverser(std::string instructions, NodeIDToJunction nodeIDToJunction)
        : m_instructions{std::move(instructions)}
        , m_nodeIDToJunction{std::move(nodeIDToJunction)}
    {

    }

    void traverse()
    {
        while (!isFinished())
        {
            executeSingleInstruction();
        }
    }

    int getNumSteps() const
    {
        return m_numSteps;
    }

private:
    const std::string m_instructions;
    const NodeIDToJunction m_nodeIDToJunction;

    std::string m_currentNodeID = START_NODE_ID;
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

    bool isFinished() const
    {
        return m_currentNodeID == END_NODE_ID;
    }
};

NodeIDToJunction::value_type parseNetworkLine(const std::string& networkLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, networkLine, boost::is_any_of(" ,=()"), boost::token_compress_on);

    return {std::move(tokens.at(0)), {std::move(tokens.at(1)), std::move(tokens.at(2))}};
}

NetworkTraverser parseMapLines(const std::vector<std::string>& mapLines)
{
    std::string instructions = mapLines.at(0);

    NodeIDToJunction nodeIDToJunction;
    for (int i = 2; i < mapLines.size(); ++i)
    {
        NodeIDToJunction::value_type nodeIDAndJunction = parseNetworkLine(mapLines.at(i));
        nodeIDToJunction.insert(std::move(nodeIDAndJunction));
    }
    return {std::move(instructions), std::move(nodeIDToJunction)};
}

int numStepsRequiredToReachEndpoint(const std::vector<std::string>& mapLines)
{
    NetworkTraverser networkTraverser = parseMapLines(mapLines);

    networkTraverser.traverse();

    return networkTraverser.getNumSteps();
}

}
}
}