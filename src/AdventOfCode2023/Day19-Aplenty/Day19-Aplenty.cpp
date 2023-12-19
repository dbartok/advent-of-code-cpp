#include "Day19-Aplenty.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <queue>
#include <unordered_map>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const std::pair<int, int> DEFAULT_RANGE = {0, std::numeric_limits<int>::max()};

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day19
{

using Category = char;
using CategoryToRating = std::unordered_map<Category, int>;
using RatingRange = std::pair<int, int>;
using CategoryToRatingRange = std::unordered_map<Category, RatingRange>;

class Workflow;
using WorkflowNameToWorkflow = std::unordered_map<std::string, Workflow>;

struct WorkflowNode
{
    std::string name;
    CategoryToRatingRange range;

    WorkflowNode(std::string name, CategoryToRatingRange range)
        : name{std::move(name)}
        , range{std::move(range)}
    {

    }
};

class Rule
{
public:
    Rule(Category category, char condition, int threshold, std::string matchDestination)
        : m_category{category}
        , m_condition{condition}
        , m_threshold{threshold}
        , m_matchDestination{std::move(matchDestination)}
    {

    }

    CategoryToRatingRange getMatchRange(CategoryToRatingRange baseRange) const
    {
        RatingRange& constrainedRange = baseRange.at(m_category);

        if (m_condition == '<')
        {
            constrainedRange.second = m_threshold - 1;
        }
        else
        {
            constrainedRange.first = m_threshold + 1;
        }

        return baseRange;
    }

    CategoryToRatingRange getNonMatchRange(CategoryToRatingRange baseRange) const
    {
        RatingRange& constrainedRange = baseRange.at(m_category);

        if (m_condition == '<')
        {
            constrainedRange.first = m_threshold;
        }
        else
        {
            constrainedRange.second = m_threshold;
        }

        return baseRange;
    }

    const std::string& getMatchDestination() const
    {
        return m_matchDestination;
    }

private:
    Category m_category;
    char m_condition;
    int m_threshold;
    std::string m_matchDestination;
};

class Workflow
{
public:
    Workflow(std::vector<Rule> rules)
        : m_rules{std::move(rules)}
    {

    }

    std::vector<WorkflowNode> getNextNodes(CategoryToRatingRange baseRange) const
    {
        std::vector<WorkflowNode> nextNodes;
        for (auto ruleIter = m_rules.cbegin(); ruleIter < m_rules.cend() - 1; ++ruleIter)
        {
            CategoryToRatingRange matchRange = ruleIter->getMatchRange(baseRange);
            nextNodes.emplace_back(ruleIter->getMatchDestination(), matchRange);

            baseRange = ruleIter->getNonMatchRange(baseRange);
        }

        // Last rule always matches
        nextNodes.emplace_back(m_rules.back().getMatchDestination(), baseRange);

        return nextNodes;
    }

private:
    std::vector<Rule> m_rules;
};

class WorkflowAnalyzer
{
public:
    WorkflowAnalyzer(WorkflowNameToWorkflow workflowNameToWorkflow)
        : m_workflowNameToWorkflow{std::move(workflowNameToWorkflow)}
    {

    }

    void analyze()
    {
        WorkflowNode startNode{"in", {{'x', DEFAULT_RANGE}, {'m', DEFAULT_RANGE}, {'a', DEFAULT_RANGE}, {'s', DEFAULT_RANGE}}};
        std::queue<WorkflowNode> openNodes;
        openNodes.push(startNode);

        while (!openNodes.empty())
        {
            WorkflowNode currentNode = openNodes.front();
            openNodes.pop();

            const Workflow& currentWorkflow = m_workflowNameToWorkflow.at(currentNode.name);
            std::vector<WorkflowNode> nextNodes = currentWorkflow.getNextNodes(currentNode.range);

            for (auto& nextNode : nextNodes)
            {
                if (nextNode.name == "A")
                {
                    m_acceptedRanges.push_back(nextNode.range);
                }
                else if (nextNode.name != "R")
                {
                    openNodes.push(std::move(nextNode));
                }
            }
        }
    }

    bool isAccepted(const CategoryToRating& part) const
    {
        return std::any_of(m_acceptedRanges.cbegin(), m_acceptedRanges.cend(), [&part](const auto& acceptedRange)
                           {
                               return std::all_of(part.cbegin(), part.cend(), [&acceptedRange](const auto& categoryAndRating)
                                                  {
                                                      const auto& acceptedRangeForCategory = acceptedRange.at(categoryAndRating.first);
                                                      return categoryAndRating.second >= acceptedRangeForCategory.first && categoryAndRating.second <= acceptedRangeForCategory.second;
                                                  });
                           });
    }

private:
    WorkflowNameToWorkflow m_workflowNameToWorkflow;

    std::vector<CategoryToRatingRange> m_acceptedRanges;
};

Rule createRule(const std::string& ruleString)
{
    std::vector<std::string> ruleStringTokens;
    boost::split(ruleStringTokens, ruleString, boost::is_any_of("<>:"));

    if (ruleStringTokens.size() == 1)
    {
        return Rule{'\0', '\0', 0, ruleString};
    }
    else
    {
        return Rule{ruleString.at(0), ruleString.at(1), std::stoi(ruleStringTokens.at(1)), ruleStringTokens.at(2)};
    }
}

Workflow createWorkflow(const std::string& workflowString)
{
    std::vector<std::string> workflowStringTokens;
    boost::split(workflowStringTokens, workflowString, boost::is_any_of(","));

    std::vector<Rule> rules;

    for (const auto& ruleString : workflowStringTokens)
    {
        Rule rule = createRule(ruleString);
        rules.push_back(std::move(rule));
    }

    return rules;
}

CategoryToRating createPart(const std::string& partLine)
{
    const std::string partString = partLine.substr(1, partLine.size() - 2);
    std::vector<std::string> partLineTokens;
    boost::split(partLineTokens, partString, boost::is_any_of(","));

    CategoryToRating categoryToRating;

    for (const auto& categoryAndRatingString : partLineTokens)
    {
        std::vector<std::string> categoryAndRatingStringTokens;
        boost::split(categoryAndRatingStringTokens, categoryAndRatingString, boost::is_any_of("="));

        categoryToRating.emplace(categoryAndRatingStringTokens.at(0).front(), std::stoi(categoryAndRatingStringTokens.at(1)));
    }

    return categoryToRating;
}

WorkflowAnalyzer createWorkflowAnalyzer(const std::vector<std::string>& systemLines)
{
    WorkflowNameToWorkflow workflowNameToWorkflow;

    for (auto workflowLineIter = systemLines.cbegin(); !workflowLineIter->empty(); ++workflowLineIter)
    {
        std::vector<std::string> workflowLineTokens;
        boost::split(workflowLineTokens, *workflowLineIter, boost::is_any_of("{}"));
        Workflow workflow = createWorkflow(workflowLineTokens.at(1));
        workflowNameToWorkflow.emplace(std::move(workflowLineTokens.at(0)), std::move(workflow));
    }

    return {std::move(workflowNameToWorkflow)};
}

std::vector<CategoryToRating> createParts(const std::vector<std::string>& systemLines)
{
    std::vector<CategoryToRating> parts;

    for (auto partLineIter = systemLines.crbegin(); !partLineIter->empty(); ++partLineIter)
    {
        CategoryToRating part = createPart(*partLineIter);
        parts.push_back(std::move(part));
    }

    return parts;
}

int sumOfRatingNumbersOfAllAcceptedParts(const std::vector<std::string>& systemLines)
{
    WorkflowAnalyzer workflowAnalyzer = createWorkflowAnalyzer(systemLines);
    std::vector<CategoryToRating> parts = createParts(systemLines);

    workflowAnalyzer.analyze();

    return std::accumulate(parts.cbegin(), parts.cend(), 0, [&workflowAnalyzer](auto acc, const auto& part)
                           {
                               if (workflowAnalyzer.isAccepted(part))
                               {
                                   acc += std::accumulate(part.cbegin(), part.cend(), 0, [](auto acc, const auto& categoryAndRating)
                                                          {
                                                              return acc + categoryAndRating.second;
                                                          });
                               }
                               return acc;
                           });
}

}
}
}