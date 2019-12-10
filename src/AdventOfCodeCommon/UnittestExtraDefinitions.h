#pragma once

#include "DisableLibraryWarningsMacros.h"

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"

#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace Microsoft
{
namespace VisualStudio
{
namespace CppUnitTestFramework
{

template <typename T>
std::wstring ToString(const std::vector<T> v)
{
    std::wstringstream s;

    for (size_t i = 0; i < v.size(); i++)
    {
        s << ToString(v[i]);
        if (i != v.size() - 1)
        {
            s << ", ";
        }
    }

    return s.str();
}

}
}
}
