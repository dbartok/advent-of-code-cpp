#pragma once

#include <vector>
#include <string>

namespace AdventOfCode
{

class KnotHasher
{
public:
    static const size_t STANDARD_LIST_SIZE = 256;
    static const size_t MAX_LIST_SIZE_THAT_FITS_IN_A_BYTE = 256;
    static const unsigned STANDARD_NUM_ROUNDS = 64;
    static const size_t STANDARD_BLOCK_SIZE = 16;

    KnotHasher(std::vector<unsigned char> bytesToHash, size_t listSize = STANDARD_LIST_SIZE, size_t blockSize = STANDARD_BLOCK_SIZE);
    void executeHashRound();
    unsigned firstTwoMultiplied() const;
    std::string denseHashStringAfterMultipleRounds(unsigned numRounds = STANDARD_NUM_ROUNDS);

private:
    std::vector<unsigned char> m_knotLengths;

    std::vector<unsigned> m_hashResultNumbers;
    size_t m_blockSize;

    unsigned m_currentPos;
    unsigned m_skipSize;

    void circularReverseHashResultNumbers(size_t beginPos, size_t endPos);
    std::string elementsAsDenseHashString();
};

}
