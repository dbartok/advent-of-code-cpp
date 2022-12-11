#include "Day11-MonkeyInTheMiddle.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <regex>
#include <memory>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int WORRY_LEVEL_DIVISOR_BEFORE_PASSING_ITEM = 3;
const unsigned NUM_ROUNDS_TO_SIMULATE = 20;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day11
{

class Operation
{
public:
    using SharedPtr = std::shared_ptr<Operation>;

    virtual int apply(int operand) const = 0;
};

class Addition : public Operation
{
public:
    Addition(int addend)
        : m_factor{addend}
    {

    }

    int apply(int operand) const override
    {
        return operand + m_factor;
    }

private:
    int m_factor;
};

class Multiplication : public Operation
{
public:
    Multiplication(int factor)
        : m_factor{factor}
    {

    }

    int apply(int operand) const override
    {
        return operand * m_factor;
    }

private:
    int m_factor;
};

class Square : public Operation
{
public:
    int apply(int operand) const override
    {
        return operand * operand;
    }
};

class Monkey
{
public:
    Monkey(std::vector<int> startingItems, Operation::SharedPtr operation, int testModulus, unsigned testTrueTarget, unsigned testFalseTarget)
        : m_operation{std::move(operation)}
        , m_testModulus{testModulus}
        , m_testTrueTarget{testTrueTarget}
        , m_testFalseTarget{testFalseTarget}
    {
        m_currentItems = {std::make_move_iterator(startingItems.begin()), std::make_move_iterator(startingItems.end())};
    }

    void setAllMonkeys(std::vector<Monkey>* allMonkeys)
    {
        m_allMonkeys = std::move(allMonkeys);
    }

    void executeTurn()
    {
        while (!m_currentItems.empty())
        {
            const int item = m_currentItems.front();
            m_currentItems.pop_front();
            processItem(item);
        }
    }

    void receiveItem(int item)
    {
        m_currentItems.push_back(item);
    }

    int getNumItemsInspected() const
    {
        return m_numItemsInspected;
    }

private:
    std::deque<int> m_currentItems;
    Operation::SharedPtr m_operation;
    int m_testModulus;
    unsigned m_testTrueTarget;
    unsigned m_testFalseTarget;

    std::vector<Monkey>* m_allMonkeys;

    int m_numItemsInspected = 0;

    void processItem(int item)
    {
        int newWorryLevel = m_operation->apply(item);
        newWorryLevel /= WORRY_LEVEL_DIVISOR_BEFORE_PASSING_ITEM;
        const unsigned target = (newWorryLevel % m_testModulus == 0) ? m_testTrueTarget : m_testFalseTarget;
        m_allMonkeys->at(target).receiveItem(newWorryLevel);

        ++m_numItemsInspected;
    }
};

class MonkeyGroupSimulator
{
public:
    MonkeyGroupSimulator(std::vector<Monkey> monkeys)
        : m_monkeys{std::move(monkeys)}
    {
        for (auto& monkey : m_monkeys)
        {
            monkey.setAllMonkeys(&m_monkeys);
        }
    }

    void simulate(unsigned numRounds)
    {
        for (unsigned iteration = 0; iteration < numRounds; ++iteration)
        {
            simulateRound();
        }
    }

    int getLevelOfMonkeyBusiness() const
    {
        std::vector<Monkey> monkeysSortedAccordingToNumItemsInspected{m_monkeys.cbegin(), m_monkeys.cend()};
        std::sort(monkeysSortedAccordingToNumItemsInspected.begin(), monkeysSortedAccordingToNumItemsInspected.end(), [](const auto& lhs, const auto& rhs)
                  {
                      return lhs.getNumItemsInspected() > rhs.getNumItemsInspected();
                  });

        const Monkey& mostActiveMonkey = *monkeysSortedAccordingToNumItemsInspected.cbegin();
        const Monkey& secondMostActiveMonkey = *(monkeysSortedAccordingToNumItemsInspected.cbegin() + 1);

        return mostActiveMonkey.getNumItemsInspected() * secondMostActiveMonkey.getNumItemsInspected();
    }

private:
    std::vector<Monkey> m_monkeys;

    void simulateRound()
    {
        for (auto& monkey : m_monkeys)
        {
            monkey.executeTurn();
        }
    }
};

using MonkeyDescriptionTextSection = std::vector<std::string>;

std::vector<int> parseStartingItemsLine(const std::string& startingItemsLine)
{
    const std::regex startingItemsLineRegex(R"(\s+Starting items: ([0-9,\s]+))");
    std::smatch startingItemsLineMatch;

    std::regex_match(startingItemsLine, startingItemsLineMatch, startingItemsLineRegex);

    const std::string startingItemsString = startingItemsLineMatch[1];

    std::vector<int> startingItems;
    Utils::splitStringIntoTypedVector(startingItemsString, startingItems, ',');

    return startingItems;
}

Operation::SharedPtr parseOperationLine(const std::string& operationLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, operationLine, boost::is_any_of(" "));

    const std::string operatorString = *(tokens.cend() - 2);
    const std::string secondOperandString = *(tokens.cend() - 1);

    if (secondOperandString == "old")
    {
        return std::make_shared<Square>();
    }

    const int secondOperand = std::stoi(secondOperandString);

    if (operatorString == "+")
    {
        return std::make_shared<Addition>(secondOperand);
    }
    else
    {
        return std::make_shared<Multiplication>(secondOperand);
    }
}

int parseTestModulusLine(const std::string& testModulusLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, testModulusLine, boost::is_any_of(" "));

    return std::stoi(tokens.back());
}

unsigned parseTestTargetLine(const std::string& testTargetLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, testTargetLine, boost::is_any_of(" "));

    return boost::lexical_cast<unsigned>(tokens.back());
}

Monkey parseMonkeyDescriptionTextSection(const MonkeyDescriptionTextSection& monkeyDescriptionTextSection)
{
    std::vector<int> startingItems = parseStartingItemsLine(monkeyDescriptionTextSection.at(1));
    Operation::SharedPtr operation = parseOperationLine(monkeyDescriptionTextSection.at(2));
    const int testModulus = parseTestModulusLine(monkeyDescriptionTextSection.at(3));
    const unsigned testTrueTarget = parseTestTargetLine(monkeyDescriptionTextSection.at(4));
    const unsigned testFalseTarget = parseTestTargetLine(monkeyDescriptionTextSection.at(5));

    return Monkey{std::move(startingItems), std::move(operation), testModulus, testTrueTarget, testFalseTarget};
}

std::vector<Monkey> parseMonkeyDescriptionLines(const std::vector<std::string>& monkeyDescriptionLines)
{
    std::vector<MonkeyDescriptionTextSection> monkeyDescriptionTextSections;
    boost::split(monkeyDescriptionTextSections, monkeyDescriptionLines, [](const auto& elem) {return elem.empty(); });

    std::vector<Monkey> monkeys;

    for (const auto& monkeyDescriptionTextSection : monkeyDescriptionTextSections)
    {
        Monkey monkey = parseMonkeyDescriptionTextSection(monkeyDescriptionTextSection);
        monkeys.push_back(std::move(monkey));
    }

    return monkeys;
}



int levelOfMonkeyBusiness(const std::vector<std::string>& monkeyDescriptionLines)
{
    std::vector<Monkey> monkeys = parseMonkeyDescriptionLines(monkeyDescriptionLines);

    MonkeyGroupSimulator monkeyGroupSimulator{monkeys};

    monkeyGroupSimulator.simulate(NUM_ROUNDS_TO_SIMULATE);

    return monkeyGroupSimulator.getLevelOfMonkeyBusiness();
}

}
}
}