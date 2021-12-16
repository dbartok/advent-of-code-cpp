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
size_t TYPE_NUM_BITS = 3;
size_t LITERAL_CHUNK_NUM_BITS = 4;
size_t OPERATOR_PACKET_TOTAL_LENGTH_NUM_BITS = 15;
size_t OPERATOR_PACKET_NUM_SUBPACKETS_NUM_BITS = 11;
int LITERAL_PACKET_TYPE_ID = 4;

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

    Packet(int versionNumber)
        : m_versionNumber{versionNumber}
    {

    }

    virtual int getSumOfAllContainedVersionNumbers() const = 0;

protected:
    int m_versionNumber;
};

class LiteralPacket : public Packet
{
public:
    LiteralPacket(int versionNumber, int literal)
        : Packet{versionNumber}
        , m_literal{literal}
    {

    }

    int getSumOfAllContainedVersionNumbers () const override
    {
        return m_versionNumber;
    }

private:
    int m_literal;
};

class OperatorPacket : public Packet
{
public:
    OperatorPacket(int versionNumber, std::vector<Packet::SharedPtr> containedPacketPtrs)
        : Packet{versionNumber}
        , m_containedPacketPtrs{std::move(containedPacketPtrs)}
    {

    }

    int getSumOfAllContainedVersionNumbers() const override
    {
        return std::accumulate(m_containedPacketPtrs.cbegin(), m_containedPacketPtrs.cend(), m_versionNumber, [](const auto& acc, const auto& packetPtr)
                               {
                                   return acc + packetPtr->getSumOfAllContainedVersionNumbers();
                               });
    }

private:
    std::vector<Packet::SharedPtr> m_containedPacketPtrs;
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
        const int versionNumber = decodeVersionNumber();
        const int type = decodeType();

        Packet::SharedPtr packet;
        if (type == LITERAL_PACKET_TYPE_ID)
        {
            const int literal = decodeLiteralPacketBody();
            return std::make_shared<LiteralPacket>(versionNumber, literal);
        }
        else
        {
            std::vector<Packet::SharedPtr> containedPacketPtrs = decodeOperatorPacketContents();
            return std::make_shared<OperatorPacket>(versionNumber, std::move(containedPacketPtrs));
        }
    }

private:
    std::string m_binaryTransmissionString;
    size_t m_currentIndex = 0;

    int decodeVersionNumber()
    {
        return decodeNextNBits(VERSION_NUMBER_NUM_BITS);
    }

    int decodeType()
    {
        return decodeNextNBits(TYPE_NUM_BITS);
    }

    int decodeNextNBits(int n)
    {
        std::string nextNBitsString = m_binaryTransmissionString.substr(m_currentIndex, n);
        m_currentIndex += n;
        return std::stoi(nextNBitsString, nullptr, 2);
    }

    int decodeLiteralPacketBody()
    {
        int literal = 0;
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
        const int lengthTypeID = decodeNextNBits(1);

        if (lengthTypeID == 0)
        {
            return decodeTotalLengthOperatorPacketContents();
        }
        else
        {
            return decodeNumSubpacketsOperatorPacketContents();
        }
    }

    std::vector<Packet::SharedPtr> decodeTotalLengthOperatorPacketContents()
    {
        std::vector<Packet::SharedPtr> packets;
        const int totalLengthInBits = decodeNextNBits(OPERATOR_PACKET_TOTAL_LENGTH_NUM_BITS);
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
        const int numSubpackets = decodeNextNBits(OPERATOR_PACKET_NUM_SUBPACKETS_NUM_BITS);

        for (int i = 0; i < numSubpackets; ++i)
        {
            Packet::SharedPtr packet = decodePacket();
            packets.push_back(std::move(packet));
        }

        return packets;
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

int sumOfAllVersionNumbersInPackets(const std::string& transmissionString)
{
    std::string binaryTransmissionString = convertHexStringToBinaryString(transmissionString);
    BitsTransmissionDecoder decoder{binaryTransmissionString};
    Packet::SharedPtr packet = decoder.decodePacket();

    return packet->getSumOfAllContainedVersionNumbers();
}

}
}
}