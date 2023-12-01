#include <iostream>

#include <regex>

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

std::string word2Digit(const std::string& line)
{
    std::string result = line;
    result = std::regex_replace(result, std::regex("one"), "o1e");
    result = std::regex_replace(result, std::regex("two"), "t2o");
    result = std::regex_replace(result, std::regex("three"), "t3e");
    result = std::regex_replace(result, std::regex("four"), "f4r");
    result = std::regex_replace(result, std::regex("five"), "f5e");
    result = std::regex_replace(result, std::regex("six"), "s6x");
    result = std::regex_replace(result, std::regex("seven"), "s7n");
    result = std::regex_replace(result, std::regex("eight"), "e8t");
    result = std::regex_replace(result, std::regex("nine"), "n9e");

    return result;
}

std::pair<int, int> findFirstLastDigit(const std::string& line)
{
    std::pair<int, int> firstLast;

    bool isFirst = true;
    for (char ch : line)
    {
        if (std::isdigit(ch))
        {
            if (isFirst)
            {
                firstLast.first = ch - '0';
                isFirst = false;
            }
            firstLast.second = ch - '0';
        }
    }

    return firstLast;
}

int solve(const std::vector<std::string>& lines)
{
    int result = 0;
    for (const std::string& line : lines)
    {
        std::pair<int, int> lineValue = findFirstLastDigit(line);

        result += lineValue.first*10 + lineValue.second;
    }

    return result;
}

int solve2(const std::vector<std::string>& lines)
{
    int result = 0;
    for (const std::string& line : lines)
    {
        std::pair<int, int> lineValue = findFirstLastDigit(word2Digit(line));

        result += lineValue.first*10 + lineValue.second;
    }

    return result;
}

int main()
{
    START_TIMER(point_1)

    std::vector<std::string> input = readInput();
    std::cout << "Answer: " << solve(input) << std::endl;
    std::cout << "Answer part 2: " << solve2(input) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
