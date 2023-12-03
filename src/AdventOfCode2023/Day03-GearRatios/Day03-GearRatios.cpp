#include "Day03-GearRatios.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day03
{

using Coordinates = std::pair<int, int>;

class SchematicAnalyzer
{
public:
    SchematicAnalyzer(std::vector<std::string> schematic)
        : m_schematic{std::move(schematic)}
        , m_width{m_schematic.size()}
        , m_height{m_schematic.at(0).size()}
    {

    }

    int getSumOfAllPartNumbers() const
    {
        int sumOfAllPartNumbers = 0;

        for (int j = 0; j < m_height; ++j)
        {
            for (int i = 0; i < m_width; ++i)
            {
                if (isStartOfPartNumber({i, j}))
                {
                    const int partNumber = getPartNumber({i, j});
                    sumOfAllPartNumbers += partNumber;
                }
            }
        }

        return sumOfAllPartNumbers;
    }

private:
    const std::vector<std::string> m_schematic;
    const size_t m_width;
    const size_t m_height;

    bool isStartOfPartNumber(const Coordinates& coordinates) const
    {
        return isStartOfNumber(coordinates) && isNumberAdjacentToSymbol(coordinates);
    }

    int getPartNumber(const Coordinates& numberStartCoordinates) const
    {
        std::vector<Coordinates> allDigitCoordinates = getAllDigitCoordinatesForNumber(numberStartCoordinates);

        return std::accumulate(allDigitCoordinates.cbegin(), allDigitCoordinates.cend(), 0, [this](int acc, const auto& coords)
                               {
                                   return 10 * acc + (getValueAt(coords) - '0');
                               });
    }

    bool isStartOfNumber(const Coordinates& coordinates) const
    {
        if (!std::isdigit(getValueAt(coordinates)))
        {
            return false;
        }

        if (coordinates.first == 0)
        {
            return true;
        }

        const Coordinates leftNeighborCoordinates = {coordinates.first - 1, coordinates.second};
        return !std::isdigit(getValueAt(leftNeighborCoordinates));
    }

    bool isNumberAdjacentToSymbol(const Coordinates& numberStartCoordinates) const
    {
        std::vector<Coordinates> allDigitCoordinates = getAllDigitCoordinatesForNumber(numberStartCoordinates);

        return std::any_of(allDigitCoordinates.cbegin(), allDigitCoordinates.cend(), [this](const auto& coords)
                    {
                        return this->isPositionAdjacentToSymbol(coords);
                    });
    }

    std::vector<Coordinates> getAllDigitCoordinatesForNumber(const Coordinates& numberStartCoordinates) const
    {
        std::vector<Coordinates> allDigitCoordinates;

        for (int i = numberStartCoordinates.first; i < m_width; ++i)
        {
            Coordinates potentialDigitCoordinates{i, numberStartCoordinates.second};

            if (!std::isdigit(getValueAt(potentialDigitCoordinates)))
            {
                break;
            }

            allDigitCoordinates.push_back(std::move(potentialDigitCoordinates));
        }

        return allDigitCoordinates;
    }

    bool isPositionAdjacentToSymbol(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> allNeighborCoordinates = getAllNeighborCoordinates(coordinates);

        return std::any_of(allNeighborCoordinates.cbegin(), allNeighborCoordinates.cend(), [this](const auto& coords)
                           {
                               const char value = this->getValueAt(coords);
                               return !isalnum(value) && value != '.';
                           });
    }

    std::vector<Coordinates> getAllNeighborCoordinates(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> allNeighborCoordinates;

        for (int j = coordinates.second - 1; j <= coordinates.second + 1; ++j)
        {
            for (int i = coordinates.first - 1; i <= coordinates.first + 1; ++i)
            {
                if (i >= 0 && i < m_width && j >= 0 && j < m_height)
                {
                    allNeighborCoordinates.push_back({i, j});
                }
            }
        }

        return allNeighborCoordinates;
    }

    char getValueAt(const Coordinates& coordinates) const
    {
        return m_schematic.at(coordinates.second).at(coordinates.first);
    }
};

int sumOfAllPartNumbers(const std::vector<std::string>& schematicLines)
{
    SchematicAnalyzer schematicAnalyzer = SchematicAnalyzer{schematicLines};

    return schematicAnalyzer.getSumOfAllPartNumbers();
}

}
}
}