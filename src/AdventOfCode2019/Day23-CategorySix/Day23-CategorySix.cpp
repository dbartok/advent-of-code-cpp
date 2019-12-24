#include "Day23-CategorySix.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/optional.hpp>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
unsigned NUM_COMPUTERS = 50;
AdventOfCode::IntcodeNumberType DEFAULT_INPUT = -1;
size_t NAT_ADDRESS = 255;
unsigned IDLE_THRESHOLD_NUM_STEPS = 10000;
}

namespace AdventOfCode
{

struct IntcodePacket
{
    size_t destination;
    IntcodeNumberType messageOne;
    IntcodeNumberType messageTwo;
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

    void runUntilFirstNatPacket()
    {
        while (!m_natPacket.has_value())
        {
            stepAllComputers();
        }
    }

    void runUntilFirstDuplicateNatPacket()
    {
        while (true)
        {
            stepAllComputers();

            if (isSystemBlocked())
            {
                assert(m_natPacket.has_value());
                deliverPacket(*m_natPacket);

                if (m_previouslyDeliveredNatPacket.has_value() &&
                    m_natPacket->messageTwo == m_previouslyDeliveredNatPacket->messageTwo)
                {
                    break;
                }

                m_previouslyDeliveredNatPacket = m_natPacket;
            }
        }
    }

    IntcodeNumberType getSecondPartOfNatPacket() const
    {
        assert(m_natPacket.has_value());
        return m_natPacket->messageTwo;
    }

private:
    std::vector<IntcodeInterpreter> m_interpreters;

    boost::optional<IntcodePacket> m_natPacket;
    boost::optional<IntcodePacket> m_previouslyDeliveredNatPacket;

    unsigned m_numStepsSincePacketSent = 0;

    void stepAllComputers()
    {
        ++m_numStepsSincePacketSent;
        for (auto& interpreter : m_interpreters)
        {
            if (interpreter.isBlockedOnInput())
            {
                interpreter.addInput(DEFAULT_INPUT);
            }

            interpreter.step();

            if (interpreter.getOutputs().size() == 3)
            {
                processOutgoingPacket(interpreter.getOutputs());
                interpreter.clearOutputs();
            }
        }
    }

    void processOutgoingPacket(const std::vector<IntcodeNumberType>& outputs)
    {
        size_t destination = outputs.at(0);
        IntcodeNumberType messageOne = outputs.at(1);
        IntcodeNumberType messageTwo = outputs.at(2);

        IntcodePacket packet{destination, messageOne, messageTwo};

        if (destination == NAT_ADDRESS)
        {
            packet.destination = 0;
            m_natPacket = packet;
        }
        else
        {
            deliverPacket(packet);
        }
    }

    void deliverPacket(const IntcodePacket& packet)
    {
        m_interpreters.at(packet.destination).addInput(packet.messageOne);
        m_interpreters.at(packet.destination).addInput(packet.messageTwo);
        m_numStepsSincePacketSent = 0;
    }

    bool isSystemBlocked() const
    {
        return m_numStepsSincePacketSent > IDLE_THRESHOLD_NUM_STEPS;
    }
};

IntcodeNumberType secondPartOfFirstPacketAtSpecialAddress(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeComputerNetwork network{intcodeProgram};

    network.runUntilFirstNatPacket();

    return network.getSecondPartOfNatPacket();
}

IntcodeNumberType secondPartOfFirstDuplicateMessageByNat(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeComputerNetwork network{intcodeProgram};

    network.runUntilFirstDuplicateNatPacket();

    return network.getSecondPartOfNatPacket();
}

}
