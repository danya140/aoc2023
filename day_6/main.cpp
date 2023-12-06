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
    std::vector<long> times = parseNumbers<long>(input[0], ' ');
    std::vector<long> distances = parseNumbers<long>(input[1], ' ');
    for (int i = 0; i < times.size(); ++i)
    {
        std::pair<long, long> pair = std::make_pair(times[i], distances[i]);
        races.push_back(pair);
    }

    std::cout << "Answer 1: " << countWinningVariants(races) << std::endl;

    std::string time;
    std::string distance;
    for (int i = 0; i < times.size(); ++i)
    {
        time += std::to_string(times[i]);
        distance += std::to_string(distances[i]);
    }

    std::cout << "Answer 2: " << findWinningVariants(std::stol(time), std::stol(distance)) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
