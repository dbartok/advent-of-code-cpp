#include "Day08-MemoryManeuver.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <memory>
#include <stack>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct LicenseNode;
using LicenseNodeSharedPtr = std::shared_ptr<LicenseNode>;

using Metadatum = unsigned;

struct LicenseNode
{
    unsigned expectedChildCount;
    unsigned expectedMetadataCount;

    std::vector<LicenseNodeSharedPtr> childrenSharedPtrs;
    std::vector<Metadatum> metadata;

    LicenseNode(unsigned expectedChildCount, unsigned expectedMetadataCount)
        : expectedChildCount{expectedChildCount}
        , expectedMetadataCount{expectedMetadataCount}
    {

    }

    bool childrenFull() const
    {
        return childrenSharedPtrs.size() == expectedChildCount;
    }

    bool metadataFull() const
    {
        return metadata.size() == expectedMetadataCount;
    }

    void addChild(LicenseNodeSharedPtr childSharedPtr)
    {
        childrenSharedPtrs.push_back(std::move(childSharedPtr));
    }

    void addMetadatum(Metadatum metadatum)
    {
        metadata.push_back(std::move(metadatum));
    }
};

class LicenseTree
{
public:
    LicenseTree(LicenseNodeSharedPtr rootSharedPtr)
        : m_rootSharedPtr{std::move(rootSharedPtr)}
    {

    }

    Metadatum getSumOfAllMetadata() const
    {
        return getSumOfAllMetadataRecursive(m_rootSharedPtr);
    }

    Metadatum getValueOfRootNode() const
    {
        return getValueOfRootNodeRecursive(m_rootSharedPtr);
    }

private:
    LicenseNodeSharedPtr m_rootSharedPtr;

    static Metadatum getSumOfAllMetadataRecursive(const LicenseNodeSharedPtr& rootSharedPtr)
    {
        const std::vector<Metadatum>& metadata = rootSharedPtr->metadata;
        Metadatum sumForThisNode = std::accumulate(metadata.cbegin(), metadata.cend(), Metadatum{},
                                                   [](Metadatum acc, const Metadatum& metadatum)
                                                   {
                                                       return acc + metadatum;
                                                   });

        const std::vector<LicenseNodeSharedPtr>& childrenSharedPtrs = rootSharedPtr->childrenSharedPtrs;
        Metadatum sumForChildren = std::accumulate(childrenSharedPtrs.cbegin(), childrenSharedPtrs.cend(), Metadatum{},
                                                   [](Metadatum acc, const LicenseNodeSharedPtr& childSharedPtr)
                                                   {
                                                       return acc + getSumOfAllMetadataRecursive(childSharedPtr);
                                                   });

        return sumForThisNode + sumForChildren;
    }


    static Metadatum getValueOfRootNodeRecursive(const LicenseNodeSharedPtr& rootSharedPtr)
    {
        const std::vector<Metadatum>& metadataForThisNode = rootSharedPtr->metadata;

        if (rootSharedPtr->childrenSharedPtrs.empty())
        {
            Metadatum sumForThisNode = std::accumulate(metadataForThisNode.cbegin(), metadataForThisNode.cend(), Metadatum{},
                                                       [](Metadatum acc, const Metadatum& metadatum)
                                                       {
                                                           return acc + metadatum;
                                                       });
            return sumForThisNode;
        }

        const std::vector<LicenseNodeSharedPtr>& childrenSharedPtrs = rootSharedPtr->childrenSharedPtrs;
        Metadatum sumForGivenChildren{};
        for (const auto& metadatum : metadataForThisNode)
        {
            if (metadatum != 0 && metadatum <= childrenSharedPtrs.size())
            {
                sumForGivenChildren += getValueOfRootNodeRecursive(childrenSharedPtrs[metadatum - 1]);
            }
        }

        return sumForGivenChildren;
    }
};

LicenseNodeSharedPtr parseLicenseTreeRoot(const std::vector<unsigned>& nodeNumbers)
{
    LicenseNodeSharedPtr rootNode = std::make_shared<LicenseNode>(nodeNumbers[0], nodeNumbers[1]);

    std::stack<LicenseNodeSharedPtr> parseStack;
    parseStack.push(rootNode);

    unsigned nodeNumbersIdx = 2;
    while (!parseStack.empty())
    {
        auto currentNodeParsed = parseStack.top();

        if (!currentNodeParsed->childrenFull())
        {
            LicenseNodeSharedPtr child = std::make_shared<LicenseNode>(nodeNumbers[nodeNumbersIdx], nodeNumbers[nodeNumbersIdx + 1]);
            currentNodeParsed->addChild(child);

            parseStack.push(std::move(child));
            nodeNumbersIdx += 2;
        }
        else if (!currentNodeParsed->metadataFull())
        {
            Metadatum metadatum = nodeNumbers[nodeNumbersIdx];
            currentNodeParsed->addMetadatum(std::move(nodeNumbers[nodeNumbersIdx]));

            ++nodeNumbersIdx;
        }
        else
        {
            parseStack.pop();
        }
    }

    return rootNode;
}

unsigned sumOfAllMetadataEntries(const std::vector<unsigned>& nodeNumbers)
{
    LicenseNodeSharedPtr rootSharedPtr = parseLicenseTreeRoot(nodeNumbers);
    LicenseTree licenseTree{std::move(rootSharedPtr)};

    return licenseTree.getSumOfAllMetadata();
}

unsigned valueOfRootNode(const std::vector<unsigned>& nodeNumbers)
{
    LicenseNodeSharedPtr rootSharedPtr = parseLicenseTreeRoot(nodeNumbers);
    LicenseTree licenseTree{std::move(rootSharedPtr)};

    return licenseTree.getValueOfRootNode();
}

}
