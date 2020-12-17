#include "Day16-TicketTranslation.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_map>
#include <unordered_set>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using TextSection = std::vector<std::string>;

using TicketValue = int;
using Ticket = std::vector<TicketValue>;
using Range = std::pair<TicketValue, TicketValue>;

class TicketFieldValidator;
using TicketFieldNameToValidator = std::unordered_map<std::string, TicketFieldValidator>;

class TicketFieldValidator
{
public:
    TicketFieldValidator(std::vector<Range> ranges)
        : m_ranges{std::move(ranges)}
    {

    }

    bool isValid(TicketValue ticketValue) const
    {
        for (const auto& range : m_ranges)
        {
            if (range.first <= ticketValue && ticketValue <= range.second)
            {
                return true;
            }
        }

        return false;
    }

private:
    std::vector<Range> m_ranges;
};

class TicketValueClassifier
{
public:
    TicketValueClassifier(TicketFieldNameToValidator ticketFieldNameToValidator)
        : m_ticketFieldNameToValidator{std::move(ticketFieldNameToValidator)}
    {

    }

    std::vector<std::string> getPossibleTicketFieldNames(TicketValue ticketValue) const
    {
        std::vector<std::string> possibleTicketFieldNames;

        for (const auto& fieldNameAndValidator : m_ticketFieldNameToValidator)
        {
            if (fieldNameAndValidator.second.isValid(ticketValue))
            {
                possibleTicketFieldNames.push_back(fieldNameAndValidator.first);
            }
        }

        return possibleTicketFieldNames;
    }

private:
    TicketFieldNameToValidator m_ticketFieldNameToValidator;
};

class TicketDecoder
{
public:
    TicketDecoder(TicketValueClassifier ticketValueClassifier, Ticket ownTicket, std::vector<Ticket> nearbyTickets)
        : m_ticketValueClassifier{std::move(ticketValueClassifier)}
        , m_ownTicket{std::move(ownTicket)}
        , m_nearbyTickets{std::move(nearbyTickets)}
        , m_numTicketFields{m_ownTicket.size()}
        , m_ticketFieldIndexToPossibleNames(m_numTicketFields)
    {

    }

    void discardInvalidTickets()
    {
        std::vector<Ticket> validNearbyTickets;

        std::copy_if(m_nearbyTickets.cbegin(), m_nearbyTickets.cend(), std::back_inserter(validNearbyTickets), [this](const auto& ticket)
                     {
                         return !std::any_of(ticket.cbegin(), ticket.cend(), [this](const auto& ticketValue)
                                            {
                                                return this->m_ticketValueClassifier.getPossibleTicketFieldNames(ticketValue).empty();
                                            });
                     });

        m_nearbyTickets = validNearbyTickets;
    }

    void decode()
    {
        determineInitialPossibilities();
        derivePossibilitiesBasedOnOtherPossibilities();
    }

    int getTicketScanningErrorRate() const
    {
        return std::accumulate(m_nearbyTickets.cbegin(), m_nearbyTickets.cend(), 0, [this](int acc, const auto& ticket)
                               {
                                   return acc + std::accumulate(ticket.cbegin(), ticket.cend(), 0, [this](int ticketAcc, const auto& ticketValue)
                                                                {
                                                                    return ticketAcc + (this->m_ticketValueClassifier.getPossibleTicketFieldNames(ticketValue).empty() ? ticketValue : 0);
                                                                });
                               });
    }

    int64_t getDepartureValuesMultiplied() const
    {
        int64_t departureValuesMultiplied = 1;

        for (size_t i = 0; i < m_numTicketFields; ++i)
        {
            const auto& possibleNames = m_ticketFieldIndexToPossibleNames.at(i);
            assert(possibleNames.size() == 1);
            const auto& name = *possibleNames.cbegin();
            if (name.rfind("departure", 0) == 0)
            {
                departureValuesMultiplied *= m_ownTicket.at(i);
            }
        }

        return departureValuesMultiplied;
    }

private:
    TicketValueClassifier m_ticketValueClassifier;
    Ticket m_ownTicket;
    std::vector<Ticket> m_nearbyTickets;

    std::vector<Ticket> m_allTickets;
    size_t m_numTicketFields;

    using TicketFieldNameSet = std::unordered_set<std::string>;
    std::vector<TicketFieldNameSet> m_ticketFieldIndexToPossibleNames;

    void determineInitialPossibilities()
    {
        for (size_t fieldIndex = 0; fieldIndex < m_numTicketFields; ++fieldIndex)
        {
            std::vector<std::string> possibleNamesBasedOnOwnTicket = m_ticketValueClassifier.getPossibleTicketFieldNames(m_ownTicket.at(fieldIndex));
            m_ticketFieldIndexToPossibleNames.at(fieldIndex).insert(std::make_move_iterator(possibleNamesBasedOnOwnTicket.begin()),
                                                                    std::make_move_iterator(possibleNamesBasedOnOwnTicket.end()));

            constrainPossibilitiesForFieldBasedOnNearbyTickets(fieldIndex);
        }
    }

