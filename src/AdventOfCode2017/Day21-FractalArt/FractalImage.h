#pragma once

#include "SmallSquare.h"
#include "ImageEnhancer.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class FractalImage
{
public:
    FractalImage(ImageEnhancer imageEnhancer);
    void enhanceRepeatedly(unsigned numIterations);
    unsigned numPixelsOn();

private:
    ImageEnhancer m_imageEnhancer;

    DynamicBoolMatrix m_image;

    void enhance();
};

}
