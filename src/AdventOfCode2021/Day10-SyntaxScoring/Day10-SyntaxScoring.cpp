#include "Day10-SyntaxScoring.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
#include <unordered_map>
#include <stack>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const std::unordered_map<char, int> CHARACTER_TO_SCORE =
{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}
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

    int getErrorScore() const
    {
        std::stack<char> openBrackets;

        for (const char c : m_line)
        {
            const auto openingCounterpartIter = CLOSING_TO_OPENING.find(c);

            // This is an opening bracket
            if (openingCounterpartIter == CLOSING_TO_OPENING.cend())
            {
                openBrackets.push(c);
            }
            // This is a closing bracket
            else
            {
                if (openBrackets.empty() || openBrackets.top() != openingCounterpartIter->second)
                {
                    return CHARACTER_TO_SCORE.at(c);
                }
                openBrackets.pop();
            }
        }

        return 0;
    }


private:
    std::string m_line;

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
    std::vector<NavigationSubsystemSyntaxChecker> syntaxCheckers = createSyntaxCheckers(navigationSubsystemSyntaxLines);
    return std::accumulate(syntaxCheckers.cbegin(), syntaxCheckers.cend(), 0, [](const auto& acc, const auto& syntaxChecker)
                           {
                               return acc + syntaxChecker.getErrorScore();
                           });
}

}
}
}