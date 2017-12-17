#pragma once

#include "State.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <functional>
END_LIBRARIES_DISABLE_WARNINGS

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
