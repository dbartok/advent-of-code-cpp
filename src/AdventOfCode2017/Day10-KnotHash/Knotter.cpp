#include "Knotter.h"

#include <numeric>

namespace AdventOfCode
{

Knotter::Knotter(size_t listSize, std::vector<unsigned> lengths)
    : m_lengths{std::move(lengths)}
    , m_skipSize{0}
    , m_elements(listSize) // We'd like to avoid resolving to the initializer list here
{
    std::iota(m_elements.begin(), m_elements.end(), 0);
}

void Knotter::execute()
{
    size_t endPos = 0;
    for (auto length : m_lengths)
    {
        if (length > m_elements.size())
        {
            throw std::runtime_error("Invalid length.");
        }

        size_t currentPos = (endPos + m_skipSize) % m_elements.size();

        if (length == 0)
        {
            endPos = currentPos;
        }
        else
        {
            endPos = (currentPos + length - 1) % m_elements.size();
        }

        circularReverseElements(currentPos, endPos);

        ++m_skipSize;
    }
}

unsigned Knotter::firstTwoMultiplied() const
{
    if (m_elements.size() < 2)
    {
        throw std::runtime_error("Not enough elements.");
    }

    return m_elements[0] * m_elements[1];
}

void Knotter::circularReverseElements(size_t beginPos, size_t endPos)
{
    unsigned numSwaps;
    if (endPos >= beginPos)
    {
        numSwaps = (endPos - beginPos + 1) / 2;
    }
    else
    {
        numSwaps = (m_elements.size() - (beginPos - endPos) + 1) / 2;
    }

    for (unsigned i = 0; i < numSwaps; ++i)
    {
        std::swap(m_elements[beginPos], m_elements[endPos]);
        
        beginPos = (beginPos == m_elements.size() - 1) ? 0 : beginPos + 1;
        endPos = (endPos == 0) ? m_elements.size() - 1 : endPos - 1;
    }
}

}
