#include "Day08-SpaceImageFormat.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
#include <algorithm>
#include <sstream>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Line = std::vector<int>;
using Layer = std::vector<Line>;

const int TRANSPARENT = 2;

class SpaceImageFormatDecoder
{
public:
    SpaceImageFormatDecoder(const std::string& encodedImage, size_t width, size_t height)
    {
        size_t encodedImageIndex = 0;

        while (encodedImageIndex < encodedImage.size())
        {
            Layer layer;
            for (size_t j = 0; j < height; ++j)
            {
                Line line;
                for (size_t i = 0; i < width; ++i)
                {
                    line.push_back(encodedImage.at(encodedImageIndex++) - '0');
                }
                layer.push_back(line);
            }
            m_image.push_back(layer);
        }
    }

    unsigned getChecksumOfLayerWithMostZeroes() const
    {
        const auto maxLayer = *std::min_element(m_image.cbegin(), m_image.cend(), [](const auto& lhs, const auto& rhs)
                                                {
                                                    return getNumberCountInLayer(lhs, 0) < getNumberCountInLayer(rhs, 0);
                                                });

        return getNumberCountInLayer(maxLayer, 1) * getNumberCountInLayer(maxLayer, 2);
    }

    Layer getDecodedImage() const
    {
        Image layersBackToFront{m_image};
        std::reverse(layersBackToFront.begin(), layersBackToFront.end());

        Layer currentLayer = layersBackToFront.front();

        for (size_t i = 0; i < layersBackToFront.size() - 1; ++i)
        {
            const auto& nextLayer = layersBackToFront.at(i + 1);
            currentLayer = superimposeLayers(currentLayer, nextLayer);
        }

        return currentLayer;
    }

private:
    using Image = std::vector<Layer>;

    Image m_image;

    static unsigned getNumberCountInLayer(const Layer& layer, int number)
    {
        return std::accumulate(layer.cbegin(), layer.cend(), 0u, [number](unsigned totalSum, const auto& row)
                               {
                                   return totalSum + std::count(row.cbegin(), row.cend(), number);
                               });
    }

    static Layer superimposeLayers(const Layer& bottomLayer, const Layer& topLayer)
    {
        Layer result{bottomLayer};

        for (size_t j = 0; j < topLayer.size(); ++j)
        {
            for (size_t i = 0; i < topLayer.at(j).size(); ++i)
            {
                if (topLayer.at(j).at(i) != TRANSPARENT)
                {
                    result.at(j).at(i) = topLayer.at(j).at(i);
                }
            }
        }

        return result;
    }
};

unsigned checksumOfLayerWithMostZeroes(const std::string& encodedImage, size_t width, size_t height)
{
    SpaceImageFormatDecoder decoder{encodedImage, width, height};

    return decoder.getChecksumOfLayerWithMostZeroes();
}

std::string layerToString(const Layer& layer)
{
    std::ostringstream oss;

    for (size_t j = 0; j < layer.size(); ++j)
    {
        for (size_t i = 0; i < layer.at(j).size(); ++i)
        {
            oss << ((layer.at(j).at(i) == 1) ? '.' : ' ');
        }
        oss << std::endl;
    }

    return oss.str();
}

std::string decodeImage(const std::string& encodedImage, size_t width, size_t height)
{
    SpaceImageFormatDecoder decoder{encodedImage, width, height};

    return layerToString(decoder.getDecodedImage());
}

}
