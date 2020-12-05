#include "Day05-BinaryBoarding.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct Seat
{
    int row;
    int column;

    int getID() const
    {
        return 8 * row + column;
    }
};

Seat parseSeat(const std::string& boardingPassLine)
{
    std::string rowString = boardingPassLine.substr(0, 7);
    std::string columnString = boardingPassLine.substr(7);

    std::replace(rowString.begin(), rowString.end(), 'F', '0');
    std::replace(rowString.begin(), rowString.end(), 'B', '1');
    int row = std::stoi(rowString, nullptr, 2);

    std::replace(columnString.begin(), columnString.end(), 'L', '0');
    std::replace(columnString.begin(), columnString.end(), 'R', '1');
    int column = std::stoi(columnString, nullptr, 2);

    return Seat{row, column};
}

std::vector<Seat> parseSeats(const std::vector<std::string>& boardingPassLines)
{
    std::vector<Seat> seats;

    for (const auto& line : boardingPassLines)
    {
        Seat seat = parseSeat(line);
        seats.push_back(std::move(seat));
    }

    return seats;
}

int highestSeatID(const std::vector<std::string>& boardingPassLines)
{
    std::vector<Seat> seats = parseSeats(boardingPassLines);

    auto highestSeatIDIter = std::max_element(seats.cbegin(), seats.cend(), [](const auto& lhs, const auto& rhs)
                                              {
                                                  return lhs.getID() < rhs.getID();
                                              });

    return highestSeatIDIter->getID();
}

}
