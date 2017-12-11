#pragma once

#include <unordered_map>

class State
{
public:
    using RegisterValueToNameMap = std::unordered_map<std::string, int>;

    const RegisterValueToNameMap& getRegisterValueToNameMap() const;

    // All of the register modifiers and accessors treat the register as 0 if it hasn't been encountered before
    void addToRegister(const std::string& registerName, int addend);
    void substractFromRegister(const std::string& registerName, int subtrahend);
    int registerValue(const std::string& registerName) const;

    int getGlobalMaxValueOfRegisters() const;

private:
    RegisterValueToNameMap m_registerValueToNameMap;
    int m_globalMaxValueOfRegisters = std::numeric_limits<int>::min();
};
