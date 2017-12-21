#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Core>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using DynamicBoolMatrix = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;

}
