#pragma once

#include "AssemblyProgram.h"
#include "AssemblyInstruction.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <unordered_set>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class AssemblyProgramParser
{
public:
    AssemblyProgramParser(std::vector<std::string> instructionStrings) noexcept;
    AssemblyProgram createProgram();

private:
    std::vector<std::string> m_instructionStrings;

    bool validateInstruction(const std::string& instructionName, const std::vector<std::string>& args);
    AssemblyInstruction::SharedPtr createInstruction(const std::string& instructionName, const std::vector<std::string>& args);
};

}
