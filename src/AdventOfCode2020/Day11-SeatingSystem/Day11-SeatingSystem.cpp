#include "Day11-SeatingSystem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>


#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Vector2D = Eigen::Matrix<int, 2, 1>;

using SeatState = char;
const SeatState FLOOR = '.';
const SeatState EMPTY_SEAT = 'L';
const SeatState OCCUPIED_SEAT = '#';

using SeatStateRow = std::string;
using SeatLayout = std::vector<SeatStateRow>;

class SeatingSimulator
{
public:
    SeatingSimulator(std::vector<std::string> seatLayoutLines, size_t visionRange, size_t numVisibleOccupiedSeatsThreshold)
        : m_seatLayout{std::move(seatLayoutLines)}
        , m_visionRange{visionRange}
        , m_numVisibleOccupiedSeatsThreshold{numVisibleOccupiedSeatsThreshold}
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

        for (int j = 0; j < m_height; ++j)
        {
            for (int i = 0; i < m_width; ++i)
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
    const size_t m_visionRange;
    const size_t m_numVisibleOccupiedSeatsThreshold;
    const size_t m_width;
    const size_t m_height;

    SeatLayout getNextLayout() const
    {
        auto nextLayout{m_seatLayout};

        for (int j = 0; j < m_height; ++j)
        {
            for (int i = 0; i < m_width; ++i)
            {
                nextLayout.at(j).at(i) = getNextStateAt(i, j);
            }
        }

        return nextLayout;
    }

    SeatState getNextStateAt(int x, int y) const
    {
        const SeatState currentState = m_seatLayout.at(y).at(x);

        if (currentState == FLOOR)
        {
            return FLOOR;
        }

        const int numOccupiedVisibleSeats = getNumVisibleOccupiedSeatsFrom(x, y);

        if (currentState == EMPTY_SEAT && numOccupiedVisibleSeats == 0)
        {
            return OCCUPIED_SEAT;
        }

        if (currentState == OCCUPIED_SEAT && numOccupiedVisibleSeats >= m_numVisibleOccupiedSeatsThreshold)
        {
            return EMPTY_SEAT;
        }

        return currentState;
    }

    int getNumVisibleOccupiedSeatsFrom(int x, int y) const
    {
        int numOccupiedNeighbors = 0;

        for (const auto& directionVector : getAllDirectionVectors())
        {
            if (isOccupiedSeatVisibleInGivenDirectionFrom(x, y, directionVector))
            {
                ++numOccupiedNeighbors;
            }
        }

        return numOccupiedNeighbors;
    }

    bool isOccupiedSeatVisibleInGivenDirectionFrom(int x, int y, const Vector2D& directionVector) const
    {
        Vector2D positionVector{x, y};
        for (size_t i = 0; i < m_visionRange; ++i)
        {
            positionVector += directionVector;

            if (isOutOfBounds(positionVector[0], positionVector[1]))
            {
                return false;
            }

            if (m_seatLayout.at(positionVector[1]).at(positionVector[0]) == OCCUPIED_SEAT)
            {
                return true;
            }

            if (m_seatLayout.at(positionVector[1]).at(positionVector[0]) == EMPTY_SEAT)
            {
                return false;
            }
        }

        return false;
    }

    bool isOutOfBounds(int x, int y) const
    {
        return x < 0 || x >= m_width || y < 0 || y >= m_height;
    }

    static std::vector<Vector2D> getAllDirectionVectors()
    {
        return std::vector<Vector2D>
        {
            Vector2D{1, 0},
            Vector2D{-1, 0},
            Vector2D{0, 1},
            Vector2D{0, -1},
            Vector2D{1, 1},
            Vector2D{-1, 1},
            Vector2D{-1, -1},
            Vector2D{1, -1},
        };
    }
};

int numOccupiedSeatsWithAdjacencyRules(std::vector<std::string> seatLayoutLines)
{
    SeatingSimulator seatSimulator{std::move(seatLayoutLines), 1, 4};
    seatSimulator.simulateUntilStable();
    return seatSimulator.getNumOccupiedSeats();
}

int numOccupiedSeatsWithVisibilityRules(std::vector<std::string> seatLayoutLines)
{
    SeatingSimulator seatSimulator{std::move(seatLayoutLines), std::numeric_limits<size_t>::max(), 5};
    seatSimulator.simulateUntilStable();
    return seatSimulator.getNumOccupiedSeats();
}

}
