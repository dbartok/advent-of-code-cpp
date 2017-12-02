#include "DayX-PuzzleName.h" //TODO: Change to the name of the puzzle

#include <fstream>
#include <iostream>


//TODO: Implement actual functions / classes
int solution1(int input)
{
	return input + 1;
}

int solution2(int input)
{
	return input + 2;
}


//TODO: Implement parsing and driver
int main()
{
	std::fstream fileIn("input.txt");
	int puzzleInput;

	fileIn >> puzzleInput;

	std::cout << "First part: " << solution1(puzzleInput) << std::endl;
	std::cout << "Second part: " << solution2(puzzleInput) << std::endl;
}
