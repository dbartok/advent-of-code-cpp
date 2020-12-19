#include "Day18-OperationOrder.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <memory>
#include <numeric>
#include <stdexcept>
#include <cassert>
#include <regex>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class ExpressionNode
{
public:
    using SharedPtr = std::shared_ptr<ExpressionNode>;

    virtual ~ExpressionNode()
    {

    }

    virtual int64_t evaluate() const = 0;
};

class TwoOperandNode : public ExpressionNode
{
public:
    TwoOperandNode(ExpressionNode::SharedPtr leftOperand, ExpressionNode::SharedPtr rightOperand)
        : m_leftOperand{std::move(leftOperand)}
        , m_rightOperand{std::move(rightOperand)}
    {

    }

protected:
    ExpressionNode::SharedPtr m_leftOperand;
    ExpressionNode::SharedPtr m_rightOperand;
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

class MultiplicationNode : public TwoOperandNode
{
public:
    using TwoOperandNode::TwoOperandNode;

    int64_t evaluate() const override
    {
        return m_leftOperand->evaluate() * m_rightOperand->evaluate();
    }
};

class NumberNode : public ExpressionNode
{
public:
    NumberNode(int64_t number)
        :m_number{number}
    {

    }

    int64_t evaluate() const override
    {
        return m_number;
    }

private:
    int64_t m_number;
};

bool isOperator(char c)
{
    return (c == '*' || c == '+');
}

bool isSingleNumber(const std::string& str)
{
    const std::regex numberRegex{R"(\d+)"};
    return std::regex_match(str, numberRegex);
}

void removeAllSpaces(std::string& str)
{
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}

void trimSurroundingParentheses(std::string& str)
{
    if (str.front() == '(' && str.back() == ')')
    {
        str = str.substr(1, str.size() - 2);
    }
}

int getOperatorIndexBeforeRightmostExpression(const std::string& expressionLine, char preferredOperator)
{
    unsigned numOpeningParenthesesNeeded = 0;
    int rightmostPreferredOperatorIndex = -1;
    for (int i = expressionLine.size() - 1; i >= 0; --i)
    {
        char c = expressionLine.at(i);
        if (c == '(')
        {
            if (numOpeningParenthesesNeeded == 0)
            {
                throw std::runtime_error("Invalid parentheses setup in: " + expressionLine);
            }
            --numOpeningParenthesesNeeded;
        }
        else if (c == ')')
        {
            ++numOpeningParenthesesNeeded;
        }
        else if (isOperator(c) && numOpeningParenthesesNeeded == 0)
        {
            if (c != preferredOperator)
            {
                return i;
            }
            else if (rightmostPreferredOperatorIndex == -1)
            {
                rightmostPreferredOperatorIndex = i;
            }
        }
    }

    return rightmostPreferredOperatorIndex;
}

TwoOperandNode::SharedPtr createTwoOperandNode(char operatorChar, ExpressionNode::SharedPtr leftOperand, ExpressionNode::SharedPtr rightOperand)
{
    if (operatorChar == '+')
    {
        return std::make_shared<AdditionNode>(leftOperand, rightOperand);
    }
    else if (operatorChar == '*')
    {
        return std::make_shared<MultiplicationNode>(leftOperand, rightOperand);
    }

    throw std::runtime_error("Invalid operator: " + operatorChar);
}

ExpressionNode::SharedPtr createExpressionTree(std::string expressionLine, char preferredOperator)
{
    removeAllSpaces(expressionLine);
    if (isSingleNumber(expressionLine))
    {
        return std::make_shared<NumberNode>(std::stoll(expressionLine));
    }

    int operatorIndex = getOperatorIndexBeforeRightmostExpression(expressionLine, preferredOperator);
    if (operatorIndex == -1)
    {
        trimSurroundingParentheses(expressionLine);
        operatorIndex = getOperatorIndexBeforeRightmostExpression(expressionLine, preferredOperator);
    }

    ExpressionNode::SharedPtr leftOperand = createExpressionTree(expressionLine.substr(0, operatorIndex), preferredOperator);
    ExpressionNode::SharedPtr rightOperand = createExpressionTree(expressionLine.substr(operatorIndex + 1), preferredOperator);

    TwoOperandNode::SharedPtr node = createTwoOperandNode(expressionLine.at(operatorIndex), std::move(leftOperand), std::move(rightOperand));
    return node;
}

int64_t sumOfResultingValues(const std::vector<std::string>& expressionLines, char preferredOperator = '\0')
{
    return std::accumulate(expressionLines.cbegin(), expressionLines.cend(), 0ll, [preferredOperator](int64_t acc, const auto& line)
                           {
                               const ExpressionNode::SharedPtr expressionTreeRoot = createExpressionTree(line, preferredOperator);
                               return acc + expressionTreeRoot->evaluate();
                           });
}

int64_t sumOfResultingValuesWithoutPrecedence(const std::vector<std::string>& expressionLines)
{
    return sumOfResultingValues(expressionLines);
}

int64_t sumOfResultingValuesWithPrecedence(const std::vector<std::string>& expressionLines)
{
    return sumOfResultingValues(expressionLines, '+');
}

}
