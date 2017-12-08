#include "Operation.h"

namespace AdventOfCode
{

Operation::Operation(std::string target, std::string name, int argument)
    : m_target{std::move(target)}
    , m_name{std::move(name)}
    , m_argument{argument}
{

}

void Operation::execute(State& state) const
{
    int& targetRegisterValue = state.registerValue(m_target);

    if (m_name == "inc")
    {
        targetRegisterValue += m_argument;
    }
    else if(m_name == "dec")
    {
        targetRegisterValue -= m_argument;
    }
    else
    {
        throw std::runtime_error("Unknown operation name.");
    }
}

}
