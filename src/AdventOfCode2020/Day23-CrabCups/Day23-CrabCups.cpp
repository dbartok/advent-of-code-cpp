#include "Day23-CrabCups.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>


#include <numeric>
#include <vector>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

size_t MANY_CUPS_NUMBER = 1'000'000;
size_t NUM_CUPS_TO_PICK_UP = 3;

}

namespace AdventOfCode
{

using Cup = unsigned;

struct list {};
struct hash {};

using CupLinkedHashSet = boost::multi_index_container<
    Cup,
    boost::multi_index::indexed_by<
        boost::multi_index::sequenced<
            boost::multi_index::tag<list>
        >,
        boost::multi_index::hashed_unique<
            boost::multi_index::tag<hash>,
            boost::multi_index::identity<Cup>
        >
    >
>;

class CupMixer
{
public:
    CupMixer(const std::vector<Cup>& initialCups)
        : m_cups{initialCups.cbegin(), initialCups.cend()}
        , m_currentCup{m_cups.front()}
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
        return getCupsNextToCup(1, m_cups.size() - 1);
    }

    int64_t getTwoCupLabelsAfterCupOneMultiplied() const
    {
        std::vector<Cup> twoCupsAfterCupOne = getCupsNextToCup(1, 2);
        return static_cast<int64_t>(twoCupsAfterCupOne.front()) * twoCupsAfterCupOne.back();
    }

private:
    CupLinkedHashSet m_cups;
    Cup m_currentCup;
    const Cup m_highestCup;

    void mix()
    {
        std::vector<Cup> cupsPickedUp = pickUpCups();
        const Cup destinationCup = getDestinationCup();
        putDownCups(cupsPickedUp, destinationCup);
        updateCurrentCup();
    }

    std::vector<Cup> pickUpCups()
    {
        const std::vector<Cup> cupsToBePickedUp = getCupsNextToCup(m_currentCup, NUM_CUPS_TO_PICK_UP);

        for (const auto& cup : cupsToBePickedUp)
        {
            m_cups.get<hash>().erase(cup);
        }

        return cupsToBePickedUp;
    }

    void putDownCups(const std::vector<Cup>& cupsPickedUp, Cup destinationCup)
    {
        const auto destinationCupListIter = getListIteratorForCup(destinationCup);
        m_cups.insert(std::next(destinationCupListIter), cupsPickedUp.cbegin(), cupsPickedUp.cend());
    }

    void updateCurrentCup()
    {
        auto currentCupIter = getListIteratorForCup(m_currentCup);
        advanceCupIterCircularly(currentCupIter);
        m_currentCup = *currentCupIter;
    }

    Cup getDestinationCup() const
    {
        Cup destinationCup = m_currentCup;

        while (true)
        {
            destinationCup = (destinationCup == 1) ? m_highestCup : destinationCup - 1;
            const auto destinationCupIter = m_cups.get<hash>().find(destinationCup);
            if (destinationCupIter != m_cups.get<hash>().cend())
            {
                return *destinationCupIter;
            }
        }
    }

    std::vector<Cup> getCupsNextToCup(Cup cup, size_t numCupsToSelect) const
    {
        auto cupIter = getListIteratorForCup(cup);
        std::vector<Cup> selectedCups;

        for (size_t offset = 1; offset <= numCupsToSelect; ++offset)
        {
            advanceCupIterCircularly(cupIter);
            selectedCups.push_back(*cupIter);
        }

        return selectedCups;
    }

    CupLinkedHashSet::const_iterator getListIteratorForCup(Cup cup) const
    {
        const auto cupHashIter = m_cups.get<hash>().find(cup);
        return m_cups.project<list>(cupHashIter);
    }

    void advanceCupIterCircularly(CupLinkedHashSet::const_iterator& cupIter) const
    {
        cupIter = (std::next(cupIter) == m_cups.cend()) ? m_cups.cbegin() : std::next(cupIter);
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

void addPaddingToInitialCups(std::vector<Cup>& cups)
{
    const size_t numPaddingCupsRequired = MANY_CUPS_NUMBER - cups.size();
    std::vector<Cup> paddingCups(numPaddingCupsRequired);

    const size_t firstPaddingCup = cups.size() + 1;
    std::iota(paddingCups.begin(), paddingCups.end(), firstPaddingCup);

    cups.insert(cups.end(), std::make_move_iterator(paddingCups.cbegin()), std::make_move_iterator(paddingCups.cend()));
}

std::string cupLabelsStartingFromCupOne(const std::string& initialCupLabellingString, size_t numMixes)
{
    std::vector<Cup> initialCups = convertStringToDigitwiseVector(initialCupLabellingString);
    CupMixer cupMixer{std::move(initialCups)};
    cupMixer.mixRepeatedly(numMixes);
    std::vector<Cup> labelsOnCupsAfterCupOne = cupMixer.getLabelsOnCupsAfterCupOne();
    return convertDigitwiseVectorToString(labelsOnCupsAfterCupOne);
}

int64_t twoCupLabelsAfterCupOneMultipliedManyCups(const std::string& initialCupLabellingString, size_t numMixes)
{
    std::vector<Cup> initialCups = convertStringToDigitwiseVector(initialCupLabellingString);
    addPaddingToInitialCups(initialCups);
    CupMixer cupMixer{std::move(initialCups)};
    cupMixer.mixRepeatedly(numMixes);
    return cupMixer.getTwoCupLabelsAfterCupOneMultiplied();
}

}
