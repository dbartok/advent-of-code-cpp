#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/optional.hpp>

#include <unordered_map>
#include <memory>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using RegisterValueType = long long;

enum class ExecutionState
{
    RUNNING,
    TERMINATED
};

struct AssemblyProgramState
{
public:
    AssemblyProgramState() noexcept;

    RegisterValueType getRegisterValue(const std::string& registerName) const;
    std::shared_ptr<RegisterValueType> getRegisterValueSharedPtr(const std::string& registerName);

    ExecutionState& executionState() noexcept;
    int& instructionIndex() noexcept;
    boost::optional<int>& lastPlayedFrequency() noexcept;
    const boost::optional<int>& lastPlayedFrequency() const noexcept;

private:
    using RegisterNameToValueMap = std::unordered_map<std::string, std::shared_ptr<RegisterValueType>>;

    ExecutionState m_executionState;
    int m_instructionIndex;
    RegisterNameToValueMap m_registerNameToValue;
    boost::optional<int> m_lastPlayedFrequency;
};

}
