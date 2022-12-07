#include "Day07-NoSpaceLeftOnDevice.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <memory>
#include <unordered_map>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

int SMALL_DIRECTORY_SIZE_THRESOLD = 100'000;
int TOTAL_FILESYSTEM_SIZE = 70'000'000;
int FREE_SPACE_REQUIRED = 30'000'000;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day07
{

class DirectoryNode
{
public:
    using SharedPtr = std::shared_ptr<DirectoryNode>;

    DirectoryNode(DirectoryNode* parent = nullptr)
        : m_parent{parent}
    {

    }

    DirectoryNode* getParent() const
    {
        return m_parent;
    }

    int getTotalSize() const
    {
        const int totalSizeOfFiles = std::accumulate(m_fileSizes.cbegin(), m_fileSizes.cend(), 0);
        const int totalSizeOfDirectories = std::accumulate(m_subdirectoryNameToSubdirectory.cbegin(), m_subdirectoryNameToSubdirectory.cend(), 0, [](int acc, const auto& subdirectoryNameAndSubdirectory)
                                                           {
                                                               return acc + subdirectoryNameAndSubdirectory.second->getTotalSize();
                                                           });

        return totalSizeOfFiles + totalSizeOfDirectories;
    }

    std::vector<DirectoryNode*> getAllSubdirectoriesRecursively()
    {
        std::vector<DirectoryNode*> subdirectoriesOutput;
        collectAllSubdirectoriesRecursively(subdirectoriesOutput);
        return subdirectoriesOutput;
    }

    DirectoryNode* subdirectoryAt(const std::string& subdirectoryName)
    {
        return m_subdirectoryNameToSubdirectory.at(subdirectoryName).get();
    }

    void insertDirectory(const std::string& name)
    {
        const auto parentNode = this;
        auto newDirectoryNode = std::make_shared<DirectoryNode>(parentNode);
        m_subdirectoryNameToSubdirectory.emplace(name, std::move(newDirectoryNode));
    }

    void insertFile(const std::string& _name, int size)
    {
        m_fileSizes.push_back(size);
    }

private:
    DirectoryNode* m_parent;
    std::unordered_map<std::string, DirectoryNode::SharedPtr> m_subdirectoryNameToSubdirectory;
    std::vector<int> m_fileSizes;

    void collectAllSubdirectoriesRecursively(std::vector<DirectoryNode*>& subdirectoriesOutput)
    {
        subdirectoriesOutput.push_back(this);

        for (const auto& subdirectoryNameAndSubdirectory : m_subdirectoryNameToSubdirectory)
        {
            subdirectoryNameAndSubdirectory.second->collectAllSubdirectoriesRecursively(subdirectoriesOutput);
        }
    }
};


class TerminalOutputAnalyzer
{
public:
    TerminalOutputAnalyzer(std::vector<std::string> terminalOutputLines)
        : m_terminalOutputLines{std::move(terminalOutputLines)}
        , m_rootDirectoryNode{std::make_shared<DirectoryNode>()}
        , m_currentDirectoryNode{nullptr}
    {

    }

    void reconstructFilesystem()
    {
        for (const auto& outputLine : m_terminalOutputLines)
        {
            processOutputLine(outputLine);
        }
    }

    int getTotalSizeOfSmallDirectories() const
    {
        std::vector<DirectoryNode*> allDirectories = m_rootDirectoryNode->getAllSubdirectoriesRecursively();

        return std::accumulate(allDirectories.cbegin(), allDirectories.cend(), 0, [](int acc, const auto directoryNodePtr)
                               {
                                   const int totalSize = directoryNodePtr->getTotalSize();
                                   if (totalSize <= SMALL_DIRECTORY_SIZE_THRESOLD)
                                   {
                                       return acc + totalSize;
                                   }

                                   return acc;
                               });
    }

    int getSizeOfSmallestDirectoryToDelete() const
    {
        const int rootDirectorySize = m_rootDirectoryNode->getTotalSize();
        const int currentFreeSpace = TOTAL_FILESYSTEM_SIZE - rootDirectorySize;
        const int minDirectorySizeToDelete = FREE_SPACE_REQUIRED - currentFreeSpace;

        std::vector<int> directorySizes = getAllDirectorySizes();

        std::sort(directorySizes.begin(), directorySizes.end());

        return *std::find_if(directorySizes.cbegin(), directorySizes.cend(), [minDirectorySizeToDelete](int directorySize)
                          {
                              return directorySize > minDirectorySizeToDelete;
                          });
    }

private:
    std::vector<std::string> m_terminalOutputLines;

    DirectoryNode::SharedPtr m_rootDirectoryNode;
    DirectoryNode* m_currentDirectoryNode;

    void processOutputLine(const std::string& outputLine)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, outputLine, boost::is_any_of(" "));

        if (tokens.at(0) == "$")
        {
            if (tokens.at(1) == "cd")
            {
                processDirectoryChange(tokens.at(2));
            }
        }
        else
        {
            processItemInCurrentDirectory(tokens.at(0), tokens.at(1));
        }
    }

    void processDirectoryChange(const std::string& directoryName)
    {
        if (directoryName == "/")
        {
            m_currentDirectoryNode = m_rootDirectoryNode.get();
        }
        else if (directoryName == "..")
        {
            m_currentDirectoryNode = m_currentDirectoryNode->getParent();
        }
        else
        {
            m_currentDirectoryNode = m_currentDirectoryNode->subdirectoryAt(directoryName);
        }
    }

    void processItemInCurrentDirectory(const std::string& metadata, const std::string& name)
    {
        if (metadata == "dir")
        {
            m_currentDirectoryNode->insertDirectory(name);
        }
        else
        {
            const int size = std::stoi(metadata);
            m_currentDirectoryNode->insertFile(name, size);
        }
    }

    std::vector<int> getAllDirectorySizes() const
    {
        std::vector<DirectoryNode*> allDirectories = m_rootDirectoryNode->getAllSubdirectoriesRecursively();

        std::vector<int> allDirectorySizes;
        std::transform(allDirectories.cbegin(), allDirectories.cend(), std::back_inserter(allDirectorySizes), [](const auto& directory)
                       {
                           return directory->getTotalSize();
                       });

        return allDirectorySizes;
    }
};

int totalSizeOfSmallDirectories(const std::vector<std::string>& terminalOutputLines)
{
    TerminalOutputAnalyzer terminalOutputAnalyzer{terminalOutputLines};

    terminalOutputAnalyzer.reconstructFilesystem();

    return terminalOutputAnalyzer.getTotalSizeOfSmallDirectories();
}

int sizeOfSmallestDirectoryToDelete(const std::vector<std::string>& terminalOutputLines)
{
    TerminalOutputAnalyzer terminalOutputAnalyzer{terminalOutputLines};

    terminalOutputAnalyzer.reconstructFilesystem();

    return terminalOutputAnalyzer.getSizeOfSmallestDirectoryToDelete();
}

}
}
}