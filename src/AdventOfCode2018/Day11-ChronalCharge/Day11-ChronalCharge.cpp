#include "Day11-ChronalCharge.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const size_t SMALL_SQUARE_SIZE = 3;
}

namespace AdventOfCode
{

using Coordinates = std::pair<unsigned, unsigned>;

struct FuelCellSubSquare
{
    Coordinates coordinates;
    size_t size;
    int powerLevel;

    FuelCellSubSquare()
        : coordinates{}
        , size{0}
        , powerLevel{std::numeric_limits<int>::min()}
    {

    }

    FuelCellSubSquare(Coordinates coordinates, size_t size, int powerLevel)
        : coordinates{std::move(coordinates)}
        , size{size}
        , powerLevel{powerLevel}
    {

    }

    bool operator<(const FuelCellSubSquare& other) const
    {
        return powerLevel < other.powerLevel;
    }

    bool operator>(const FuelCellSubSquare& other) const
    {
        return powerLevel > other.powerLevel;
    }
};

class FuelCellGrid
{
public:
    FuelCellGrid(int serialNumber)
        : m_serialNumber{serialNumber}
    {

    }

    Coordinates highestPowerSmallSquareCoordinates() const
    {
        return highestPowerGivenSizeSubSquare(SMALL_SQUARE_SIZE).coordinates;
    }

    FuelCellSubSquare highestPowerSquare(size_t maxSize) const
    {
        FuelCellSubSquare maxSubSquare;

        for (size_t currentSize = 1; currentSize <= maxSize; ++currentSize)
        {
            auto currentSubSquare = highestPowerGivenSizeSubSquare(currentSize);
            maxSubSquare = std::max(maxSubSquare, currentSubSquare);
        }

        return maxSubSquare;
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

    int getPowerLevelAt(const Coordinates& coordinates, size_t squareSize) const
    {
        int powerLevel = 0;

        for (size_t i = coordinates.first; i < coordinates.first + squareSize; ++i)
        {
            for (size_t j = coordinates.second; j < coordinates.second + squareSize; ++j)
            {
                powerLevel += valueAt({i, j});
            }
        }

        return powerLevel;
    }

    FuelCellSubSquare highestPowerGivenSizeSubSquare(size_t squareSize) const
    {
        Coordinates maxCoordinates;
        int maxPowerLevel = std::numeric_limits<int>::min();
        const size_t topLeftCornerCandidatesSize = GRID_SIZE - squareSize;

        for (size_t i = 0; i < topLeftCornerCandidatesSize; ++i)
        {
            for (size_t j = 0; j < topLeftCornerCandidatesSize; ++j)
            {
                Coordinates currentCoordinates{i, j};
                int currentPowerLevel = getPowerLevelAt(currentCoordinates, squareSize);

                if (currentPowerLevel > maxPowerLevel)
                {
                    maxPowerLevel = currentPowerLevel;
                    maxCoordinates = currentCoordinates;
                }
            }
        }

        return FuelCellSubSquare{std::move(maxCoordinates), squareSize, maxPowerLevel};
    }
};

std::string highestPowerSmallSquareCoordinates(int gridSerialNumber)
{
    FuelCellGrid fuelCellGrid{gridSerialNumber};

    auto coordinates = fuelCellGrid.highestPowerSmallSquareCoordinates();

    return std::to_string(coordinates.first) + "," + std::to_string(coordinates.second);
}

std::string highestPowerSquareCoordinatesAndSize(int gridSerialNumber, size_t maxSize)
{
    FuelCellGrid fuelCellGrid{gridSerialNumber};

    auto square = fuelCellGrid.highestPowerSquare(maxSize);
    const auto& coordinates = square.coordinates;
    size_t size = square.size;

    return std::to_string(coordinates.first) + "," + std::to_string(coordinates.second) + "," + std::to_string(size);
}

}
