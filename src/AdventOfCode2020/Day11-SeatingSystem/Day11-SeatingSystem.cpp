#include "Day11-SeatingSystem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using SeatState = char;
const SeatState FLOOR = '.';
const SeatState EMPTY_SEAT = 'L';
const SeatState OCCUPIED_SEAT = '#';

using SeatStateRow = std::string;
using SeatLayout = std::vector<SeatStateRow>;

class SeatingSimulator
{
public:
    SeatingSimulator(std::vector<std::string> seatLayoutLines)
        : m_seatLayout{std::move(seatLayoutLines)}
        , m_width{m_seatLayout.at(0).size()}
        , m_height{m_seatLayout.size()}
    {

    }

    void simulateUntilStable()
    {
        while (true)
        {
            SeatLayout nextLayout = getNextLayout();
            if (nextLayout == m_seatLayout)
            {
                break;
            }
            m_seatLayout = std::move(nextLayout);
        }
    }

    int getNumOccupiedSeats() const
    {
        int numOccupiedSeats = 0;

        for (int i = 0; i < m_width; ++i)
        {
            for (int j = 0; j < m_height; ++j)
            {
                if (m_seatLayout.at(j).at(i) == OCCUPIED_SEAT)
                {
                    ++numOccupiedSeats;
                }
            }
        }

        return numOccupiedSeats;
    }

private:
    SeatLayout m_seatLayout;
    const size_t m_width;
    const size_t m_height;

    SeatLayout getNextLayout() const
    {
        auto nextLayout{m_seatLayout};
        for (int i = 0; i < m_width; ++i)
        {
            for (int j = 0; j < m_height; ++j)
            {
                nextLayout.at(j).at(i) = getNextStateAt(i, j);
            }
        }
        return nextLayout;
    }

    SeatState getNextStateAt(int x, int y) const
    {
        SeatState currentState = m_seatLayout.at(y).at(x);

        if (currentState == FLOOR)
        {
            return FLOOR;
        }

        int numOccupiedNeighbors = getNumOccupiedNeighborsAt(x, y);

        if (currentState == EMPTY_SEAT && numOccupiedNeighbors == 0)
        {
            return OCCUPIED_SEAT;
        }

        if (currentState == OCCUPIED_SEAT && numOccupiedNeighbors >= 4)
        {
            return EMPTY_SEAT;
        }

        return currentState;
    }

    int getNumOccupiedNeighborsAt(int x, int y) const
    {
        const int xLowerBound = std::max(x - 1, 0);
        const int xUpperBound = std::min(x + 1, static_cast<int>(m_width) - 1);
        const int yLowerBound = std::max(y - 1, 0);
        const int yUpperBound = std::min(y + 1, static_cast<int>(m_height) - 1);

        int numOccupiedNeighbors = 0;

        for (int i = xLowerBound; i <= xUpperBound; ++i)
        {
            for (int j = yLowerBound; j <= yUpperBound; ++j)
            {
                if (x == i && y == j)
                {
                    continue;
                }
                if (m_seatLayout.at(j).at(i) == OCCUPIED_SEAT)
                {
                    ++numOccupiedNeighbors;
                }
            }
        }

        return numOccupiedNeighbors;
    }
};

int numOccupiedSeatsInStabilizedState(std::vector<std::string> seatLayoutLines)
{
    SeatingSimulator seatSimulator{std::move(seatLayoutLines)};
    seatSimulator.simulateUntilStable();
    return seatSimulator.getNumOccupiedSeats();
}

}
