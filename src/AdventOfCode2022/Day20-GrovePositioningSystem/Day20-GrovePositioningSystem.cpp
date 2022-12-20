#include "Day20-GrovePositioningSystem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <iterator>
#include <memory>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const std::vector<int> GROVE_OFFSETS{1000, 2000, 3000};

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day20
{

class EncryptedFileMixer
{
public:
    EncryptedFileMixer(const std::vector<int>& encrypedFile)
    {
        Node* previousNode = nullptr;
        for (int value : encrypedFile)
        {
            m_nodes.push_back(std::make_shared<Node>(value));
            Node* currentNode = m_nodes.back().get();

            if (previousNode)
            {
                previousNode->next = currentNode;
                currentNode->previous = previousNode;
            }

            previousNode = currentNode;
        }

        // Link last node and first node
        m_nodes.back()->next = m_nodes.front().get();
        m_nodes.front()->previous = m_nodes.back().get();
    }

    void mix()
    {
        for (auto& node : m_nodes)
        {
            moveNodeForward(node.get(), node->value);
        }
    }

    int getSumOfGroveCoordinates() const
    {
        Node* baseNode = findNodeWithValue(0);

        int sumOfGroveCoordinates = 0;

        for (const int groveOffset : GROVE_OFFSETS)
        {
            const Node* groveCoordinateNode = getNodeAtOffset(baseNode, groveOffset);
            sumOfGroveCoordinates += groveCoordinateNode->value;
        }

        return sumOfGroveCoordinates;
    }

private:
    struct Node
    {
        int value;
        Node* next;
        Node* previous;

        Node(int value)
            : value{value}
            , next{nullptr}
            , previous{nullptr}
        {

        }
    };

    using NodeSharedPtr = std::shared_ptr<Node>;

    std::vector<NodeSharedPtr> m_nodes;

    void moveNodeForward(Node* node, int numSteps)
    {
        // Unlink node
        node->previous->next = node->next;
        node->next->previous = node->previous;

        // Simplify the number of steps
        const int modulus = m_nodes.size() - 1;
        numSteps %= modulus;
        if (numSteps < 0)
        {
            numSteps += modulus;
        }

        // Find new previous node
        Node* newPreviousNode = node->previous;
        for (size_t i = 0; i < numSteps; ++i)
        {
            newPreviousNode = newPreviousNode->next;
        }

        // Link with new neighbors
        auto newNextNode = newPreviousNode->next;
        newPreviousNode->next = node;
        node->previous = newPreviousNode;
        newNextNode->previous = node;
        node->next = newNextNode;
    }

    Node* getNodeAtOffset(Node* node, int offset) const
    {
        offset %= m_nodes.size();
        Node* offsetNode = node;
        for (size_t i = 0; i < offset; ++i)
        {
            offsetNode = offsetNode->next;
        }

        return offsetNode;
    }

    Node* findNodeWithValue(int value) const
    {
        Node* node = m_nodes.at(0).get();

        while (node->value != value)
        {
            node = node->next;
        }

        return node;
    }
};

std::vector<int> parseEncrypedFileLines(const std::vector<std::string>& encryptedFileLines)
{
    std::vector<int> encryptedFile;

    std::transform(encryptedFileLines.cbegin(), encryptedFileLines.end(), std::back_inserter(encryptedFile), [](const auto& line)
                   {
                       return std::stoi(line);
                   });

    return encryptedFile;
}

int sumOfGroveCoordinates(const std::vector<std::string>& encryptedFileLines)
{
    std::vector<int> encryptedFile = parseEncrypedFileLines(encryptedFileLines);

    EncryptedFileMixer encryptedFileMixer{encryptedFile};

    encryptedFileMixer.mix();

    return encryptedFileMixer.getSumOfGroveCoordinates();
}

}
}
}