#include "Day23-CrabCups.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
#include <vector>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Cup = unsigned;

class CupMixer
{
public:
    CupMixer(std::vector<Cup> initialCups)
        : m_cups{std::move(initialCups)}
        , m_currentCup{m_cups.at(0)}
        , m_highestCup{m_cups.size()}
    {

    }

    void mixRepeatedly(size_t numMixes)
    {
        for (size_t i = 0; i < numMixes; ++i)
        {
            mix();
        }
    }

    std::vector<Cup> getLabelsOnCupsAfterCupOne() const
    {
        return selectCupsNextToCup(1, m_cups.size() - 1);
    }

private:
    std::vector<Cup> m_cups;
    Cup m_currentCup;
    const Cup m_highestCup;

    void mix()
    {
        std::vector<Cup> cupsPickedUp = pickUpCups();
        size_t destinationCupIndex = selectDestinationCupIndex();
        putDownCups(cupsPickedUp, destinationCupIndex);
        updateCurrentCup();
    }

    std::vector<Cup> pickUpCups()
    {
        std::vector<Cup> cupsToBePickedUp = selectCupsNextToCup(m_currentCup, 3u);

        auto eraseBeginIter = std::remove_if(m_cups.begin(), m_cups.end(), [&cupsToBePickedUp](Cup cup)
                                             {
                                                 return std::find(cupsToBePickedUp.cbegin(), cupsToBePickedUp.cend(), cup) != cupsToBePickedUp.cend();
                                             });

        m_cups.erase(eraseBeginIter, m_cups.end());

        return cupsToBePickedUp;
    }

    size_t selectDestinationCupIndex()
    {
        Cup destinationCup = m_currentCup;

        while (true)
        {
            destinationCup = destinationCup == 1 ? m_highestCup : destinationCup - 1;
            auto destinationCupIter = std::find(m_cups.cbegin(), m_cups.cend(), destinationCup);
            if (destinationCupIter != m_cups.cend())
            {
                return destinationCupIter - m_cups.cbegin();
            }
        }
    }

    void putDownCups(const std::vector<Cup>& cupsPickedUp, size_t destinationCupIndex)
    {
        m_cups.insert(m_cups.begin() + destinationCupIndex + 1, cupsPickedUp.cbegin(), cupsPickedUp.cend());
    }

    void updateCurrentCup()
    {
        size_t currentCupIndex = std::find(m_cups.cbegin(), m_cups.cend(), m_currentCup) - m_cups.cbegin();
        size_t updatedCurrentCupIndex = (currentCupIndex + 1) % m_cups.size();
        m_currentCup = m_cups.at(updatedCurrentCupIndex);
    }

    std::vector<Cup> selectCupsNextToCup(Cup cup, size_t numCupsToSelect) const
    {
        std::vector<Cup> selectedCups;
        size_t cupIndex = std::find(m_cups.cbegin(), m_cups.cend(), cup) - m_cups.cbegin();

        for (size_t offset = 1; offset <= numCupsToSelect; ++offset)
        {
            size_t pickupIndex = (cupIndex + offset) % m_cups.size();
            selectedCups.push_back(m_cups.at(pickupIndex));
        }

        return selectedCups;
    }
};

std::vector<unsigned> convertStringToDigitwiseVector(const std::string& str)
{
    std::vector<unsigned> digitwiseVector;

    std::transform(str.cbegin(), str.cend(), std::back_inserter(digitwiseVector), [](char c)
                   {
                       return c - '0';
                   });

    return digitwiseVector;
}

std::string convertDigitwiseVectorToString(const std::vector<unsigned>& digitwiseVector)
{
    std::string result;

    std::transform(digitwiseVector.cbegin(), digitwiseVector.cend(), std::back_inserter(result), [](unsigned digit)
                   {
                       return digit + '0';
                   });

    return result;
}

std::string labelsOnCupsAfterCupOne(const std::string& initialCupLabellingString, size_t numMixes)
{
    std::vector<Cup> initialCups = convertStringToDigitwiseVector(initialCupLabellingString);
    CupMixer cupMixer{std::move(initialCups)};
    cupMixer.mixRepeatedly(numMixes);
    std::vector<Cup> labelsOnCupsAfterCupOne = cupMixer.getLabelsOnCupsAfterCupOne();
    return convertDigitwiseVectorToString(labelsOnCupsAfterCupOne);
}

}
