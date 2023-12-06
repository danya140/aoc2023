#include <iostream>
#include <map>

#include "helpers.h"

const char* INPUT = "input.txt";

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


long findWinningVariants(long time, long distance)
{
    long result = 0;
    for (int i = 0; i <= time; ++i)
    {
        long calcDistance = (time - i) * i;
        if (calcDistance > distance)
        {
            result++;
        }
    }

    return result;
}

long countWinningVariants(const std::vector<std::pair<long, long>>& races)
{
    long result = 1;
    for (const std::pair<long, long>& pair : races)
    {
        result *= findWinningVariants(pair.first, pair.second);
    }

    return result;
}


int main()
{
    START_TIMER(point_1)

    std::vector<std::string> input = readInput();
    std::vector<std::pair<long, long>> races;
    std::vector<std::string> timeParts = split(input[0], ' ');
    std::vector<std::string> distanceParts = split(input[1], ' ');
    for (int i = 1; i < timeParts.size(); ++i)
    {
        std::pair<long, long> pair = std::make_pair(std::stol(timeParts[i]), std::stol(distanceParts[i]));
        races.push_back(pair);
    }

    std::cout << "Answer 1: " << countWinningVariants(races) << std::endl;

    std::string time;
    std::string distance;
    for (int i = 1; i < timeParts.size(); ++i)
    {
        time += timeParts[i];
        distance += distanceParts[i];
    }

    std::cout << "Answer 2: " << findWinningVariants(std::stol(time), std::stol(distance)) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
