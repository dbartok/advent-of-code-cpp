#include "State.h"

const State::RegisterValueToNameMap& State::getRegisterValueToNameMap() const
{
    return m_registerValueToNameMap;
}

int& State::registerValue(const std::string& registerName)
{
    return m_registerValueToNameMap[registerName];
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
