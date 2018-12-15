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
    int recipeNumber;

    fileIn >> recipeNumber;

    std::string recipeSequence = std::to_string(recipeNumber);

    std::cout << "First part: " << AoC::scoresOfTenRecipesAfterSeveralRecipes(recipeNumber) << std::endl;
    std::cout << "Second part: " << AoC::numRecipesBeforeSequenceAppears(recipeSequence) << std::endl;
}
