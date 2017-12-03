#include "Day3-SpiralMemory.h"

#include <fstream>
#include <iostream>
#include <algorithm>


unsigned stepsToCarryBack(unsigned location)
{
	if (location == 1)
	{
		return 0;
	}

	unsigned squareSideLength = 3;
	unsigned totalElements = 1;

	while (true)
	{
		unsigned numElementsOnPerimeter = (squareSideLength - 1) * 4;

		if (totalElements + numElementsOnPerimeter >= location)
		{
			break;
		}

		totalElements += numElementsOnPerimeter;
		squareSideLength += 2;
	}

	unsigned locationInRing = location - totalElements;
	unsigned locationInSide = locationInRing % (squareSideLength - 1);
	unsigned radiusOfSquare = squareSideLength / 2;

	unsigned distanceFromMiddleOfSide = std::max(radiusOfSquare, locationInSide) - std::min(radiusOfSquare, locationInSide);

	unsigned steps = radiusOfSquare + distanceFromMiddleOfSide;
}


int main()
{
	std::fstream fileIn("input.txt");
	unsigned inputLocation;

	fileIn >> inputLocation;

	std::cout << "First part: " << stepsToCarryBack(inputLocation) << std::endl;
}
