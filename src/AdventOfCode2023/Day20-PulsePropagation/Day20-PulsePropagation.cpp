#include "Day20-PulsePropagation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

#include <memory>
#include <unordered_map>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned NUM_BUTTON_PRESSES_PART_ONE = 1000;

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day20
{

class Module;
using ModuleSharedPtr = std::shared_ptr<Module>;
using ModuleNameToModuleSharedPtr = std::unordered_map<std::string, ModuleSharedPtr>;

enum class PulseHeight
{
    LOW = 0,
    HIGH = 1,
};

class Module
{
public:
    Module(std::vector<std::string> outputModuleNames)
        : m_outputModuleNames{std::move(outputModuleNames)}
    {

    }

    const std::vector<std::string>& getOutputModuleNames() const
    {
        return m_outputModuleNames;
    }

    virtual void addInputModule(const std::string& inputModuleName) = 0;
    virtual boost::optional<PulseHeight> processIncomingPulse(const std::string& sourceModuleName, PulseHeight height) = 0;

    virtual ~Module()
    {

    }

private:
    std::vector<std::string> m_outputModuleNames;
};


class FlipFlopModule : public Module
{
public:
    using Module::Module;

    void addInputModule(const std::string& inputModuleName) override
    {
        // Flip-flops don't need to store any data about their inputs
    }

    boost::optional<PulseHeight> processIncomingPulse(const std::string& sourceModuleName, PulseHeight height) override
    {
        if (height == PulseHeight::HIGH)
        {
            return boost::none;
        }
        else
        {
            if (m_isOn)
            {
                m_isOn = false;
                return PulseHeight::LOW;
            }
            else
            {
                m_isOn = true;
                return PulseHeight::HIGH;
            }
        }
    }

private:
    bool m_isOn = false;
};

class ConjunctionModule : public Module
{
public:
    using Module::Module;

    void addInputModule(const std::string& inputModuleName) override
    {
        m_inputModuleNameToMostRecentPulse.emplace(inputModuleName, PulseHeight::LOW);
    }

    boost::optional<PulseHeight> processIncomingPulse(const std::string& sourceModuleName, PulseHeight height) override
    {
        m_inputModuleNameToMostRecentPulse.at(sourceModuleName) = height;

        if (std::all_of(m_inputModuleNameToMostRecentPulse.cbegin(), m_inputModuleNameToMostRecentPulse.cend(), [](const auto& inputModuleNameAndMostRecentPulse)
                        {
                            return inputModuleNameAndMostRecentPulse.second == PulseHeight::HIGH;
                        }))
        {
            return PulseHeight::LOW;
        }
        else
        {
            return PulseHeight::HIGH;
        }
    }

private:
    std::unordered_map<std::string, PulseHeight> m_inputModuleNameToMostRecentPulse;
};

class BroadcastModule : public Module
{
public:
    using Module::Module;

    void addInputModule(const std::string& inputModuleName) override
    {
        throw std::runtime_error("The broadcast module cannot accept inputs.");
    }

    boost::optional<PulseHeight> processIncomingPulse(const std::string& sourceModuleName, PulseHeight height) override
    {
        return height;
    }
};

class OutputModule : public Module
{
public:
    using Module::Module;

    void addInputModule(const std::string& inputModuleName) override
    {
        // Output modules don't need to store any data about their inputs
    }

    boost::optional<PulseHeight> processIncomingPulse(const std::string& sourceModuleName, PulseHeight height) override
    {
        return boost::none;
    }
};

struct Pulse
{
    std::string sourceModuleName;
    std::string destinationModuleName;
    PulseHeight height;

    Pulse(std::string sourceModuleName, std::string destinationModuleName, PulseHeight height)
        : sourceModuleName{std::move(sourceModuleName)}
        , destinationModuleName{std::move(destinationModuleName)}
        , height{std::move(height)}
    {

    }
};

class PulseProcessor
{
public:
    PulseProcessor(ModuleNameToModuleSharedPtr moduleNameToModuleSharedPtr)
        : m_moduleNameToModuleSharedPtr{std::move(moduleNameToModuleSharedPtr)}
    {

    }

    void processRepeatedButtonPresses(unsigned numButtonPresses)
    {
        for (int i = 0; i < numButtonPresses; ++i)
        {
            processSingleButtonPress();
        }
    }

    int getNumLowAndHighPulsesMultipliedTogether() const
    {
        return m_numLowPulses * m_numHighPulses;
    }

private:
    ModuleNameToModuleSharedPtr m_moduleNameToModuleSharedPtr;

    int m_numLowPulses = 0;
    int m_numHighPulses = 0;

    void processSingleButtonPress()
    {
        std::queue<Pulse> signalQueue;
        signalQueue.emplace("button", "broadcaster", PulseHeight::LOW);

        while (!signalQueue.empty())
        {
            Pulse currentPulse = signalQueue.front();
            signalQueue.pop();

            if (currentPulse.height == PulseHeight::LOW)
            {
                ++m_numLowPulses;
            }
            else
            {
                ++m_numHighPulses;
            }

            auto destinationModuleSharedPtr = m_moduleNameToModuleSharedPtr.at(currentPulse.destinationModuleName);
            boost::optional<PulseHeight> outgoingPulseHeight = destinationModuleSharedPtr->processIncomingPulse(currentPulse.sourceModuleName, currentPulse.height);

            if (!outgoingPulseHeight)
            {
                continue;
            }

            for (const auto& newSignalOutputModuleName : destinationModuleSharedPtr->getOutputModuleNames())
            {
                signalQueue.emplace(currentPulse.destinationModuleName, newSignalOutputModuleName, outgoingPulseHeight.get());
            }
        }
    }
};

ModuleSharedPtr createModuleSharedPtr(std::vector<std::string> outputModuleNames, char op)
{
    switch (op)
    {
        case '%':
            return std::make_shared<FlipFlopModule>(outputModuleNames);
        case '&':
            return std::make_shared<ConjunctionModule>(outputModuleNames);
        case 'b':
            return std::make_shared<BroadcastModule>(outputModuleNames);
        default:
            throw std::runtime_error("Invalid operator: " + std::string{op});
    }
}


std::pair<std::string, ModuleSharedPtr> parseModuleLine(const std::string& moduleLine)
{
    std::vector<std::string> moduleLineTokens;
    boost::split(moduleLineTokens, moduleLine, boost::is_any_of("->"), boost::token_compress_on);

    std::string outputNamesToken = moduleLineTokens.at(1);
    boost::trim(outputNamesToken);
    std::vector<std::string> outputNames;
    boost::split(outputNames, outputNamesToken, boost::is_any_of(", "), boost::token_compress_on);

    std::string name = moduleLineTokens.at(0);
    boost::trim(name);
    if (!isalpha(name.front()))
    {
        name = name.substr(1, name.size() - 1);
    }

    ModuleSharedPtr moduleSharedPtr = createModuleSharedPtr(std::move(outputNames), moduleLine.front());

    return {std::move(name), std::move(moduleSharedPtr)};
}

PulseProcessor parseModuleLines(const std::vector<std::string>& moduleLines)
{
    ModuleNameToModuleSharedPtr moduleNameToModuleSharedPtr;

    for (const auto& moduleLine : moduleLines)
    {
        std::pair<std::string, ModuleSharedPtr> moduleNameAndModuleSharedPtr = parseModuleLine(moduleLine);
        moduleNameToModuleSharedPtr.insert(std::move(moduleNameAndModuleSharedPtr));
    }

    // Add inputs
    for (const auto& moduleNameAndModuleSharedPtr : moduleNameToModuleSharedPtr)
    {
        const std::string& moduleName = moduleNameAndModuleSharedPtr.first;
        const ModuleSharedPtr& moduleSharedPtr = moduleNameAndModuleSharedPtr.second;

        for (const auto& outputModuleName : moduleSharedPtr->getOutputModuleNames())
        {
            const auto outputModuleSharedPtrIter = moduleNameToModuleSharedPtr.find(outputModuleName);

            // Found an output-only module
            if (outputModuleSharedPtrIter == moduleNameToModuleSharedPtr.cend())
            {
                moduleNameToModuleSharedPtr.emplace(outputModuleName, std::make_shared<OutputModule>(std::vector<std::string>{}));
            }
            else
            {
                outputModuleSharedPtrIter->second->addInputModule(moduleName);
            }
        }
    }

    return {moduleNameToModuleSharedPtr};
}


int numLowAndHighPulsesMultipliedTogether(const std::vector<std::string>& moduleLines)
{
    PulseProcessor pulseProcessor = parseModuleLines(moduleLines);

    pulseProcessor.processRepeatedButtonPresses(NUM_BUTTON_PRESSES_PART_ONE);

    return pulseProcessor.getNumLowAndHighPulsesMultipliedTogether();
}

}
}
}