#include "Day06-CustomCustoms.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_set>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using GroupAnswerTextSection = std::vector<std::string>;

struct GroupAnswer
{
    GroupAnswerTextSection answers;

    int getNumTotalYesAnswers() const
    {
        std::unordered_set<char> answerSet;
        auto allAnswers = boost::join(answers, "");

        for (auto answer : allAnswers)
        {
            answerSet.insert(answer);
        }

        return answerSet.size();
    }
};

std::vector<GroupAnswer> parseGroupAnswers(const std::vector<std::string>& lines)
{
    std::vector<GroupAnswerTextSection> groupAnswerTextSections;
    boost::split(groupAnswerTextSections, lines, [](const auto& elem) {return elem.empty(); });

    std::vector<GroupAnswer> groupAnswers;
    for (auto& groupAnswerTextSection : groupAnswerTextSections)
    {
        GroupAnswer groupAnswer{std::move(groupAnswerTextSection)};
        groupAnswers.push_back(std::move(groupAnswer));
    }

    return groupAnswers;
}

int sumOfYesAnswers(const std::vector<std::string>& lines)
{
    std::vector<GroupAnswer> groupAnswers = parseGroupAnswers(lines);
    return std::accumulate(groupAnswers.cbegin(), groupAnswers.cend(), 0, [](int acc, const auto& groupAnswer)
                           {
                               return acc + groupAnswer.getNumTotalYesAnswers();
                           });
}

}
