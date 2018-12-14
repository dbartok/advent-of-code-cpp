#include "Day14-ChocolateCharts.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    int numRecipes;

    fileIn >> numRecipes;

    std::cout << "First part: " << AoC::scoresOfTenRecipesAfterSeveralRecipes(numRecipes) << std::endl;
}
