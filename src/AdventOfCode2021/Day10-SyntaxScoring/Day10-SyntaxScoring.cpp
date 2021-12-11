#include "Day10-SyntaxScoring.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include <cassert>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const std::unordered_map<char, int> CHARACTER_TO_ERROR_SCORE =
{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}
};

const std::unordered_map<char, int> CHARACTER_TO_AUTOCOMPLETE_SCORE =
{
    {'(', 1},
    {'[', 2},
    {'{', 3},
    {'<', 4}
};

const std::unordered_map<char, char> CLOSING_TO_OPENING =
{
    {']', '['},
    {')', '('},
    {'}', '{'},
    {'>', '<'}
};

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day10
{

class NavigationSubsystemSyntaxChecker
{
public:
    NavigationSubsystemSyntaxChecker(std::string line)
        : m_line{std::move(line)}
    {

    }

    int calculateErrorScore() const
    {
        m_openBrackets = {};

        for (const char c : m_line)
        {
            const auto openingCounterpartIter = CLOSING_TO_OPENING.find(c);

            // This is an opening bracket
            if (openingCounterpartIter == CLOSING_TO_OPENING.cend())
            {
                m_openBrackets.push(c);
            }
            // This is a closing bracket
            else
            {
                if (m_openBrackets.empty() || m_openBrackets.top() != openingCounterpartIter->second)
                {
                    return CHARACTER_TO_ERROR_SCORE.at(c);
                }
                m_openBrackets.pop();
            }
        }

        return 0;
    }

    int64_t getAutocompleteScore() const
    {
        const int errorScore = calculateErrorScore();
        assert(errorScore == 0);

        int64_t autocompleteScore = 0;

        while (!m_openBrackets.empty())
        {
            char c = m_openBrackets.top();
            autocompleteScore *= 5;
            autocompleteScore += CHARACTER_TO_AUTOCOMPLETE_SCORE.at(c);

            m_openBrackets.pop();
        }

        return autocompleteScore;
    }


private:
    std::string m_line;
    mutable std::stack<char> m_openBrackets;
};

std::vector<NavigationSubsystemSyntaxChecker> createSyntaxCheckers(const std::vector<std::string>& navigationSubsystemSyntaxLines)
{
    std::vector<NavigationSubsystemSyntaxChecker> syntaxCheckers;

    for (const auto& line : navigationSubsystemSyntaxLines)
    {
        syntaxCheckers.emplace_back(line);
    }

    return syntaxCheckers;
}


int totalSyntaxErrorScore(const std::vector<std::string>& navigationSubsystemSyntaxLines)
{
    const std::vector<NavigationSubsystemSyntaxChecker> syntaxCheckers = createSyntaxCheckers(navigationSubsystemSyntaxLines);
    return std::accumulate(syntaxCheckers.cbegin(), syntaxCheckers.cend(), 0, [](const auto& acc, const auto& syntaxChecker)
                           {
                               return acc + syntaxChecker.calculateErrorScore();
                           });
}

int64_t middleAutocompleteScore(const std::vector<std::string>& navigationSubsystemSyntaxLines)
{
    std::vector<NavigationSubsystemSyntaxChecker> syntaxCheckers = createSyntaxCheckers(navigationSubsystemSyntaxLines);
    syntaxCheckers.erase(std::remove_if(syntaxCheckers.begin(),
                                        syntaxCheckers.end(),
                                        [](const auto& syntaxChecker)
                                        {
                                            return syntaxChecker.calculateErrorScore() > 0;
                                        }),
                         syntaxCheckers.end());

    std::sort(syntaxCheckers.begin(), syntaxCheckers.end(), [](const auto& lhs, const auto& rhs)
              {
                  return lhs.getAutocompleteScore() < rhs.getAutocompleteScore();
              });

    return syntaxCheckers.at(syntaxCheckers.size() / 2).getAutocompleteScore();
}

}
}
}