#include "Day1-InverseCaptcha.h"

#include <fstream>
#include <iostream>

unsigned sumOfDigitsMatchNextDigit(const std::string& digitsString)
{
    unsigned sum = 0;
    for (size_t i = 0; i < digitsString.size() - 1; i++)
    {
        if (digitsString[i] == digitsString[i + 1])
        {
            sum += digitsString[i] - '0';
        }
    }

    // Sequence of digits is treated as circular
    if (!digitsString.empty())
    {
        if (digitsString.back() == digitsString.front())
        {
            sum += digitsString.back() - '0';
        }
    }

    return sum;
}

unsigned sumOfDigitsMatchHalfwayRoundDigit(const std::string& digitsString)
{
    unsigned sum = 0;
    unsigned offset = digitsString.size() / 2;
    size_t size = digitsString.size();
    for (size_t i = 0; i < digitsString.size(); i++)
    {
        if (digitsString[i] == digitsString[(i + offset) % size])
        {
            sum += digitsString[i] - '0';
        }
    }

    return sum;
}

int main()
{
    std::fstream fileIn("input.txt");
    std::string digitsString;

    fileIn >> digitsString;

    std::cout << "First part: " << sumOfDigitsMatchNextDigit(digitsString) << std::endl;
    std::cout << "Second part: " << sumOfDigitsMatchHalfwayRoundDigit(digitsString) << std::endl;
}
