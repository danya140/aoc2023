#include <iostream>

#include "helpers.h"

#include "Almanac.h"

const char* INPUT = "input.txt";

std::string readInput()
{
    std::ifstream file(INPUT);

    std::string inputData;

    if(file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            inputData += line + '\n';
        }
    }

    return inputData;
}

int main()
{
    START_TIMER(point_1)

    std::string input = readInput();
    auto almanac = new Almanac(input);
    std::cout << "Answer 1: " << almanac->findLowestLocation() << std::endl;

    std::cout << "Answer 2: " << almanac->findLowestLocationInRange() << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
