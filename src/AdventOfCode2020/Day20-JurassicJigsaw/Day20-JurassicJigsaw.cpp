#include "Day20-JurassicJigsaw.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <Eigen/Dense>

#include <regex>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Image = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;

class Tile
{
public:
    Tile(int id, Image image)
        : m_id{id}
        , m_image{std::move(image)}
    {

    }

    Tile()
        : m_id{-1}
    {

    }

    void rotateRight()
    {
        m_image.transposeInPlace();
        m_image.rowwise().reverseInPlace();
    }

    void flipHorizontally()
    {
        m_image.rowwise().reverseInPlace();
    }

    void removeBorders()
    {
        for (int i = 0; i < 2; ++i)
        {
            m_image.conservativeResize(m_image.cols() - 1, m_image.rows() - 1);
            rotateRight();
            rotateRight();
        }
    }

    bool isPopulated() const
    {
        return m_id != -1;
    }

    int getID() const
    {
        return m_id;
    }

    const Image& getImage() const
    {
        return m_image;
    }

    std::vector<Tile> getAllVariants() const
    {
        std::vector<Tile> variants;

        auto copy{*this};
        for (size_t i = 0; i < 4; ++i)
        {
            variants.push_back(copy);
            copy.flipHorizontally();
            variants.push_back(copy);
            copy.flipHorizontally();
            copy.rotateRight();
        }

        return variants;
    }

    bool operator==(const Tile& other) const
    {
        return this->m_id == other.m_id;
    }

private:
    int m_id = -1;
    Image m_image;
};

enum class Direction
{
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3,
};

class ImageReassembler
{
public:
    ImageReassembler(std::vector<Tile> scrambledTiles)
        : m_scrambledTiles{std::move(scrambledTiles)}
        , m_reassembledImageLengthInTiles{static_cast<size_t>(std::sqrt(m_scrambledTiles.size()))}
        , m_reassembledTileGrid(m_reassembledImageLengthInTiles, std::vector<Tile>(m_reassembledImageLengthInTiles))
    {

    }

    void reassemble()
    {
        populateTopLeftCorner();
        populateSubsequentTiles();
        removeBorders();
    }

    int64_t getCornerTileIDsMultiplied() const
    {
        const auto& firstRow = m_reassembledTileGrid.front();
        const auto& lastRow = m_reassembledTileGrid.back();

        return static_cast<int64_t>(firstRow.front().getID()) * firstRow.back().getID() * lastRow.front().getID() * lastRow.back().getID();
    }

    std::vector<Image> getAllPossibleImages() const
    {
        Tile combinedTile = getAllTilesCombined();
        std::vector<Tile> allPossibleTiles = combinedTile.getAllVariants();

        std::vector<Image> allPossibleImages;
        std::transform(allPossibleTiles.cbegin(), allPossibleTiles.cend(), std::back_inserter(allPossibleImages), [](const auto& tile) {return tile.getImage(); });
        return allPossibleImages;
    }

private:
    std::vector<Tile> m_scrambledTiles;
    size_t m_reassembledImageLengthInTiles;
    std::vector<std::vector<Tile>> m_reassembledTileGrid;

    void populateTopLeftCorner()
    {
        Tile topLeftCorner;

        for (const auto& tile : m_scrambledTiles)
        {
            for (const auto& tileVariant : tile.getAllVariants())
            {
                if (isTileVariantTopLeftCorner(tileVariant))
                {
                    topLeftCorner = tileVariant;
                    break;
                }
            }
            if (topLeftCorner.isPopulated())
            {
                break;
            }
        }

        m_reassembledTileGrid.at(0).at(0) = topLeftCorner;
    }

