#include "State.h"

#include <algorithm>

const State::RegisterValueToNameMap& State::getRegisterValueToNameMap() const
{
    return m_registerValueToNameMap;
}

void State::addToRegister(const std::string& registerName, int addend)
{
    int& registerValue = m_registerValueToNameMap[registerName];
    registerValue += addend;

    m_globalMaxValueOfRegisters = std::max(m_globalMaxValueOfRegisters, registerValue);
}

void State::substractFromRegister(const std::string& registerName, int subtrahend)
{
    int& registerValue = m_registerValueToNameMap[registerName];
    registerValue -= subtrahend;

    m_globalMaxValueOfRegisters = std::max(m_globalMaxValueOfRegisters, registerValue);
}

int State::registerValue(const std::string& registerName) const
{
    auto foundIter = m_registerValueToNameMap.find(registerName);
    if (foundIter == m_registerValueToNameMap.end())
    {
        return 0;
    }

    return foundIter->second;
}

int State::getGlobalMaxValueOfRegisters() const
{
    return m_globalMaxValueOfRegisters;
}