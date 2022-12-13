#include "Day13-DistressSignal.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2022
{
namespace Day13
{

class Packet;
using PacketSharedPtr = std::shared_ptr<Packet>;
using Packets = std::vector<PacketSharedPtr>;

class Packet
{
public:
    virtual Packets getContainedPackets() const = 0;
    virtual boost::optional<int> getScalarValue() const = 0;

private:
};


class ListPacket : public Packet
{
public:
    ListPacket(Packets containedPackets)
        : m_containedPackets{std::move(containedPackets)}
    {

    }

    Packets getContainedPackets() const override
    {
        return m_containedPackets;
    }

    boost::optional<int> getScalarValue() const override
    {
        return boost::none;
    }

private:
    Packets m_containedPackets;
};

class IntegerPacket : public Packet
{
public:
    IntegerPacket(int value)
        : m_value{value}
    {

    }

    Packets getContainedPackets() const override
    {
        throw std::runtime_error("Integer packets contain no further packets");
    }

    boost::optional<int> getScalarValue() const override
    {
        return m_value;
    }

private:
    int m_value;
};

using PacketPair = std::pair<PacketSharedPtr, PacketSharedPtr>;
using PacketPairs = std::vector<PacketPair>;

class PacketParser
{
public:
    PacketParser(std::string packetString)
        : m_packetString{std::move(packetString)}
    {
        std::string numberTokenBuffer;
        for (auto c : m_packetString)
        {
            if (isdigit(c))
            {
                numberTokenBuffer += c;
            }
            else
            {
                if (!numberTokenBuffer.empty())
                {
                    m_tokens.push_back(std::move(numberTokenBuffer));
                    numberTokenBuffer.clear();
                }
                if (c != ',')
                {
                    m_tokens.push_back(std::string{c});
                }
            }
        }
    }

    void parse()
    {
        // Skip outermost brackets because the root packet is precreated already
        getNextToken();

        Packets containedPackets = parseContainedPackets();
        m_rootPacket = std::make_shared<ListPacket>(std::move(containedPackets));
    }

    PacketSharedPtr getRootPacket()
    {
        return m_rootPacket;
    }

private:
    std::string m_packetString;
    std::vector<std::string> m_tokens;
    size_t m_currentTokenIndex = 0;

    PacketSharedPtr m_rootPacket = nullptr;

    Packets parseContainedPackets()
    {
        Packets containedPackets;

        while (true)
        {
            std::string token = getNextToken();

            if (std::all_of(token.cbegin(), token.cend(), isdigit))
            {
                PacketSharedPtr packet = std::make_shared<IntegerPacket>(std::stoi(token));

                containedPackets.push_back(std::move(packet));
            }
            else if (token == "[")
            {
                Packets containedSubpackets = parseContainedPackets();
                PacketSharedPtr packet = std::make_shared<ListPacket>(std::move(containedSubpackets));

                containedPackets.push_back(std::move(packet));
            }
            else if (token == "]")
            {
                break;
            }
        }

        return containedPackets;
    }

    std::string getNextToken()
    {
        return m_tokens.at(m_currentTokenIndex++);
    }
};

using PacketPairTextSection = std::vector<std::string>;

PacketSharedPtr parsePacketLine(const std::string& packetLine)
{
    PacketParser packetParser(packetLine);
    packetParser.parse();
    return packetParser.getRootPacket();
}

PacketPair parsePacketPairTextSection(const PacketPairTextSection& packetPairTextSection)
{
    PacketSharedPtr firstPacket = parsePacketLine(packetPairTextSection.at(0));
    PacketSharedPtr secondPacket = parsePacketLine(packetPairTextSection.at(1));

    return PacketPair{std::move(firstPacket), std::move(secondPacket)};
}

PacketPairs parseDistressSignalLines(const std::vector<std::string>& distressSignalLines)
{
    std::vector<PacketPairTextSection> packetPairTextSections;
    boost::split(packetPairTextSections, distressSignalLines, [](const auto& elem) { return elem.empty(); });

    PacketPairs packetPairs;

    for (const auto& packetPairTextSection : packetPairTextSections)
    {
        PacketPair packetPair = parsePacketPairTextSection(packetPairTextSection);
        packetPairs.push_back(std::move(packetPair));
    }

    return packetPairs;
}

PacketSharedPtr wrapInListPacket(const PacketSharedPtr& packet)
{
    return std::make_shared<ListPacket>(std::vector<PacketSharedPtr>{packet});
}

boost::optional<bool> arePacketsInOrder(const PacketSharedPtr& firstPacket, const PacketSharedPtr& secondPacket)
{
    Packets subpacketsInFirstPacket = firstPacket->getContainedPackets();
    Packets subpacketsInSecondPacket = secondPacket->getContainedPackets();

    for (size_t packetIndex = 0; ; ++packetIndex)
    {
        if (packetIndex == subpacketsInFirstPacket.size() && packetIndex == subpacketsInSecondPacket.size())
        {
            return boost::none;
        }

        if (packetIndex == subpacketsInFirstPacket.size())
        {
            return true;
        }

        if (packetIndex == subpacketsInSecondPacket.size())
        {
            return false;
        }

        PacketSharedPtr firstSubpacket = subpacketsInFirstPacket.at(packetIndex);
        PacketSharedPtr secondSubpacket = subpacketsInSecondPacket.at(packetIndex);

        const boost::optional<int> firstSubpacketScalarValue = firstSubpacket->getScalarValue();
        const boost::optional<int> secondSubpacketScalarValue = secondSubpacket->getScalarValue();

        if (firstSubpacketScalarValue && secondSubpacketScalarValue)
        {
            if (firstSubpacketScalarValue.value() != secondSubpacketScalarValue.value())
            {
                return firstSubpacketScalarValue.value() < secondSubpacketScalarValue.value();
            }
        }
        // At least one list packet
        else
        {
            if (firstSubpacketScalarValue)
            {
                firstSubpacket = wrapInListPacket(firstSubpacket);
            }

            if (secondSubpacketScalarValue)
            {
                secondSubpacket = wrapInListPacket(secondSubpacket);
            }

            const boost::optional<bool> orderResult = arePacketsInOrder(firstSubpacket, secondSubpacket);
            if (orderResult.has_value())
            {
                return orderResult;
            }
        }
    }
}

int sumOfIndicesOfPairsInRightOrder(const std::vector<std::string>& distressSignalLines)
{
    PacketPairs packetPairs = parseDistressSignalLines(distressSignalLines);

    int sum = 0;

    for (size_t i = 0; i < packetPairs.size(); ++i)
    {
        const boost::optional<bool> orderResult = arePacketsInOrder(packetPairs.at(i).first, packetPairs.at(i).second);
        if (orderResult.get_value_or(true))
        {
            sum += (i + 1);
        }
    }

    return sum;
}

}
}
}