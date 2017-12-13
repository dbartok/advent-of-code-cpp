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
    if (m_name == "inc")
    {
        state.addToRegister(m_target, m_argument);
    }
    else if (m_name == "dec")
    {
        state.substractFromRegister(m_target, m_argument);
    }
    else
    {
        throw std::runtime_error("Unknown operation name.");
    }
}

}
