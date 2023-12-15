#include <iostream>

#include "helpers.h"

const char* INPUT = "input_example.txt";

std::vector<std::string> readInput()
{
    std::ifstream file(INPUT);

    std::vector<std::string> inputData;

    if(file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            inputData.push_back(line);
        }
    }

    return inputData;
}

int main()
{
    START_TIMER(point_1)

    std::vector<std::string> input = readInput();

    std::cout << "Answer part 1: " << std::endl;

    std::cout << "Answer part 2: " << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
