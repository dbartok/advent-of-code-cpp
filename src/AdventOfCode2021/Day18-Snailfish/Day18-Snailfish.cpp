#include "Day18-Snailfish.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/optional.hpp>

#include <memory>
#include <stdexcept>
#include <cmath>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

int EXPLODE_NESTING_LEVEL_THRESHOLD = 4;
int SPLIT_THRESHOLD = 10;
int MAGNITUDE_LEFT_MULTIPLIER = 3;
int MAGNITUDE_RIGHT_MULTIPLIER = 2;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day18
{

struct Node
{
    using SharedPtr = std::shared_ptr<Node>;

    Node::SharedPtr leftChildPtr = nullptr;
    Node::SharedPtr rightChildPtr = nullptr;
    boost::optional<int> number;

    Node* prevNodeInOrderPtr = nullptr;
    Node* nextNodeInOrderPtr = nullptr;
};

class SnailfishNumberCalculator
{
public:
    SnailfishNumberCalculator(Node::SharedPtr initialNodePtr)
        : m_rootNodePtr{std::move(initialNodePtr)}
    {

    }

    void addNumber(Node::SharedPtr addendNodePtr)
    {
        m_rootNodePtr = mergeNodes(m_rootNodePtr, addendNodePtr);
        reduce();
    }

    int getMagnitude() const
    {
        return getMagnitude(m_rootNodePtr.get());
    }

private:
    Node::SharedPtr m_rootNodePtr;

    void reduce()
    {
        while (true)
        {
            if (explode())
            {
                continue;
            }

            if (split())
            {
                continue;
            }

            break;
        }
    }

    bool explode()
    {
        Node* nodeToExplode = findNodeToExplode(m_rootNodePtr.get(), 0);

        if (!nodeToExplode)
        {
            return false;
        }

        auto leftChildPtr = nodeToExplode->leftChildPtr;
        if (Node* prevNodePtr = leftChildPtr->prevNodeInOrderPtr)
        {
            prevNodePtr->number.value() += leftChildPtr->number.value();
            prevNodePtr->nextNodeInOrderPtr = nodeToExplode;
            nodeToExplode->prevNodeInOrderPtr = prevNodePtr;
        }

        auto rightChildPtr = nodeToExplode->rightChildPtr;
        if (Node* nextNodePtr = rightChildPtr->nextNodeInOrderPtr)
        {
            nextNodePtr->number.value() += rightChildPtr->number.value();
            nextNodePtr->prevNodeInOrderPtr = nodeToExplode;
            nodeToExplode->nextNodeInOrderPtr = nextNodePtr;
        }

        nodeToExplode->leftChildPtr = nullptr;
        nodeToExplode->rightChildPtr = nullptr;
        nodeToExplode->number = 0;

        return true;
    }

    bool split()
    {
        Node* nodeToSplit = findNodeToSplit(m_rootNodePtr.get());

        if (!nodeToSplit)
        {
            return false;
        }

        Node::SharedPtr newLeftChildPtr = std::make_shared<Node>();
        newLeftChildPtr->number = nodeToSplit->number.value() / 2;
        if (Node* prevNodePtr = nodeToSplit->prevNodeInOrderPtr)
        {
            prevNodePtr->nextNodeInOrderPtr = newLeftChildPtr.get();
            newLeftChildPtr->prevNodeInOrderPtr = prevNodePtr;
        }

        Node::SharedPtr newRightChildPtr = std::make_shared<Node>();
        newRightChildPtr->number = (nodeToSplit->number.value() + 1) / 2;
        if (Node* nextNodePtr = nodeToSplit->nextNodeInOrderPtr)
        {
            nextNodePtr->prevNodeInOrderPtr = newRightChildPtr.get();
            newRightChildPtr->nextNodeInOrderPtr = nextNodePtr;
        }

        newLeftChildPtr->nextNodeInOrderPtr = newRightChildPtr.get();
        newRightChildPtr->prevNodeInOrderPtr = newLeftChildPtr.get();

        nodeToSplit->leftChildPtr = newLeftChildPtr;
        nodeToSplit->rightChildPtr = newRightChildPtr;
        nodeToSplit->number.reset();

        return true;
    }

    static Node* findNodeToExplode(Node* rootNodePtr, int nestingLevel)
    {
        if (!(rootNodePtr->leftChildPtr && rootNodePtr->rightChildPtr))
        {
            return nullptr;
        }

        const bool areBothChildrenRegularNodes = rootNodePtr->leftChildPtr->number && rootNodePtr->rightChildPtr->number;
     
        if (areBothChildrenRegularNodes && nestingLevel >= EXPLODE_NESTING_LEVEL_THRESHOLD)
        {
            return rootNodePtr;
        }

        if (Node* node = findNodeToExplode(rootNodePtr->leftChildPtr.get(), nestingLevel + 1))
        {
            return node;
        }

        return findNodeToExplode(rootNodePtr->rightChildPtr.get(), nestingLevel + 1);
    }

    static Node* findNodeToSplit(Node* rootNodePtr)
    {
        if (!rootNodePtr)
        {
            return nullptr;
        }
        
        if (rootNodePtr->number.value_or(0) >= SPLIT_THRESHOLD)
        {
            return rootNodePtr;
        }

        if (Node* node = findNodeToSplit(rootNodePtr->leftChildPtr.get()))
        {
            return node;
        }

        return findNodeToSplit(rootNodePtr->rightChildPtr.get());
    }

    static Node::SharedPtr mergeNodes(Node::SharedPtr leftChildPtr, Node::SharedPtr rightChildPtr)
    {
        Node::SharedPtr newRootNodePtr = std::make_shared<Node>();

        newRootNodePtr->leftChildPtr = leftChildPtr;

        newRootNodePtr->rightChildPtr = rightChildPtr;

        Node* rightmostRegularNodeInLeftSubtree = findRightmostRegularNode(leftChildPtr.get());
        Node* leftmostRegularNodeInRightSubtree = findLeftmostRegularNode(rightChildPtr.get());

        rightmostRegularNodeInLeftSubtree->nextNodeInOrderPtr = leftmostRegularNodeInRightSubtree;
        leftmostRegularNodeInRightSubtree->prevNodeInOrderPtr = rightmostRegularNodeInLeftSubtree;

        return newRootNodePtr;
    }

    static Node* findRightmostRegularNode(Node* root)
    {
        if (root->number)
        {
            return root;
        }

        return findRightmostRegularNode(root->rightChildPtr.get());
    }

    static Node* findLeftmostRegularNode(Node* root)
    {
        if (root->number)
        {
            return root;
        }

        return findLeftmostRegularNode(root->leftChildPtr.get());
    }

    static int getMagnitude(Node* root)
    {
        if (root->number)
        {
            return root->number.value();
        }

        return MAGNITUDE_LEFT_MULTIPLIER * getMagnitude(root->leftChildPtr.get()) + MAGNITUDE_RIGHT_MULTIPLIER * getMagnitude(root->rightChildPtr.get());
    }
};

class SnailfishNumberStringParser
{
public:
    SnailfishNumberStringParser(std::string snailfishNumberString)
        : m_snailfishNumberString{std::move(snailfishNumberString)}
    {
        m_snailfishNumberString.erase(std::remove(m_snailfishNumberString.begin(), m_snailfishNumberString.end(), ','), m_snailfishNumberString.end());
    }

    Node::SharedPtr createRootNodePtr()
    {
        Node::SharedPtr node = std::make_shared<Node>();

        const char c = getNextChar();
        if (c == '[')
        {
            node->leftChildPtr = createRootNodePtr();
            node->rightChildPtr = createRootNodePtr();
            if (getNextChar() != ']')
            {
                throw std::runtime_error("Missing closing bracket");
            }
        }
        else
        {
            node->number = c - '0';
            chainWithPrevCreatedNode(node.get());
        }

        return node;
    }

private:
    std::string m_snailfishNumberString;

    size_t m_index = 0;
    Node* m_prevCreatedNode = nullptr;

    char getNextChar()
    {
        return m_snailfishNumberString.at(m_index++);
    }

    void chainWithPrevCreatedNode(Node* node)
    {
        if (m_prevCreatedNode)
        {
            m_prevCreatedNode->nextNodeInOrderPtr = node;
        }
        node->prevNodeInOrderPtr = m_prevCreatedNode;
        m_prevCreatedNode = node;
    }
};

int magnitudeOfFinalSum(const std::vector<std::string>& snailfishNumberStringLines)
{
    SnailfishNumberStringParser parser{snailfishNumberStringLines.front()};
    Node::SharedPtr firstNumberNodePtr = parser.createRootNodePtr();
    SnailfishNumberCalculator calculator{std::move(firstNumberNodePtr)};

    for (auto lineIter = std::next(snailfishNumberStringLines.cbegin()); lineIter != snailfishNumberStringLines.cend(); ++lineIter)
    {
        SnailfishNumberStringParser parser{*lineIter};
        Node::SharedPtr currentNumberPtr = parser.createRootNodePtr();
        calculator.addNumber(std::move(currentNumberPtr));
    }

    return calculator.getMagnitude();
}

}
}
}