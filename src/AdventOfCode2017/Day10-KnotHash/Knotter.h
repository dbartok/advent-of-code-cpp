#pragma once

#include <vector>

namespace AdventOfCode
{

class Knotter
{
public:
    Knotter(size_t listSize, std::vector<unsigned> lengths);
    void execute();
    unsigned firstTwoMultiplied() const;

private:
    std::vector<unsigned> m_lengths;
    unsigned m_skipSize;

    std::vector<unsigned> m_elements;

    void circularReverseElements(size_t beginPos, size_t endPos);
};

}
