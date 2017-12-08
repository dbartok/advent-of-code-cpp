#pragma once

#include "State.h"

#include <functional>

namespace AdventOfCode
{

class Operation
{
public:
    Operation(std::string target, std::string name, int argument);
    void execute(State& state) const;

private:
    std::string m_target;
    std::string m_name;
    int m_argument;
};

}
