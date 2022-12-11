#include "Day11-MonkeyInTheMiddle.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/integer/common_factor.hpp>

#include <regex>
#include <memory>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int WORRY_LEVEL_DIVISOR_FIRST_PART = 3;
const int WORRY_LEVEL_DIVISOR_SECOND_PART = 1;
const unsigned NUM_ROUNDS_TO_SIMULATE_FIRST_PART = 20;
const unsigned NUM_ROUNDS_TO_SIMULATE_SECOND_PART = 10'000;

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

    virtual LargeNumberType apply(LargeNumberType operand) const = 0;
};

class Addition : public Operation
{
public:
    Addition(int addend)
        : m_factor{addend}
    {

    }

    LargeNumberType apply(LargeNumberType operand) const override
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

    LargeNumberType apply(LargeNumberType operand) const override
    {
        return operand * m_factor;
    }

private:
    int m_factor;
};

class Square : public Operation
{
public:
    LargeNumberType apply(LargeNumberType operand) const override
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

    void setStorageModulus(int storageModulus)
    {
        m_storageModulus = storageModulus;
    }

    void executeTurn(int worryLevelDivisor)
    {
        while (!m_currentItems.empty())
        {
            const LargeNumberType item = m_currentItems.front();
            m_currentItems.pop_front();
            processItem(item, worryLevelDivisor);
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

    int getTestModulus() const
    {
        return m_testModulus;
    }

private:
    std::deque<LargeNumberType> m_currentItems;
    Operation::SharedPtr m_operation;
    int m_testModulus;
    unsigned m_testTrueTarget;
    unsigned m_testFalseTarget;

    std::vector<Monkey>* m_allMonkeys;
    int m_storageModulus = -1;

    int m_numItemsInspected = 0;

    void processItem(LargeNumberType item, int worryLevelDivisor)
    {
        LargeNumberType newWorryLevel = m_operation->apply(item);
        newWorryLevel /= worryLevelDivisor;
        newWorryLevel %= m_storageModulus;

        const unsigned target = (newWorryLevel % m_testModulus == 0) ? m_testTrueTarget : m_testFalseTarget;
        m_allMonkeys->at(target).receiveItem(newWorryLevel);

        ++m_numItemsInspected;
    }
};

class MonkeyGroupSimulator
{
public:
    MonkeyGroupSimulator(std::vector<Monkey> monkeys, int worryLevelDivisor)
        : m_monkeys{std::move(monkeys)}
        , m_worryLevelDivisor{worryLevelDivisor}
    {
        const int storageModulus = determineStorageModulus();

        for (auto& monkey : m_monkeys)
        {
            monkey.setAllMonkeys(&m_monkeys);
            monkey.setStorageModulus(storageModulus);
        }
    }

    void simulate(unsigned numRounds)
    {
        for (unsigned iteration = 0; iteration < numRounds; ++iteration)
        {
            simulateRound();
        }
    }

    int64_t getLevelOfMonkeyBusiness() const
    {
        std::vector<Monkey> monkeysSortedAccordingToNumItemsInspected{m_monkeys.cbegin(), m_monkeys.cend()};
        std::sort(monkeysSortedAccordingToNumItemsInspected.begin(), monkeysSortedAccordingToNumItemsInspected.end(), [](const auto& lhs, const auto& rhs)
                  {
                      return lhs.getNumItemsInspected() > rhs.getNumItemsInspected();
                  });

        const Monkey& mostActiveMonkey = *monkeysSortedAccordingToNumItemsInspected.cbegin();
        const Monkey& secondMostActiveMonkey = *(monkeysSortedAccordingToNumItemsInspected.cbegin() + 1);

        return int64_t{mostActiveMonkey.getNumItemsInspected()} * secondMostActiveMonkey.getNumItemsInspected();
    }

private:
    std::vector<Monkey> m_monkeys;
    int m_worryLevelDivisor;

    void simulateRound()
    {
        for (auto& monkey : m_monkeys)
        {
            monkey.executeTurn(m_worryLevelDivisor);
        }
    }

    int determineStorageModulus() const
    {
        return std::accumulate(m_monkeys.cbegin(), m_monkeys.cend(), 1, [](auto acc, const auto& monkey)
                               {
                                   return boost::integer::lcm(acc, monkey.getTestModulus());
                               });

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

int64_t levelOfMonkeyBusiness(const std::vector<std::string>& monkeyDescriptionLines)
{
    std::vector<Monkey> monkeys = parseMonkeyDescriptionLines(monkeyDescriptionLines);

    MonkeyGroupSimulator monkeyGroupSimulator{monkeys, WORRY_LEVEL_DIVISOR_FIRST_PART};

    monkeyGroupSimulator.simulate(NUM_ROUNDS_TO_SIMULATE_FIRST_PART);

    return monkeyGroupSimulator.getLevelOfMonkeyBusiness();
}

int64_t levelOfMonkeyBusinessWithRidiculousWorryLevels(const std::vector<std::string>& monkeyDescriptionLines)
{
    std::vector<Monkey> monkeys = parseMonkeyDescriptionLines(monkeyDescriptionLines);

    MonkeyGroupSimulator monkeyGroupSimulator{monkeys, WORRY_LEVEL_DIVISOR_SECOND_PART};

    monkeyGroupSimulator.simulate(NUM_ROUNDS_TO_SIMULATE_SECOND_PART);

    return monkeyGroupSimulator.getLevelOfMonkeyBusiness();
}

}
}
}