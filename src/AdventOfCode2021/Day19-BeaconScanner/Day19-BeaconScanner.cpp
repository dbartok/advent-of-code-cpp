#include "Day19-BeaconScanner.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/dense>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

#include <regex>
#include <list>
#include <algorithm>
#include <set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned MIN_NUM_OVERLAPPING_BEACONS_FOR_ALIGNMENT = 12;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day19
{

using Vector3D = Eigen::RowVector3i;
using Matrix3D = Eigen::Matrix3i;

struct Vector3DCmp
{
    bool operator()(const Vector3D& lhs, const Vector3D& rhs) const
    {
        return std::lexicographical_compare(
            lhs.data(), lhs.data() + lhs.size(),
            rhs.data(), rhs.data() + rhs.size());
    }
};

using Vector3DSet = std::set<Vector3D, Vector3DCmp>;

class Scanner
{
public:
    Scanner(int id, std::vector<Vector3D> beaconPositions)
        : m_id{id}
        , m_beaconPositions{std::move(beaconPositions)}
    {

    }

    void applyBaseTransform(const Matrix3D& baseTransformVector)
    {
        for (auto& beaconPosition : m_beaconPositions)
        {
            beaconPosition = (beaconPosition * baseTransformVector).transpose();
        }
    }

    void applyOffset(const Vector3D& offset)
    {
        for (auto& beaconPosition : m_beaconPositions)
        {
            beaconPosition += offset;
        }
    }

    Vector3DSet getPossibleOffsetsFrom(const Scanner& other) const
    {
        Vector3DSet possibleOffsets;

        for (const auto& thisBeaconPosition : m_beaconPositions)
        {
            for (const auto& otherBeaconPosition : other.m_beaconPositions)
            {
                Vector3D possibleOffset = otherBeaconPosition - thisBeaconPosition;
                possibleOffsets.insert(possibleOffset);
            }
        }

        return possibleOffsets;
    }

    unsigned getNumOverlappingBeacons(const Scanner& other) const
    {
        Vector3DSet otherBeaconPositions{other.m_beaconPositions.cbegin(), other.m_beaconPositions.cend()};
        Vector3DSet beaconPositionsUnion = getBeaconPositionUnion(otherBeaconPositions);

        return m_beaconPositions.size() + other.m_beaconPositions.size() - beaconPositionsUnion.size();
    }

    Vector3DSet getBeaconPositionUnion(Vector3DSet otherBeaconPositions) const
    {
        Vector3DSet thisBeaconPositions{m_beaconPositions.cbegin(), m_beaconPositions.cend()};
        Vector3DSet beaconPositionsUnion;

        std::set_union(thisBeaconPositions.cbegin(), thisBeaconPositions.cend(),
                       otherBeaconPositions.cbegin(), otherBeaconPositions.cend(),
                       std::inserter(beaconPositionsUnion, beaconPositionsUnion.begin()),
                       Vector3DCmp{});

        return beaconPositionsUnion;
    }

    bool operator==(const Scanner& other) const
    {
        return m_id == other.m_id;
    }

    int getId()
    {
        return m_id;
    }

private:
    std::vector<Vector3D> m_beaconPositions;
    int m_id;
};

void getBaseTransformationMatricesRecursive(const Vector3DSet& baseRows, Matrix3D matrixSoFar, std::vector<Matrix3D>& baseTransformationMatrices)
{
    if (baseRows.empty())
    {
        baseTransformationMatrices.push_back(matrixSoFar);
        return;
    }

    size_t rowIndex = 3 - baseRows.size();

    for (const auto& baseRow : baseRows)
    {
        auto baseRowsCopy{baseRows};
        baseRowsCopy.erase(baseRow);

        matrixSoFar.row(rowIndex) = baseRow;
        getBaseTransformationMatricesRecursive(baseRowsCopy, matrixSoFar, baseTransformationMatrices);

        matrixSoFar.row(rowIndex) *= -1;
        getBaseTransformationMatricesRecursive(baseRowsCopy, matrixSoFar, baseTransformationMatrices);
    }
}

std::vector<Matrix3D> getBaseTransformationMatrices()
{
    const Vector3DSet baseRows = {Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1)};
    std::vector<Matrix3D> baseTransformationMatrices;

    getBaseTransformationMatricesRecursive(baseRows, {}, baseTransformationMatrices);

    return baseTransformationMatrices;
}

class ScannerAligner
{
public:
    ScannerAligner(std::vector<Scanner> scanners)
        : m_scanners{std::move(scanners)}
    {

    }

    void alignAll()
    {
        m_alignedScanners.push_back(m_scanners.front());

        for (const auto& alignedScanner : m_alignedScanners)
        {
            reconstructAllNeighbors(alignedScanner);
        }
    }

