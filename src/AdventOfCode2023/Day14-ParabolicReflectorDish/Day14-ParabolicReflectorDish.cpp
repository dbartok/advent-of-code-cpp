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

const unsigned NUM_SPIN_CYCLES = 1'000'000'000;
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

    void executeSpinCycles(unsigned numSpinsToExecute)
    {
        for (unsigned numSpinsDone = 0; numSpinsDone < numSpinsToExecute; ++numSpinsDone)
        {
            const auto findResult = std::find(m_seenMatrices.cbegin(), m_seenMatrices.cend(), m_matrix);

            if (findResult != m_seenMatrices.cend())
            {
                const size_t cycleBeginIndex = findResult - m_seenMatrices.cbegin();
                const size_t cycleLength = m_seenMatrices.cend() - findResult;

                const unsigned numSpinsInsideCycle = numSpinsToExecute - cycleBeginIndex;
                const unsigned offsetInsideCycle = numSpinsInsideCycle % cycleLength;

                m_matrix = m_seenMatrices.at(cycleBeginIndex + offsetInsideCycle);
                break;
            }

            m_seenMatrices.push_back(m_matrix);
            executeSpinCycle();
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
    std::vector<Matrix> m_seenMatrices;

    void executeSpinCycle()
    {
        for (int step = 1; step <= 4; ++step)
        {
            slideNorth();
            rotateRight();
        }
    }

    void rotateRight()
    {
        m_matrix.transposeInPlace();
        m_matrix.rowwise().reverseInPlace();
    }

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

int totalLoadOnNorthSupportBeamsAfterSeveralCycles(const std::vector<std::string>& platformLines)
{
    Platform platform = parsePlatformLines(platformLines);

    platform.executeSpinCycles(NUM_SPIN_CYCLES);

    return platform.getTotalLoadOnNorthSupportBeams();
}

}
}
}