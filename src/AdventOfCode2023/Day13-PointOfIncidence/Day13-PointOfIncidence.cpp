#include "Day13-PointOfIncidence.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/dense>
#include <boost/algorithm/string.hpp>

#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int HORIZONTAL_MIRROR_MULTIPLIER = 100;

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day13
{

using Matrix = Eigen::MatrixXi;

class MirrorFinder
{
public:
    MirrorFinder(Matrix matrix)
        : m_matrix{std::move(matrix)}
    {

    }

    void find()
    {
        const auto exactMatch = [](const Matrix& upperHalf, const Matrix& lowerHalf) { return upperHalf == lowerHalf; };
        m_horizontalMirrorPositionSubsequentIndex = findHorizontalMirrorPositionSubsequentIndex(exactMatch);
        m_verticalMirrorPositionSubsequentIndex = findVerticalMirrorPositionSubsequentIndex(exactMatch);
    }

    void findSmudged()
    {
        const auto smudgedMatch = [](const Matrix& upperHalf, const Matrix& lowerHalf)
        {
            return (upperHalf - lowerHalf).cwiseAbs().sum() == 1;
        };
        m_horizontalMirrorPositionSubsequentIndex = findHorizontalMirrorPositionSubsequentIndex(smudgedMatch);
        m_verticalMirrorPositionSubsequentIndex = findVerticalMirrorPositionSubsequentIndex(smudgedMatch);
    }

    int getNoteNumber() const
    {
        if (m_horizontalMirrorPositionSubsequentIndex != -1)
        {
            return m_horizontalMirrorPositionSubsequentIndex * HORIZONTAL_MIRROR_MULTIPLIER;
        }
        else if (m_verticalMirrorPositionSubsequentIndex != -1)
        {
            return m_verticalMirrorPositionSubsequentIndex;
        }
        else
        {
            throw std::runtime_error("No mirrors found");
        }
    }

private:
    using MatrixComparisonFunc = std::function<bool(const Matrix&, const Matrix&)>;

    Matrix m_matrix;

    int m_horizontalMirrorPositionSubsequentIndex = -1;
    int m_verticalMirrorPositionSubsequentIndex = -1;

    int findHorizontalMirrorPositionSubsequentIndex(const MatrixComparisonFunc& cmp) const
    {
        for (int potentialMirrorPositionSubsequentIndex = 1; potentialMirrorPositionSubsequentIndex < m_matrix.rows(); ++potentialMirrorPositionSubsequentIndex)
        {
            const size_t mirroredHeight = potentialMirrorPositionSubsequentIndex <= m_matrix.rows() / 2 ? potentialMirrorPositionSubsequentIndex : m_matrix.rows() - potentialMirrorPositionSubsequentIndex;

            Matrix upperHalf = m_matrix.block(potentialMirrorPositionSubsequentIndex - mirroredHeight, 0, mirroredHeight, m_matrix.cols());
            Matrix lowerHalf = m_matrix.block(potentialMirrorPositionSubsequentIndex, 0, mirroredHeight, m_matrix.cols());

            lowerHalf.colwise().reverseInPlace();

            if (cmp(upperHalf, lowerHalf))
            {
                return potentialMirrorPositionSubsequentIndex;
            }
        }

        return -1;
    }

    int findVerticalMirrorPositionSubsequentIndex(const MatrixComparisonFunc& cmp) const
    {
        auto* non_const_this = const_cast<MirrorFinder*>(this);

        non_const_this->m_matrix.transposeInPlace();

        const int verticalMirrorPositionSubsequentIndex = findHorizontalMirrorPositionSubsequentIndex(cmp);

        non_const_this->m_matrix.transposeInPlace();

        return verticalMirrorPositionSubsequentIndex;
    }

};

using MappingTextSection = std::vector<std::string>;

MirrorFinder parseMapTextSection(const MappingTextSection& mappingTextSection)
{
    Matrix matrix = Matrix::Zero(mappingTextSection.size(), mappingTextSection.front().size());

    for (int j = 0; j < mappingTextSection.size(); ++j)
    {
        for (int i = 0; i < mappingTextSection.front().size(); ++i)
        {
            if (mappingTextSection.at(j).at(i) == '#')
            {
                matrix(j, i) = 1;
            }
        }
    }

    return MirrorFinder{std::move(matrix)};
}

std::vector<MirrorFinder> parseMapLines(const std::vector<std::string>& mapLines)
{
    std::vector<MappingTextSection> mapTextSections;
    boost::split(mapTextSections, mapLines, [](const auto& elem) { return elem.empty(); });

    std::vector<MirrorFinder> mirrorFinders;
    for (const auto& mapTextSection : mapTextSections)
    {
        MirrorFinder mirrorFinder = parseMapTextSection(mapTextSection);
        mirrorFinders.push_back(std::move(mirrorFinder));
    }
    return mirrorFinders;
}

int numberAfterSummarizingAllNotes(const std::vector<std::string>& mapLines)
{
    std::vector<MirrorFinder> mirrorFinders = parseMapLines(mapLines);

    return std::accumulate(mirrorFinders.begin(), mirrorFinders.end(), 0, [](int acc, auto& mirrorFinder)
                           {
                               mirrorFinder.find();
                               return acc + mirrorFinder.getNoteNumber();
                           });
}

int numberAfterSummarizingAllNotesWithSmudge(const std::vector<std::string>& mapLines)
{
    std::vector<MirrorFinder> mirrorFinders = parseMapLines(mapLines);

    return std::accumulate(mirrorFinders.begin(), mirrorFinders.end(), 0, [](int acc, auto& mirrorFinder)
                           {
                               mirrorFinder.findSmudged();
                               return acc + mirrorFinder.getNoteNumber();
                           });
}


}
}
}