#include "Day19-MonsterMessages.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <regex>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class MessageRulesMaterializer
{
public:
    MessageRulesMaterializer(std::vector<std::string> messageRules)
    {
        for (const auto& rule : messageRules)
        {
            std::vector<std::string> tokens;
            boost::split(tokens, rule, boost::is_any_of(":"), boost::token_compress_on);

            const int ruleNumber = std::stoi(tokens.at(0));
            std::string matcherString = tokens.at(1).substr(1);
            m_ruleNumToMatcherString[ruleNumber] = std::move(matcherString);
        }
    }

    void materializeAll()
    {
        for (const auto& ruleNumberAndMatcher : m_ruleNumToMatcherString)
        {
            materializeRuleWithCaching(ruleNumberAndMatcher.first);
        }
    }

    std::regex getRegexForRule(int ruleNumber) const
    {
        return std::regex{m_ruleNumToMaterializedRule.at(ruleNumber)};
    }

private:
    std::unordered_map<int, std::string> m_ruleNumToMatcherString;
    std::unordered_map<int, std::string> m_ruleNumToMaterializedRule;

    void materializeRuleWithCaching(int ruleNumber)
    {
        if (m_ruleNumToMaterializedRule.find(ruleNumber) != m_ruleNumToMaterializedRule.cend())
        {
            return;
        }

        const std::string matcherString = m_ruleNumToMatcherString.at(ruleNumber);

        std::vector<std::string> tokens;
        boost::split(tokens, matcherString, boost::is_any_of(" "));

        if (tokens.size() == 1 && tokens.front().front() == '\"')
        {
            m_ruleNumToMaterializedRule[ruleNumber] = tokens.front().substr(1, tokens.front().size() - 2);
            return;
        }

        for (auto& token : tokens)
        {
            if (token != "|")
            {
                const int ruleNumber = std::stoi(token);
                materializeRuleWithCaching(ruleNumber);
                token = m_ruleNumToMaterializedRule.at(ruleNumber);
            }
        }

        std::string joinedTokens = boost::join(tokens, "");
        joinedTokens.insert(0, "(");
        joinedTokens.insert(joinedTokens.size(), ")");
        m_ruleNumToMaterializedRule[ruleNumber] = joinedTokens;
    }
};

std::regex createRegexFromRules(const std::vector<std::string>& ruleLines)
{
    MessageRulesMaterializer materializer{ruleLines};
    materializer.materializeAll();
    return materializer.getRegexForRule(0);
}

int numMessagesMatchingRuleZero(const std::vector<std::string>& ruleAndMessageLines)
{
    std::vector<std::vector<std::string>> sections;
    boost::split(sections, ruleAndMessageLines, [](const auto& line) {return line.empty(); });

    std::regex messageRegex = createRegexFromRules(sections.at(0));
    const std::vector<std::string>& messages = sections.at(1);

    return std::count_if(messages.cbegin(), messages.cend(), [&messageRegex](const auto& message)
                         {
                             return std::regex_match(message, messageRegex);
                         });
}

}
