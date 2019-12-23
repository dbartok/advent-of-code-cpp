#include "Day23-CategorySix.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
unsigned NUM_COMPUTERS = 50;
AdventOfCode::IntcodeNumberType DEFAULT_INPUT = -1;
size_t SPECIAL_ADDRESS = 255;
}

namespace AdventOfCode
{

struct IntcodePacket
{
    size_t destination;
    std::vector<IntcodeNumberType> messages;
};

class IntcodeComputerNetwork
{
public:
    IntcodeComputerNetwork(const std::vector<IntcodeNumberType>& program)
    {
        for (size_t i = 0; i < NUM_COMPUTERS; ++i)
        {
            IntcodeInterpreter interpreter{program};
            interpreter.addInput(i);
            m_interpreters.push_back(std::move(interpreter));
        }
    }

    void runUntilFirstPacketAtSpecialAddress()
    {
        while (true)
        {
            for (auto& interpreter : m_interpreters)
            {
                if (interpreter.isBlockedOnInput())
                {
                    interpreter.addInput(DEFAULT_INPUT);
                }

                interpreter.step();

                if (interpreter.getOutputs().size() == 3)
                {
                    size_t destination = interpreter.getOutputs().at(0);
                    IntcodeNumberType messageOne = interpreter.getOutputs().at(1);
                    IntcodeNumberType messageTwo = interpreter.getOutputs().at(2);

                    if (destination == SPECIAL_ADDRESS)
                    {
                        m_firstPacketAtSpecialAddress = messageTwo;
                        return;
                    }

                    m_interpreters.at(destination).addInput(messageOne);
                    m_interpreters.at(destination).addInput(messageTwo);
                    interpreter.clearOutputs();
                }
            }
        }
    }

    IntcodeNumberType getSecondPartOfFirstPacketAtSpecialAddress() const
    {
        return m_firstPacketAtSpecialAddress;
    }

private:
    std::vector<IntcodeInterpreter> m_interpreters;

    IntcodeNumberType m_firstPacketAtSpecialAddress;
};

IntcodeNumberType secondPartOfFirstPacketAtSpecialAddress(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeComputerNetwork network{intcodeProgram};

    network.runUntilFirstPacketAtSpecialAddress();

    return network.getSecondPartOfFirstPacketAtSpecialAddress();
}

}
