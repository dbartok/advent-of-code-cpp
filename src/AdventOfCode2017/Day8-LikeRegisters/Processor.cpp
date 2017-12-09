#include "Processor.h"

#include <algorithm>

namespace AdventOfCode
{

Processor::Processor(std::vector<Instruction> instructions)
    : m_instructions{instructions}
    , m_state{}
{

}

void Processor::run()
{
    for (const auto& instruction : m_instructions)
    {
        instruction.execute(m_state);
    }
}

int Processor::largestRegister() const
{
    const auto& registerValueToNameMap = m_state.getRegisterValueToNameMap();
    if (registerValueToNameMap.empty())
    {
        throw std::runtime_error("There are no registers.");
    }

    auto maxValueIter = std::max_element(registerValueToNameMap.begin(), registerValueToNameMap.end(), [](const auto& lhs, const auto& rhs)
    {
        return lhs.second < rhs.second;
    });

    return maxValueIter->second;
}

int Processor::globalMaxValueOfRegisters() const
{
    return m_state.getGlobalMaxValueOfRegisters();
}

}