    unsigned getNumTotalBeacons() const
    {
        Vector3DSet allBeaconPositions;

        for (const auto& scanner : m_alignedScanners)
        {
            allBeaconPositions = scanner.getBeaconPositionUnion(allBeaconPositions);
        }

        return allBeaconPositions.size();
    }

private:
    std::vector<Scanner> m_scanners;
    std::list<Scanner> m_alignedScanners;

    void reconstructAllNeighbors(const Scanner& alignedScanner)
    {
        for (const auto& neighborCandidate : m_scanners)
        {
            if (isScannerAlignedAlready(neighborCandidate))
            {
                continue;
            }

            boost::optional<Scanner> alignedNeighbor = tryToAlignNeighborCandidate(alignedScanner, neighborCandidate);
            if (alignedNeighbor)
            {
                m_alignedScanners.push_back(alignedNeighbor.value());
            }
        }
    }

    static boost::optional<Scanner> tryToAlignNeighborCandidate(const Scanner& alignedScanner, const Scanner& neighborCandidate)
    {
        const std::vector<Matrix3D> baseTransformMatrices = getBaseTransformationMatrices();

        for (const auto& baseTransformMatrix : baseTransformMatrices)
        {
            auto baseTransformedNeighborCandidate{neighborCandidate};
            baseTransformedNeighborCandidate.applyBaseTransform(baseTransformMatrix);
            boost::optional<Scanner> alignedNeighbor = tryToMoveNeighborCandidateToCorrectOffset(alignedScanner, baseTransformedNeighborCandidate);
            if (alignedNeighbor)
            {
                return alignedNeighbor;
            }
        }

        return boost::none;
    }

    static boost::optional<Scanner> tryToMoveNeighborCandidateToCorrectOffset(const Scanner& alignedScanner, const Scanner& neighborCandidate)
    {
        const Vector3DSet possibleOffsets = neighborCandidate.getPossibleOffsetsFrom(alignedScanner);

        for (const auto& possibleOffset : possibleOffsets)
        {
            auto offsetNeighborCandidate{neighborCandidate};
            offsetNeighborCandidate.applyOffset(possibleOffset);
            const unsigned numOverlappingBeacons = offsetNeighborCandidate.getNumOverlappingBeacons(alignedScanner);
            if (numOverlappingBeacons >= MIN_NUM_OVERLAPPING_BEACONS_FOR_ALIGNMENT)
            {
                return offsetNeighborCandidate;
            }
        }

        return boost::none;
    }

    
    bool isScannerAlignedAlready(const Scanner& scanner) const
    {
        return std::find(m_alignedScanners.cbegin(), m_alignedScanners.cend(), scanner) != m_alignedScanners.cend();
    }
};

using TextSection = std::vector<std::string>;

Vector3D parseBeaconPositionLine(const std::string& beaconPositionLine)
{
    std::vector<int> coordinates;
    Utils::splitStringIntoTypedVector(beaconPositionLine, coordinates, ',');
    return Vector3D{coordinates.at(0), coordinates.at(1), coordinates.at(2)};
}

Scanner parseBeaconPositionsLines(const std::vector<std::string>& beaconPositionsLines)
{
    std::vector<Vector3D> beaconPositions;

    std::regex scannerIdRegex(R"(--- scanner ([0-9]+) ---)");
    std::smatch scannerIdMatch;

    std::regex_match(beaconPositionsLines.front(), scannerIdMatch, scannerIdRegex);

    const int scannerId = std::stoi(scannerIdMatch[1].str());

    for (auto lineIter = std::next(beaconPositionsLines.cbegin()); lineIter != beaconPositionsLines.cend(); ++lineIter)
    {
        Vector3D beaconPosition = parseBeaconPositionLine(*lineIter);
        beaconPositions.push_back(beaconPosition);
    }

    return Scanner{scannerId, std::move(beaconPositions)};
}

std::vector<Scanner> parseAllBeaconPositionsLines(const std::vector<std::string>& allBeaconPositionsLines)
{
    std::vector<TextSection> textSections;
    boost::split(textSections, allBeaconPositionsLines, [](const auto& elem) { return elem.empty(); });

    std::vector<Scanner> scanners;

    for (const auto& textSection : textSections)
    {
        Scanner scanner = parseBeaconPositionsLines(textSection);
        scanners.push_back(std::move(scanner));
    }

    return scanners;
}

unsigned numBeacons(const std::vector<std::string>& allBeaconPositionsLines)
{
    std::vector<Scanner> scanners = parseAllBeaconPositionsLines(allBeaconPositionsLines);
    ScannerAligner scannerAligner{std::move(scanners)};
    scannerAligner.alignAll();
    return scannerAligner.getNumTotalBeacons();
}

}
}
}