#include "Day10-KnotHash.h"

#include "KnotHasher.h"
#include "../../Common/Utils.h"

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

namespace
{
    std::vector<unsigned char> EXTRA_SALT_BYTES = {17, 31, 73, 47, 23};
}

namespace AdventOfCode
{

unsigned firstTwoMultipliedAfterKnotting(size_t listSize, const std::vector<unsigned>& lengths)
{
    std::vector<unsigned char> bytesToHash;
    bytesToHash.reserve(lengths.size());
    std::transform(lengths.cbegin(), lengths.cend(), std::back_inserter(bytesToHash), [](unsigned lengthElem) 
    {
        if (lengthElem > 255)
        {
            throw std::runtime_error("An input is larger than 255.");
        }

        return static_cast<unsigned char>(lengthElem);
    });

    KnotHasher knotHasher{bytesToHash, listSize, 1};
    knotHasher.executeHashRound();
    return knotHasher.firstTwoMultiplied();
}

std::string knotHashDenseString(const std::string& stringToHash)
{
    auto stringToHashTrimmed(stringToHash);
    boost::trim(stringToHashTrimmed);

    std::vector<unsigned char> bytesToHash;
    bytesToHash.reserve(stringToHashTrimmed.size());
    std::transform(stringToHashTrimmed.cbegin(), stringToHashTrimmed.cend(), std::back_inserter(bytesToHash), [](char c)
    {
        return static_cast<unsigned char>(c);
    });

    bytesToHash.insert(bytesToHash.end(), EXTRA_SALT_BYTES.cbegin(), EXTRA_SALT_BYTES.cend());

    KnotHasher knotHasher{bytesToHash};
    return knotHasher.denseHashStringAfterMultipleRounds();
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string inputLengthsString;
    std::getline(fileIn, inputLengthsString);

    std::vector<unsigned> inputLengths;
    AoC::Utils::splitStringIntoTypedVector(inputLengthsString, inputLengths, ',');

    std::cout << "First part: " << AoC::firstTwoMultipliedAfterKnotting(AoC::KnotHasher::STANDARD_LIST_SIZE, inputLengths) << std::endl;
    std::cout << "Second part: " << AoC::knotHashDenseString(inputLengthsString) << std::endl;
}
