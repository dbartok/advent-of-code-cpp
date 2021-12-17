#include "Day16-PacketDecoder.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <sstream>
#include <bitset>
#include <memory>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

size_t VERSION_NUMBER_NUM_BITS = 3;
size_t TYPE_ID_NUM_BITS = 3;
size_t LITERAL_CHUNK_NUM_BITS = 4;
size_t OPERATOR_PACKET_TOTAL_LENGTH_NUM_BITS = 15;
size_t OPERATOR_PACKET_NUM_SUBPACKETS_NUM_BITS = 11;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day16
{

class Packet
{
public:
    using SharedPtr = std::shared_ptr<Packet>;

    Packet(int64_t versionNumber)
        : m_versionNumber{versionNumber}
    {

    }

    virtual int64_t getSumOfAllContainedVersionNumbers() const = 0;
    virtual int64_t evaluate() const = 0;

    virtual ~Packet()
    {

    }

protected:
    int64_t m_versionNumber;
};

class LiteralPacket : public Packet
{
public:
    LiteralPacket(int64_t versionNumber, int64_t literal)
        : Packet{versionNumber}
        , m_literal{literal}
    {

    }

    int64_t getSumOfAllContainedVersionNumbers() const override
    {
        return m_versionNumber;
    }

    int64_t evaluate() const override
    {
        return m_literal;
    }

    virtual ~LiteralPacket()
    {

    }

private:
    int64_t m_literal;
};

class OperatorPacket : public Packet
{
public:
    OperatorPacket(int64_t versionNumber, std::vector<Packet::SharedPtr> containedPacketPtrs)
        : Packet{versionNumber}
        , m_containedPacketPtrs{std::move(containedPacketPtrs)}
    {

    }

    int64_t getSumOfAllContainedVersionNumbers() const override
    {
        return std::accumulate(m_containedPacketPtrs.cbegin(), m_containedPacketPtrs.cend(), m_versionNumber, [](const auto& acc, const auto& packetPtr)
                               {
                                   return acc + packetPtr->getSumOfAllContainedVersionNumbers();
                               });
    }

    virtual int64_t evaluate() const = 0;

    virtual ~OperatorPacket()
    {

    }

protected:
    std::vector<Packet::SharedPtr> m_containedPacketPtrs;
};

class SumOperatorPacket : public OperatorPacket
{
public:
    using OperatorPacket::OperatorPacket;

    int64_t evaluate() const override
    {
        return std::accumulate(m_containedPacketPtrs.cbegin(), m_containedPacketPtrs.cend(), 0ll, [](const auto& acc, const auto& packetPtr)
                               {
                                   return acc + packetPtr->evaluate();
                               });
    }

    virtual ~SumOperatorPacket()
    {

    }
};

class ProductOperatorPacket : public OperatorPacket
{
public:
    using OperatorPacket::OperatorPacket;

    int64_t evaluate() const override
    {
        return std::accumulate(m_containedPacketPtrs.cbegin(), m_containedPacketPtrs.cend(), 1ll, [](const auto& acc, const auto& packetPtr)
                               {
                                   return acc * packetPtr->evaluate();
                               });
    }

    virtual ~ProductOperatorPacket()
    {

    }
};

class MinimumOperatorPacket : public OperatorPacket
{
public:
    using OperatorPacket::OperatorPacket;

    int64_t evaluate() const override
    {
        return std::accumulate(m_containedPacketPtrs.cbegin(), m_containedPacketPtrs.cend(), std::numeric_limits<int64_t>::max(), [](const auto& acc, const auto& packetPtr)
                               {
                                   return std::min(acc, packetPtr->evaluate());
                               });
    }

    virtual ~MinimumOperatorPacket()
    {

    }
};

class MaximumOperatorPacket : public OperatorPacket
{
public:
    using OperatorPacket::OperatorPacket;

    int64_t evaluate() const override
    {
        return std::accumulate(m_containedPacketPtrs.cbegin(), m_containedPacketPtrs.cend(), std::numeric_limits<int64_t>::min(), [](const auto& acc, const auto& packetPtr)
                               {
                                   return std::max(acc, packetPtr->evaluate());
                               });
    }

    virtual ~MaximumOperatorPacket()
    {

    }
};

class TwoOperandOperatorPacket : public OperatorPacket
{
public:
    using OperatorPacket::OperatorPacket;

    int64_t evaluate() const override
    {
        if (m_containedPacketPtrs.size() != 2)
        {
            throw std::runtime_error("A two operand operator must have exactly two operands");
        }

        const auto& lhsPacketPtr = m_containedPacketPtrs.at(0);
        const auto& rhsPacketPtr = m_containedPacketPtrs.at(1);

        return evaluateForTwoOperands(lhsPacketPtr->evaluate(), rhsPacketPtr->evaluate());
    }

    virtual int64_t evaluateForTwoOperands(int64_t lhs, int64_t rhs) const = 0;

    virtual ~TwoOperandOperatorPacket()
    {

    }
};

class GreaterThanOperatorPacket : public TwoOperandOperatorPacket
{
public:
    using TwoOperandOperatorPacket::TwoOperandOperatorPacket;

    virtual int64_t evaluateForTwoOperands(int64_t lhs, int64_t rhs) const
    {
        return lhs > rhs;
    }

    virtual ~GreaterThanOperatorPacket()
    {

    }
};

class LessThanOperatorPacket : public TwoOperandOperatorPacket
{
public:
    using TwoOperandOperatorPacket::TwoOperandOperatorPacket;

    virtual int64_t evaluateForTwoOperands(int64_t lhs, int64_t rhs) const
    {
        return lhs < rhs;
    }

    virtual ~LessThanOperatorPacket()
    {

    }
};

class EqualToOperatorPacket : public TwoOperandOperatorPacket
{
public:
    using TwoOperandOperatorPacket::TwoOperandOperatorPacket;

    virtual int64_t evaluateForTwoOperands(int64_t lhs, int64_t rhs) const
    {
        return lhs == rhs;
    }

    virtual ~EqualToOperatorPacket()
    {

    }
};

enum class PacketTypeID
{
    SUM = 0,
    PRODUCT = 1,
    MINIMUM = 2,
    MAXIMUM = 3,
    LITERAL = 4,
    GREATER_THAN = 5,
    LESS_THAN = 6,
    EQUAL_TO = 7,
};

class BitsTransmissionDecoder
{
public:
    BitsTransmissionDecoder(std::string binaryTransmissionString)
        : m_binaryTransmissionString{std::move(binaryTransmissionString)}
    {

    }

    Packet::SharedPtr decodePacket()
    {
        const int64_t versionNumber = decodeVersionNumber();
        const int64_t typeID = decodeTypeID();

        Packet::SharedPtr packet;
        if (typeID == static_cast<int>(PacketTypeID::LITERAL))
        {
            const int64_t literal = decodeLiteralPacketBody();
            return std::make_shared<LiteralPacket>(versionNumber, literal);
        }
        else
        {
            std::vector<Packet::SharedPtr> containedPacketPtrs = decodeOperatorPacketContents();
            return createOperatorPacket(static_cast<PacketTypeID>(typeID), versionNumber, std::move(containedPacketPtrs));
        }
    }

private:
    std::string m_binaryTransmissionString;
    size_t m_currentIndex = 0;

    int64_t decodeVersionNumber()
    {
        return decodeNextNBits(VERSION_NUMBER_NUM_BITS);
    }

    int64_t decodeTypeID()
    {
        return decodeNextNBits(TYPE_ID_NUM_BITS);
    }

    int64_t decodeLiteralPacketBody()
    {
        int64_t literal = 0;
        bool areThereChunksRemaining = true;
        while (areThereChunksRemaining)
        {
            areThereChunksRemaining = decodeNextNBits(1);

            const int multiplicationFactor = std::pow(2, LITERAL_CHUNK_NUM_BITS);
            literal *= multiplicationFactor;

            const int literalChunk = decodeNextNBits(LITERAL_CHUNK_NUM_BITS);
            literal += literalChunk;
        }

        return literal;
    }

    std::vector<Packet::SharedPtr> decodeOperatorPacketContents()
    {
        const int64_t lengthTypeID = decodeNextNBits(1);

        if (lengthTypeID == 0)
        {
            return decodeTotalLengthOperatorPacketContents();
        }
        else
        {
            return decodeNumSubpacketsOperatorPacketContents();
        }
    }

    Packet::SharedPtr createOperatorPacket(PacketTypeID typeID, int64_t versionNumber, std::vector<Packet::SharedPtr> containedPacketPtrs) const
    {
        switch (typeID)
        {
            case PacketTypeID::SUM:
                return std::make_shared<SumOperatorPacket>(versionNumber, std::move(containedPacketPtrs));
            case PacketTypeID::PRODUCT:
                return std::make_shared<ProductOperatorPacket>(versionNumber, std::move(containedPacketPtrs));
            case PacketTypeID::MINIMUM:
                return std::make_shared<MinimumOperatorPacket>(versionNumber, std::move(containedPacketPtrs));
            case PacketTypeID::MAXIMUM:
                return std::make_shared<MaximumOperatorPacket>(versionNumber, std::move(containedPacketPtrs));
            case PacketTypeID::GREATER_THAN:
                return std::make_shared<GreaterThanOperatorPacket>(versionNumber, std::move(containedPacketPtrs));
            case PacketTypeID::LESS_THAN:
                return std::make_shared<LessThanOperatorPacket>(versionNumber, std::move(containedPacketPtrs));
            case PacketTypeID::EQUAL_TO:
                return std::make_shared<EqualToOperatorPacket>(versionNumber, std::move(containedPacketPtrs));
            default:
                throw std::runtime_error("Unknown packet type ID: " + std::to_string(static_cast<int>(typeID)));
        }
    }


    std::vector<Packet::SharedPtr> decodeTotalLengthOperatorPacketContents()
    {
        std::vector<Packet::SharedPtr> packets;
        const int64_t totalLengthInBits = decodeNextNBits(OPERATOR_PACKET_TOTAL_LENGTH_NUM_BITS);
        const size_t startIndex = m_currentIndex;

        while (m_currentIndex - startIndex < totalLengthInBits)
        {
            Packet::SharedPtr packet = decodePacket();
            packets.push_back(std::move(packet));
        }

        return packets;
    }

    std::vector<Packet::SharedPtr> decodeNumSubpacketsOperatorPacketContents()
    {
        std::vector<Packet::SharedPtr> packets;
        const int64_t numSubpackets = decodeNextNBits(OPERATOR_PACKET_NUM_SUBPACKETS_NUM_BITS);

        for (int64_t i = 0; i < numSubpackets; ++i)
        {
            Packet::SharedPtr packet = decodePacket();
            packets.push_back(std::move(packet));
        }

        return packets;
    }

    int64_t decodeNextNBits(size_t n)
    {
        std::string nextNBitsString = m_binaryTransmissionString.substr(m_currentIndex, n);
        m_currentIndex += n;
        return std::stoi(nextNBitsString, nullptr, 2);
    }
};

std::string convertHexCharToBinaryString(char hexChar)
{
    int decimal = std::stoi(std::string{hexChar}, nullptr, 16);
    const std::bitset<4> binary(decimal);
    std::ostringstream oss;
    oss << binary;
    return oss.str();
}

std::string convertHexStringToBinaryString(const std::string& hexString)
{
    std::string binaryString;

    for (const char hexChar : hexString)
    {
        std::string binaryStringPiece = convertHexCharToBinaryString(hexChar);
        binaryString += binaryStringPiece;
    }

    return binaryString;
}

int64_t sumOfAllVersionNumbersInPackets(const std::string& transmissionString)
{
    std::string binaryTransmissionString = convertHexStringToBinaryString(transmissionString);
    BitsTransmissionDecoder decoder{std::move(binaryTransmissionString)};
    Packet::SharedPtr packet = decoder.decodePacket();

    return packet->getSumOfAllContainedVersionNumbers();
}

int64_t evaluateExpression(const std::string& transmissionString)
{
    std::string binaryTransmissionString = convertHexStringToBinaryString(transmissionString);
    BitsTransmissionDecoder decoder{std::move(binaryTransmissionString)};
    Packet::SharedPtr packet = decoder.decodePacket();

    return packet->evaluate();
}

}
}
}