    void populateSubsequentTiles()
    {
        for (size_t j = 0; j < m_reassembledImageLengthInTiles; ++j)
        {
            for (size_t i = 0; i < m_reassembledImageLengthInTiles; ++i)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }
                populateTileAt(i, j);
            }
        }
    }

    void populateTileAt(size_t i, size_t j)
    {
        boost::optional<std::vector<Tile>> possibleTilesFromUpwardsConstraint = getPossibleTilesFromUpwardsConstraint(i, j);
        boost::optional<std::vector<Tile>> possibleTilesFromLeftwardsConstraint = getPossibleTilesFromLeftwardsConstraint(i, j);

        Tile resolvedTile = resolveTileFromMaybeConstraints(possibleTilesFromUpwardsConstraint, possibleTilesFromLeftwardsConstraint);
        m_reassembledTileGrid.at(j).at(i) = std::move(resolvedTile);
    }

    void removeBorders()
    {
        for (auto& row : m_reassembledTileGrid)
        {
            for (auto& tile : row)
            {
                tile.removeBorders();
            }
        }
    }

    bool isTileVariantTopLeftCorner(const Tile& tileVariant) const
    {
        std::vector<Tile> allPossibleNeighborsToLeftwards = findAllPossibleNeighborsInDirection(tileVariant, Direction::LEFT);
        std::vector<Tile> allPossibleNeighborsToUpwards = findAllPossibleNeighborsInDirection(tileVariant, Direction::UP);

        return allPossibleNeighborsToLeftwards.empty() && allPossibleNeighborsToUpwards.empty();
    }

    std::vector<Tile> findAllPossibleNeighborsInDirection(const Tile& anchorTileVariant, Direction direction) const
    {
        std::vector<Tile> allPossibleNeighborsInDirection;
        for (const auto& tile : m_scrambledTiles)
        {
            if (tile == anchorTileVariant)
            {
                continue;
            }

            for (const auto& tileVariant : tile.getAllVariants())
            {
                if (doesTileVariantFitAnchorInDirection(anchorTileVariant, tileVariant, direction))
                {
                    allPossibleNeighborsInDirection.push_back(tileVariant);
                }
            }
        }

        return allPossibleNeighborsInDirection;
    }

    bool doesTileVariantFitAnchorInDirection(Tile anchorTileVariant, Tile tileVariant, Direction direction) const
    {
        int numRightRotationsRequired = static_cast<int>(direction);

        while ((numRightRotationsRequired--) > 0)
        {
            anchorTileVariant.rotateRight();
            tileVariant.rotateRight();
        }

        return doesTileVariantFitAnchorToUpwards(anchorTileVariant, tileVariant);
    }

    bool doesTileVariantFitAnchorToUpwards(const Tile& anchorTileVariant, const Tile& tileVariant) const
    {
        const auto& anchorTileVariantImage = anchorTileVariant.getImage();
        const auto& tileVariantImage = tileVariant.getImage();

        return tileVariantImage.row(tileVariantImage.rows() - 1) == anchorTileVariantImage.row(0);
    }

    boost::optional<std::vector<Tile>> getPossibleTilesFromUpwardsConstraint(size_t x, size_t y)
    {
        if (y > 0)
        {
            const Tile& upwardsNeighbor = m_reassembledTileGrid.at(y - 1).at(x);
            if (upwardsNeighbor.isPopulated())
            {
                return findAllPossibleNeighborsInDirection(upwardsNeighbor, Direction::DOWN);
            }
        }

        return boost::none;
    }

    boost::optional<std::vector<Tile>> getPossibleTilesFromLeftwardsConstraint(size_t x, size_t y)
    {
        if (x > 0)
        {
            const Tile& leftwardsNeighbor = m_reassembledTileGrid.at(y).at(x-1);
            if (leftwardsNeighbor.isPopulated())
            {
                return findAllPossibleNeighborsInDirection(leftwardsNeighbor, Direction::RIGHT);
            }
        }

        return boost::none;
    }

    Tile resolveTileFromMaybeConstraints(const boost::optional<std::vector<Tile>>& possibleTilesFromUpwardsConstraint,
                                         const boost::optional<std::vector<Tile>>& possibleTilesFromLeftwardsConstraint) const
    {
        if (!(possibleTilesFromUpwardsConstraint.has_value() || possibleTilesFromLeftwardsConstraint.has_value()))
        {
            throw std::runtime_error("Tile has no constraints in either direction");
        }

        if (possibleTilesFromUpwardsConstraint.has_value() && !possibleTilesFromLeftwardsConstraint.has_value())
        {
            if (possibleTilesFromUpwardsConstraint.get().size() != 1)
            {
                throw std::runtime_error("Unable to uniquely resolve tile from upwards constraint");
            }
            return possibleTilesFromUpwardsConstraint.get().front();
        }

        if (possibleTilesFromLeftwardsConstraint.has_value() && !possibleTilesFromUpwardsConstraint.has_value())
        {
            if (possibleTilesFromLeftwardsConstraint.get().size() != 1)
            {
                throw std::runtime_error("Unable to uniquely resolve tile from leftwards constraint");
            }
            return possibleTilesFromLeftwardsConstraint.get().front();
        }

        return resolveTileFromBothConstraints(possibleTilesFromUpwardsConstraint.get(), possibleTilesFromLeftwardsConstraint.get());
    }

    Tile resolveTileFromBothConstraints(std::vector<Tile> possibleTilesFromUpwardsConstraint,
                                        std::vector<Tile> possibleTilesFromLeftwardsConstraint) const
    {
        auto sortingFunc = [](const Tile& lhs, const Tile& rhs) {return lhs.getID() < rhs.getID(); };
        std::sort(possibleTilesFromUpwardsConstraint.begin(), possibleTilesFromUpwardsConstraint.end(), sortingFunc);
        std::sort(possibleTilesFromLeftwardsConstraint.begin(), possibleTilesFromLeftwardsConstraint.end(), sortingFunc);

        std::vector<Tile> interectionTiles;
        std::set_intersection(possibleTilesFromUpwardsConstraint.cbegin(), possibleTilesFromUpwardsConstraint.cend(),
                              possibleTilesFromLeftwardsConstraint.cbegin(), possibleTilesFromLeftwardsConstraint.cend(),
                              std::back_inserter(interectionTiles), sortingFunc);

        if (interectionTiles.size() != 1)
        {
            throw std::runtime_error("Unable to uniquely resolve tile from both constraints");
        }

        return interectionTiles.front();
    }

    Tile getAllTilesCombined() const
    {
        const Tile& someTile = m_reassembledTileGrid.front().front();
        size_t tileLength = someTile.getImage().rows();
        size_t combinedImageLength = m_reassembledImageLengthInTiles * tileLength;
        Image combinedImage{combinedImageLength, combinedImageLength};

        for (size_t j = 0; j < combinedImageLength; ++j)
        {
            for (size_t i = 0; i < combinedImageLength; ++i)
            {
                size_t tileX = i / tileLength;
                size_t tileY = j / tileLength;
                const Image& tileImage = m_reassembledTileGrid.at(tileY).at(tileX).getImage();

                size_t offsetInTileImageX = i % tileLength;
                size_t offsetInTileImageY = j % tileLength;

                combinedImage(j, i) = tileImage(offsetInTileImageY, offsetInTileImageX);
            }
        }
        return Tile{0, std::move(combinedImage)};
    }
};

