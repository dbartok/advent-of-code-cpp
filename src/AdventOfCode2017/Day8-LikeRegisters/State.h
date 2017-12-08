#pragma once

#include <unordered_map>

class State
{
public:
    using RegisterValueToNameMap = std::unordered_map<std::string, int>;

    const RegisterValueToNameMap& getRegisterValueToNameMap() const;

    // Both of these return 0 if the register was not encountered before
    int& registerValue(const std::string& registerName);
    int registerValue(const std::string& registerName) const;

private:
    RegisterValueToNameMap m_registerValueToNameMap;
};
