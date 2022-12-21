#include "Day21-MonkeyMath.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <memory>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const char* ROOT_NODE_NAME = "root";

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day21
{

class MonkeyNode;
using MonkeyNodeSharedPtr = std::shared_ptr<MonkeyNode>;

class MonkeyNode
{
public:
    MonkeyNode(std::string name)
        : m_name{std::move(name)}
    {

    }

    virtual void setOperands(MonkeyNodeSharedPtr leftOperand, MonkeyNodeSharedPtr rightOperand) = 0;
    virtual int64_t evaluate() const = 0;

private:
    std::string m_name;
};

class IntegerNode : public MonkeyNode
{
public:
    IntegerNode(std::string name, int64_t value)
        : MonkeyNode{std::move(name)}
        , m_value(value)
    {

    }

    void setOperands(MonkeyNodeSharedPtr leftOperand, MonkeyNodeSharedPtr rightOperand) override
    {
        throw std::runtime_error("Cannot set operands on an integer node");
    }

    int64_t evaluate() const override
    {
        return m_value;
    }

private:
    int64_t m_value;
};

class TwoOperandNode : public MonkeyNode
{
public:
    using MonkeyNode::MonkeyNode;

    void setOperands(MonkeyNodeSharedPtr leftOperand, MonkeyNodeSharedPtr rightOperand) override
    {
        m_leftOperand = std::move(leftOperand);
        m_rightOperand = std::move(rightOperand);
    }

protected:
    MonkeyNodeSharedPtr m_leftOperand = nullptr;
    MonkeyNodeSharedPtr m_rightOperand = nullptr;
};

class AdditionNode : public TwoOperandNode
{
public:
    using TwoOperandNode::TwoOperandNode;

    int64_t evaluate() const override
    {
        return m_leftOperand->evaluate() + m_rightOperand->evaluate();
    }
};

class SubtractionNode : public TwoOperandNode
{
public:
    using TwoOperandNode::TwoOperandNode;

    int64_t evaluate() const override
    {
        return m_leftOperand->evaluate() - m_rightOperand->evaluate();
    }
};

class MultiplicationNode : public TwoOperandNode
{
public:
    using TwoOperandNode::TwoOperandNode;

    int64_t evaluate() const override
    {
        return m_leftOperand->evaluate() * m_rightOperand->evaluate();
    }
};

class DivisionNode : public TwoOperandNode
{
public:
    using TwoOperandNode::TwoOperandNode;

    int64_t evaluate() const override
    {
        return m_leftOperand->evaluate() / m_rightOperand->evaluate();
    }
};

class MonkeyParser
{
public:
    MonkeyParser(const std::vector<std::string>& monkeyLines)
    {
        for (const auto& monkeyLine : monkeyLines)
        {
            std::vector<std::string> tokens;
            boost::split(tokens, monkeyLine, boost::is_any_of(": "), boost::token_compress_on);

            m_allMonkeyTokens.push_back(std::move(tokens));
        }
    }

    void parse()
    {
        createAllNodes();
        setAllChildrenNodes();
    }

    MonkeyNodeSharedPtr getRootNode() const
    {
        return m_monkeyNameToNode.at(ROOT_NODE_NAME);
    }

private:
    using Tokens = std::vector<std::string>;

    std::vector<Tokens> m_allMonkeyTokens;

    std::unordered_map<std::string, MonkeyNodeSharedPtr> m_monkeyNameToNode;

    void createAllNodes()
    {
        for (const auto& monkeyTokens : m_allMonkeyTokens)
        {
            createAndStoreNode(monkeyTokens);
        }
    }

    void setAllChildrenNodes()
    {
        for (const auto& monkeyTokens : m_allMonkeyTokens)
        {
            setChildrenNodes(monkeyTokens);
        }
    }

    void createAndStoreNode(const Tokens& monkeyTokens)
    {
        MonkeyNodeSharedPtr node = createNode(monkeyTokens);
        m_monkeyNameToNode.emplace(getName(monkeyTokens), std::move(node));
    }

    MonkeyNodeSharedPtr createNode(const Tokens& monkeyTokens)
    {
        const auto& name = getName(monkeyTokens);

        if (!isOperator(monkeyTokens))
        {
            return std::make_shared<IntegerNode>(name, std::stoll(monkeyTokens.at(1)));
        }
        else
        {
            const auto& operatorToken = monkeyTokens.at(2).front();
            switch (operatorToken)
            {
                case '+':
                    return std::make_shared<AdditionNode>(name);
                case '-':
                    return std::make_shared<SubtractionNode>(name);
                case '*':
                    return std::make_shared<MultiplicationNode>(name);
                case '/':
                    return std::make_shared<DivisionNode>(name);
                default:
                    throw std::runtime_error("Invalid operator: " + std::string{operatorToken});
            }
        }
    }

    void setChildrenNodes(const Tokens& monkeyTokens)
    {
        const auto& name = getName(monkeyTokens);

        if (isOperator(monkeyTokens))
        {
            auto& currentNode = m_monkeyNameToNode.at(name);

            const std::string& leftOperandName = monkeyTokens.at(1);
            auto& leftOperand = m_monkeyNameToNode.at(leftOperandName);

            const std::string& rightOperandName = monkeyTokens.at(3);
            auto& rightOperand = m_monkeyNameToNode.at(rightOperandName);

            currentNode->setOperands(leftOperand, rightOperand);
        }
    }

    static const std::string& getName(const Tokens& monkeyTokens)
    {
        return monkeyTokens.at(0);
    }

    static bool isOperator(const Tokens& monkeyTokens)
    {
        return monkeyTokens.size() >= 4;
    }

};

int64_t numberYelledByRootMonkey(const std::vector<std::string>& monkeyLines)
{
    MonkeyParser monkeyParser{monkeyLines};

    monkeyParser.parse();

    auto rootNode = monkeyParser.getRootNode();

    return rootNode->evaluate();
}

}
}
}