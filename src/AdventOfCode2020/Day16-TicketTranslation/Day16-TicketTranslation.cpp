#include "Day16-TicketTranslation.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_map>
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
    {

    }

    int getTicketScanningErrorRate() const
    {
        int errorRate = 0;

        for (const auto& ticket : m_nearbyTickets)
        {
            for (const auto& ticketValue : ticket)
            {
                if (m_ticketValueClassifier.getPossibleTicketFieldNames(ticketValue).empty())
                {
                    errorRate += ticketValue;
                }
            }
        }

        return errorRate;
    }

private:
    TicketValueClassifier m_ticketValueClassifier;
    Ticket m_ownTicket;
    std::vector<Ticket> m_nearbyTickets;
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

    for (const auto& ticketString : rulesSection)
    {
        Ticket ticket = parseTicket(ticketString);
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

}