    void constrainPossibilitiesForFieldBasedOnNearbyTickets(size_t fieldIndex)
    {
        for (const auto& nearbyTicket : m_nearbyTickets)
        {
            TicketValue ticketValue = nearbyTicket.at(fieldIndex);
            std::vector<std::string> possibleNamesBasedOnNearbyTicket = m_ticketValueClassifier.getPossibleTicketFieldNames(ticketValue);
            std::unordered_set<std::string> possibleNamesBasedOnNearbyTicketSet{std::make_move_iterator(possibleNamesBasedOnNearbyTicket.begin()),
                                                                               std::make_move_iterator(possibleNamesBasedOnNearbyTicket.end())};

            auto& possibleNames = m_ticketFieldIndexToPossibleNames.at(fieldIndex);
            std::unordered_set<std::string> constrainedPossibleNames{possibleNames};

            for (const auto& possibleName : possibleNames)
            {
                if (possibleNamesBasedOnNearbyTicketSet.find(possibleName) == possibleNamesBasedOnNearbyTicketSet.cend())
                {
                    constrainedPossibleNames.erase(possibleName);
                }
            }

            possibleNames = constrainedPossibleNames;
        }
    }

    void derivePossibilitiesBasedOnOtherPossibilities()
    {
        std::unordered_set<size_t> finalizedIndices;

        while (finalizedIndices.size() != m_numTicketFields)
        {
            for (size_t i = 0; i < m_numTicketFields; ++i)
            {
                if (finalizedIndices.find(i) == finalizedIndices.cend() && m_ticketFieldIndexToPossibleNames.at(i).size() == 1)
                {
                    finalizedIndices.insert(i);
                    eliminatePossibilitiesAtOtherIndicesBasedOnFinalizedIndex(i);
                    break;
                }
            }
        }
    }

    void eliminatePossibilitiesAtOtherIndicesBasedOnFinalizedIndex(size_t finalizedIndex)
    {
        const std::string& onlyPossibleName = *m_ticketFieldIndexToPossibleNames.at(finalizedIndex).cbegin();
        for (size_t i = 0; i < m_numTicketFields; ++i)
        {
            if (i != finalizedIndex)
            {
                m_ticketFieldIndexToPossibleNames.at(i).erase(onlyPossibleName);
            }
        }
    }
};

Range parseRange(const std::string& rangeString)
{
    std::vector<std::string> tokens;
    boost::split(tokens, rangeString, boost::is_any_of("-"));

    return Range{std::stoi(tokens.at(0)), std::stoi(tokens.at(1))};
}

std::vector<Range> parseRanges(const std::string& rangesString)
{
    std::vector<std::string> tokens;
    boost::split(tokens, rangesString, boost::is_any_of("or "), boost::token_compress_on);

    std::vector<Range> ranges;

    for (const auto& token : tokens)
    {
        Range range = parseRange(token);
        ranges.push_back(std::move(range));
    }

    return ranges;
}

TicketFieldNameToValidator parseTicketFieldNameToValidator(const TextSection& rulesSection)
{
    TicketFieldNameToValidator ticketFieldNameToValidator;

    for (const auto& line : rulesSection)
    {
        const size_t colonPosition = line.find(':');
        std::string name = line.substr(0, colonPosition);
        std::vector<Range> ranges = parseRanges(line.substr(colonPosition + 2));

        TicketFieldValidator ticketFieldValidator{ranges};

        ticketFieldNameToValidator.emplace(name, TicketFieldValidator{std::move(ranges)});
    }

    return ticketFieldNameToValidator;
}

Ticket parseTicket(const std::string& ticketString)
{
    Ticket outputTicket;
    Utils::splitStringIntoTypedVector(ticketString, outputTicket, ',');
    return outputTicket;
}

Ticket parseOwnTicket(const TextSection& rulesSection)
{
    return parseTicket(rulesSection.at(1));
}

std::vector<Ticket> parseNearbyTickets(const TextSection& rulesSection)
{
    std::vector<Ticket> tickets;

    for (auto ticketStringIter = std::next(rulesSection.cbegin()); ticketStringIter != rulesSection.cend(); ++ticketStringIter)
    {
        Ticket ticket = parseTicket(*ticketStringIter);
        tickets.push_back(std::move(ticket));
    }

    return tickets;
}

TicketDecoder createTicketDecoder(const std::vector<std::string>& noteLines)
{
    std::vector<TextSection> textSections;
    boost::split(textSections, noteLines, [](const auto& elem) {return elem.empty(); });

    TicketFieldNameToValidator ticketFieldNameToValidator = parseTicketFieldNameToValidator(textSections.at(0));
    TicketValueClassifier ticketValueClassifier{std::move(ticketFieldNameToValidator)};

    Ticket ownTicket = parseOwnTicket(textSections.at(1));

    std::vector<Ticket> nearbyTickets = parseNearbyTickets(textSections.at(2));

    return TicketDecoder{std::move(ticketValueClassifier), std::move(ownTicket), std::move(nearbyTickets)};
}

int ticketScanningErrorRate(const std::vector<std::string>& noteLines)
{
    TicketDecoder ticketDecoder = createTicketDecoder(noteLines);
    return ticketDecoder.getTicketScanningErrorRate();
}

int64_t departureValuesMultiplied(const std::vector<std::string>& noteLines)
{
    TicketDecoder ticketDecoder = createTicketDecoder(noteLines);
    ticketDecoder.discardInvalidTickets();
    ticketDecoder.decode();
    return ticketDecoder.getDepartureValuesMultiplied();
}

}
