#include "Day2-CorruptedChecksum.h"
#include "../../Common/Utils.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

unsigned checkSumDiffSmallestLargest(const RowsOfNumbers& rows)
{
	unsigned checkSum = 0;

	for (const auto& row : rows)
	{
		if (row.empty())
		{
			continue;
		}

		unsigned maxElement = *std::max_element(row.begin(), row.end());
		unsigned minElement = *std::min_element(row.begin(), row.end());
		checkSum += (maxElement - minElement);
	}

	return checkSum;
}


int main()
{
	std::fstream fileIn("input.txt");
	RowsOfNumbers inputRows;

	std::string lineBuffer;
	while (std::getline(fileIn, lineBuffer))
	{
		RowOfNumbers row;
		parseStringIntoVector(lineBuffer, row);
		inputRows.push_back(std::move(row));
	}

	std::cout << "First part: " << checkSumDiffSmallestLargest(inputRows) << std::endl;
}