class SeaMonsterFinder
{
public:
    SeaMonsterFinder(std::vector<std::string> image)
        : m_originalImage{std::move(image)}
        , m_image{m_originalImage}
        , m_imageHeight{m_image.size()}
        , m_imageWidth{m_image.front().size()}
    {

    }

    void markSeaMonsterFields()
    {
        for (size_t j = 0; j < m_imageHeight - m_seaMonsterPatternHeight + 1; ++j)
        {
            for (size_t i = 0; i < m_imageWidth - m_seaMonsterPatternWidth + 1; ++i)
            {
                if (isSeaMonsterStartingAt(i, j))
                {
                    markSeaMonsterPatternStartingAt(i, j);
                }
            }
        }
    }

    int getNumWaterFields() const
    {
        return std::accumulate(m_image.cbegin(), m_image.cend(), 0, [](int outerAcc, const auto& row)
                               {
                                   return outerAcc + std::accumulate(row.cbegin(), row.cend(), 0, [](int innerAcc, const auto& field)
                                                                     {
                                                                         return innerAcc + (field == '#');
                                                                     });
                               });
    }

private:
    const std::vector<std::string> m_originalImage;
    std::vector<std::string> m_image;
    size_t m_imageHeight;
    size_t m_imageWidth;

    const std::vector<std::string> m_seaMonsterPattern =
    {
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    };
    const size_t m_seaMonsterPatternHeight = m_seaMonsterPattern.size();
    const size_t m_seaMonsterPatternWidth = m_seaMonsterPattern.front().size();

