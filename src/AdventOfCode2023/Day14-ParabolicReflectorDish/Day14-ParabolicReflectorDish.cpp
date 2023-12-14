#include "Day14-ParabolicReflectorDish.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/dense>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const char ROUND_ROCK = 'O';
const char STABLE_ROCK = '#';
const char EMPTY = '.';

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day14
{

using Matrix = Eigen::Matrix<char, Eigen::Dynamic, Eigen::Dynamic>;

class Platform
{
public:
    Platform(Matrix matrix)
        : m_matrix{std::move(matrix)}
    {

    }

    void slideNorth()
    {
        for (int j = 0; j < m_matrix.rows(); ++j)
        {
            for (int i = 0; i < m_matrix.cols(); ++i)
            {
                if (m_matrix(j, i) == ROUND_ROCK)
                {
                    const int newYCoordinate = getNewYCoordinateForRoundRockAt(i, j);

                    std::swap(m_matrix(newYCoordinate, i), m_matrix(j, i));
                }
            }
        }
    }

    int getTotalLoadOnNorthSupportBeams() const
    {
        int totalLoadOnNorthSupportBeams = 0;

        for (int j = 0; j < m_matrix.rows(); ++j)
        {
            for (int i = 0; i < m_matrix.cols(); ++i)
            {
                if (m_matrix(j, i) == ROUND_ROCK)
                {
                    totalLoadOnNorthSupportBeams += m_matrix.rows() - j;
                }
            }
        }

        return totalLoadOnNorthSupportBeams;
    }

private:
    Matrix m_matrix;

    int getNewYCoordinateForRoundRockAt(int x, int y) const
    {
        for (int newY = y; newY >= 0; --newY)
        {
            if (newY == 0 || m_matrix(newY - 1, x) != EMPTY)
            {
                return newY;
            }
        }

        assert(false); // Unreachable
    }
};

Platform parsePlatformLines(const std::vector<std::string>& platformLines)
{
    Matrix matrix = Matrix::Zero(platformLines.size(), platformLines.front().size());

    for (int j = 0; j < matrix.rows(); ++j)
    {
        for (int i = 0; i < matrix.cols(); ++i)
        {
            matrix(j, i) = platformLines.at(j).at(i);
        }
    }

    return Platform{std::move(matrix)};
}

int totalLoadOnNorthSupportBeamsAfterNorthSlide(const std::vector<std::string>& platformLines)
{
    Platform platform = parsePlatformLines(platformLines);

    platform.slideNorth();

    return platform.getTotalLoadOnNorthSupportBeams();
}

}
}
}