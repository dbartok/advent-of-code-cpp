#include "AssemblyProgramState.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

AssemblyProgramState::AssemblyProgramState() noexcept
    : m_executionState{ExecutionState::RUNNING}
    , m_instructionIndex{0}
    , m_registerNameToValue{}
    , m_lastPlayedFrequency{}
{

}

ExecutionState& AssemblyProgramState::executionState() noexcept
{
    return m_executionState;
}
int& AssemblyProgramState::instructionIndex() noexcept
{
    return m_instructionIndex;
}

boost::optional<int>& AssemblyProgramState::lastPlayedFrequency() noexcept
{
    return m_lastPlayedFrequency;
}

const boost::optional<int>& AssemblyProgramState::lastPlayedFrequency() const noexcept
{
    return m_lastPlayedFrequency;
}

RegisterValueType AssemblyProgramState::getRegisterValue(const std::string& registerName) const
{
    const auto foundIter = m_registerNameToValue.find(registerName);
    if (foundIter == m_registerNameToValue.cend())
    {
        return 0;
    }

    return *foundIter->second;
}

std::shared_ptr<RegisterValueType> AssemblyProgramState::getRegisterValueSharedPtr(const std::string& registerName)
{
    auto foundIter = m_registerNameToValue.find(registerName);
    if (foundIter == m_registerNameToValue.end())
    {
        std::tie(foundIter, std::ignore) = m_registerNameToValue.insert(std::make_pair(registerName, std::make_shared<RegisterValueType>(0)));
    }

    return foundIter->second;
}

}
