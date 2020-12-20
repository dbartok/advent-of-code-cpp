#include "Day19-MonsterMessages.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <regex>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int LEFT_INFINITE_RULE_NUMBER = 42;
const int RIGHT_INFINITE_RULE_NUMBER = 31;

}

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
                const int tokenRuleNumber = std::stoi(token);
                materializeRuleWithCaching(tokenRuleNumber);
                token = m_ruleNumToMaterializedRule.at(tokenRuleNumber);
            }
        }

        std::string joinedTokens = boost::join(tokens, "");
        joinedTokens.insert(0, "(");
        joinedTokens.insert(joinedTokens.size(), ")");
        m_ruleNumToMaterializedRule[ruleNumber] = joinedTokens;
    }
};

size_t getChunkSize(const std::vector<std::string>& messageLines)
{
    auto shortestMessageIter = std::min_element(messageLines.cbegin(), messageLines.cend(), [](const auto& lhs, const auto& rhs)
                                                {
                                                    return lhs.size() < rhs.size();
                                                });

    // Each message contains at least 3 chunks (the left infinite rule
    // occurring twice followed by the right infinite rule occurring once)
    return shortestMessageIter->size() / 3;

}

std::vector<std::string> splitStringIntoChunks(const std::string& str, size_t chunkSize)
{
    std::vector<std::string> chunks;
    for (size_t i = 0; i < str.size(); i += chunkSize)
    {
        chunks.push_back(str.substr(i, chunkSize));
    }
    return chunks;
}

template <typename Iterator>
size_t getNumMaximalConsecutiveMatchesFromStart(Iterator begin, Iterator end, const std::regex& regex)
{
    auto iter = begin;
    for (; iter != end; ++iter)
    {
        if (!std::regex_match(*iter, regex))
        {
            break;
        }
    }

    return std::distance(begin, iter);
}

bool isInfiniteMatching(const std::vector<std::string>& messageChunks, const std::regex& leftInfiniteRegex, const std::regex& rightInfiniteRegex)
{
    const size_t numMaximalLeftMatches = getNumMaximalConsecutiveMatchesFromStart(messageChunks.cbegin(), messageChunks.cend(), leftInfiniteRegex);
    const size_t numMaximalRightMatches = getNumMaximalConsecutiveMatchesFromStart(messageChunks.crbegin(), messageChunks.crend(), rightInfiniteRegex);

    // The left infinite rule needs to occur at least twice,
    // the right infinite rule needs to occur at least once
    if (numMaximalLeftMatches < 2 || numMaximalRightMatches < 1)
    {
        return false;
    }

    const size_t numRightMatchesRequired = messageChunks.size() - numMaximalLeftMatches;
    if (numRightMatchesRequired < numMaximalRightMatches)
    {
        return false;
    }

    // The left infinite rule occurs once on its own, then each time the right
    // infinite rule occurs, the left infinite rule has to occur once with it
    const size_t numLeftMatchesRequired = 1 + numRightMatchesRequired;
    if (numLeftMatchesRequired + numRightMatchesRequired > messageChunks.size())
    {
        return false;
    }

    return true;
}

int numMessagesMatchingRuleZero(const std::vector<std::string>& ruleAndMessageLines)
{
    std::vector<std::vector<std::string>> sections;
    boost::split(sections, ruleAndMessageLines, [](const auto& line) {return line.empty(); });

    MessageRulesMaterializer materializer{sections.at(0)};
    materializer.materializeAll();

    const std::regex messageRegex = materializer.getRegexForRule(0);

    const std::vector<std::string>& messages = sections.at(1);

    return std::count_if(messages.cbegin(), messages.cend(), [&messageRegex](const auto& message)
                         {
                             return std::regex_match(message, messageRegex);
                         });
}

int numMessagesMatchingRuleZeroWithInfiniteRules(const std::vector<std::string>& ruleAndMessageLines)
{
    std::vector<std::vector<std::string>> sections;
    boost::split(sections, ruleAndMessageLines, [](const auto& line) {return line.empty(); });

    MessageRulesMaterializer materializer{sections.at(0)};
    materializer.materializeAll();

    const std::regex leftInfiniteRegex = materializer.getRegexForRule(LEFT_INFINITE_RULE_NUMBER);
    const std::regex rightInfiniteRegex = materializer.getRegexForRule(RIGHT_INFINITE_RULE_NUMBER);

    const std::vector<std::string>& messages = sections.at(1);
    const size_t chunkSize = getChunkSize(messages);

    return std::count_if(messages.cbegin(), messages.cend(), [&leftInfiniteRegex, &rightInfiniteRegex, chunkSize](const auto& message)
                         {
                             std::vector<std::string> chunks = splitStringIntoChunks(message, chunkSize);
                             return isInfiniteMatching(std::move(chunks), leftInfiniteRegex, rightInfiniteRegex);
                         });
}

}