    void markSeaMonsterPatternStartingAt(size_t x, size_t y)
    {
        for (size_t j = 0; j < m_seaMonsterPatternHeight; ++j)
        {
            for (size_t i = 0; i < m_seaMonsterPatternWidth; ++i)
            {
                const auto& seaMonsterPatternField = m_seaMonsterPattern.at(j).at(i);
                if (seaMonsterPatternField == '#')
                {
                    m_image.at(y + j).at(x + i) = 'O';
                }
            }
        }
    }

    bool isSeaMonsterStartingAt(size_t x, size_t y) const
    {
        for (size_t j = 0; j < m_seaMonsterPatternHeight; ++j)
        {
            for (size_t i = 0; i < m_seaMonsterPatternWidth; ++i)
            {
                const auto& seaMonsterPatternField = m_seaMonsterPattern.at(j).at(i);
                if (seaMonsterPatternField == '#' && m_image.at(y + j).at(x + i) != '#')
                {
                    return false;
                }
            }
        }

        return true;
    }
};

std::vector<std::string> convertImageToStringImage(const Image& image)
{
    std::vector<std::string> result;

    for (size_t j = 0; j < image.rows(); ++j)
    {
        std::string row;
        for (size_t i = 0; i < image.cols(); ++i)
        {
            row.push_back(image(j, i) ? '#' : '.');
        }
        result.push_back(std::move(row));
    }

    return result;
}

int createTileID(const std::string& tileIDLine)
{
    const std::regex idLineRegex{R"(Tile (\d+):)"};
    std::smatch matches;
    std::regex_search(tileIDLine, matches, idLineRegex);
    if (matches.size() != 2)
    {
        throw std::runtime_error("Invalid tile ID line: " + tileIDLine);
    }
    return std::stoi(matches[1]);
}

Image createImage(const std::vector<std::string>& imageLines)
{
    Image image{imageLines.size(), imageLines.at(0).size()};

    for (size_t j = 0; j < imageLines.size(); ++j)
    {
        for (size_t i = 0; i < imageLines.size(); ++i)
        {
            image(j, i) = (imageLines[j][i] == '#');
        }
    }

    return image;
}

Tile createTile(const std::vector<std::string>& tileLines)
{
    int tileID = createTileID(tileLines.front());

    std::vector<std::string> imageLines{std::next(tileLines.cbegin()), tileLines.cend()};
    Image image = createImage(imageLines);

    return Tile{std::move(tileID), std::move(image)};
}

std::vector<Tile> createTiles(const std::vector<std::string>& cameraArrayLines)
{
    std::vector<std::vector<std::string>> tokens;
    boost::split(tokens, cameraArrayLines, [](const auto& line) {return line.empty(); });

    std::vector<Tile> tiles;

    for (const auto& tileLines : tokens)
    {
        if (!tileLines.empty())
        {
            Tile tile = createTile(tileLines);
            tiles.push_back(std::move(tile));
        }
    }

    return tiles;
}

int64_t cornerTileIDsMultiplied(const std::vector<std::string>& cameraArrayLines)
{
    std::vector<Tile> tiles = createTiles(cameraArrayLines);
    ImageReassembler imageReassembler{std::move(tiles)};
    imageReassembler.reassemble();
    return imageReassembler.getCornerTileIDsMultiplied();
}

int numWaterNotPartOfASeaMonster(const std::vector<std::string>& cameraArrayLines)
{
    std::vector<Tile> tiles = createTiles(cameraArrayLines);
    ImageReassembler imageReassembler{std::move(tiles)};
    imageReassembler.reassemble();
    std::vector<Image> allPossibleReassembledImages = imageReassembler.getAllPossibleImages();

    for (const auto& image : allPossibleReassembledImages)
    {
        std::vector<std::string> stringImage = convertImageToStringImage(image);
        SeaMonsterFinder seaMonsterFinder{std::move(stringImage)};

        int originalNumWater = seaMonsterFinder.getNumWaterFields();
        seaMonsterFinder.markSeaMonsterFields();
        int numWaterNotPartOfASeaMonster = seaMonsterFinder.getNumWaterFields();

        if (originalNumWater != numWaterNotPartOfASeaMonster)
        {
            return numWaterNotPartOfASeaMonster;
        }
    }

    throw std::runtime_error("Unable to find any sea monsters in any of the possible images");
}

}
