#include "Day11-ChronalCharge.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const size_t GRID_SIZE = 300;
const size_t SMALL_SQUARE_SIZE = 3;
}

namespace AdventOfCode
{

using Coordinates = std::pair<unsigned, unsigned>;

class FuelCellGrid
{
public:
    FuelCellGrid(int serialNumber)
        : m_serialNumber{serialNumber}
    {

    }

    Coordinates highestPowerSmallSquareCoordinates() const
    {
        Coordinates maxCoordinates;
        int maxPowerLevel = std::numeric_limits<int>::min();
        const size_t topLeftCornerCandidatesSize = GRID_SIZE - SMALL_SQUARE_SIZE;

        for (size_t i = 0; i < topLeftCornerCandidatesSize; ++i)
        {
            for (size_t j = 0; j < topLeftCornerCandidatesSize; ++j)
            {
                Coordinates currentCoordinates{i, j};
                int currentPowerLevel = getPowerLevelAt(currentCoordinates);

                if (currentPowerLevel > maxPowerLevel)
                {
                    maxPowerLevel = currentPowerLevel;
                    maxCoordinates = currentCoordinates;
                }
            }
        }

        return maxCoordinates;
    }

private:
    int m_serialNumber;

    int valueAt(const Coordinates& coordinates) const
    {
        int rackID = coordinates.first + 10;

        int powerLevel = rackID * coordinates.second;
        powerLevel += m_serialNumber;
        powerLevel *= rackID;

        int hundredsDigit = (powerLevel / 100) % 10;
        return hundredsDigit - 5;
    }

    int getPowerLevelAt(const Coordinates& coordinates) const
    {
        int powerLevel = 0;

        for (size_t i = coordinates.first; i < coordinates.first + SMALL_SQUARE_SIZE; ++i)
        {
            for (size_t j = coordinates.second; j < coordinates.second + SMALL_SQUARE_SIZE; ++j)
            {
                powerLevel += valueAt({i, j});
            }
        }

        return powerLevel;
    }
};

std::string highestPowerSmallSquareCoordinates(int gridSerialNumber)
{
    FuelCellGrid fuelCellGrid{gridSerialNumber};

    auto coordinates = fuelCellGrid.highestPowerSmallSquareCoordinates();

    return std::to_string(coordinates.first) + "," + std::to_string(coordinates.second);
}

}
