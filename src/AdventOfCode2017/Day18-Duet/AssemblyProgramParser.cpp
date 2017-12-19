#include "AssemblyProgramParser.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

AssemblyProgramParser::AssemblyProgramParser(std::vector<std::string> instructionStrings) noexcept
    : m_instructionStrings{std::move(instructionStrings)}
{

}

AssemblyProgram AssemblyProgramParser::createProgram()
{
    std::vector<AssemblyInstruction::SharedPtr> instructions;

    for (const auto& instructionString : m_instructionStrings)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, instructionString, boost::is_any_of(" "));

        if (tokens.empty())
        {
            throw std::runtime_error("No tokens in instruction.");
        }

        std::string instructionName = std::move(tokens[0]);
        std::vector<std::string> args{std::make_move_iterator(tokens.begin() + 1), std::make_move_iterator(tokens.end())};

        AssemblyInstruction::SharedPtr instruction = createInstruction(instructionName, args);
        if (!instruction)
        {
            throw std::runtime_error("Unable to parse instruction: " + instructionString);
        }

        instructions.push_back(std::move(instruction));
    }

    return AssemblyProgram{instructions};
}

bool AssemblyProgramParser::validateInstruction(const std::string & instructionName, const std::vector<std::string> & args)
{
    if (instructionName == "set" || instructionName == "add" || instructionName == "mul" || instructionName == "mod" || instructionName == "jgz")
    {
        if (args.size() != 2)
        {
            return false;
        }
    }
    else
    {
        if (args.size() != 1)
        {
            return false;
        }
    }

    return true;
}

AssemblyInstruction::SharedPtr AssemblyProgramParser::createInstruction(const std::string& instructionName, const std::vector<std::string>& args)
{
    if (!validateInstruction(instructionName, args))
    {
        return nullptr;
    }

    if (instructionName == "set")
    {
        return std::make_shared<SetInstruction>(args[0], args[1]);
    }
    else if (instructionName == "add")
    {
        return std::make_shared<AddInstruction>(args[0], args[1]);
    }
    else if (instructionName == "mul")
    {
        return std::make_shared<MultiplyInstruction>(args[0], args[1]);
    }
    else if (instructionName == "mod")
    {
        return std::make_shared<ModuloInstruction>(args[0], args[1]);
    }
    else if (instructionName == "jgz")
    {
        return std::make_shared<JumpGreaterThanZeroInstruction>(args[0], args[1]);
    }
    else if (instructionName == "snd")
    {
        return std::make_shared<SoundInstruction>(args[0]);
    }
    else if (instructionName == "rcv")
    {
        return std::make_shared<RecoverInstruction>(args[0]);
    }
    else
    {
        return nullptr;
    }
}

}
