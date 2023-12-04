#include <iostream>

#include "helpers.h"

const char* INPUT = "input.txt";


struct Number
{
public:

    bool haveLocation(int x, int y) const
    {
        for (const std::pair<int, int>& location : m_location)
        {
            if (location.first == x && location.second == y)
            {
                return true;
            }
        }

        return false;
    }

    bool operator==(const Number& other) const
    {
        return m_location == other.m_location;
    }

    std::vector<std::pair<int, int>> m_location;
    int m_value = 0;

};

std::vector<std::vector<char>> readInput()
{
    std::ifstream file(INPUT);

    std::vector<std::vector<char>> inputData;

    if(file.is_open())
    {
        std::vector<char> line;

        while (file.good())
        {
            char ch;
            file.get(ch);
            if (ch == '\n' && !line.empty())
            {
                inputData.push_back(line);
                line.clear();
            }
            else
            {
                line.push_back(ch);
            }
        }
    }

    return inputData;
}

bool haveAdjacentSymbol(const std::vector<std::vector<char>>& input, int x, int y)
{
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (x + j >= 0 && x + j < input.at(y).size() && y + i >= 0 && y + i < input.size())
            {
                char ch = input[y+i][x+j];
                if (!isdigit(ch) && ch != '.')
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool isCorrect(const std::vector<std::vector<char>>& input, const Number& number)
{
    for (std::pair<int, int> coordinate : number.m_location)
    {
        bool haveSymbol = haveAdjacentSymbol(input, coordinate.first, coordinate.second);
        if (haveSymbol)
        {
            return true;
        }
    }

    return false;
}

std::vector<Number> parseNumbers(const std::vector<std::vector<char>>& input)
{
    std::vector<Number> numbers;

    Number number;
    for (int i = 0; i < input.size(); ++i)
    {
        number = Number();
        for (int j = 0; j < input.at(i).size(); ++j)
        {
            if (std::isdigit(input[i][j]))
            {
                number.m_location.push_back(std::make_pair(j, i));
                number.m_value = number.m_value * 10 + (input[i][j] - '0');
            }
            else if(!number.m_location.empty())
            {
                numbers.push_back(number);
                number = Number();
            }
        }

        if(!number.m_location.empty())
        {
            numbers.push_back(number);
        }
    }

    return numbers;
}

int solve1(const std::vector<std::vector<char>>& input)
{
    int result = 0;
    std::vector<Number> numbers = parseNumbers(input);
    for (const Number& number : numbers)
    {
        if(isCorrect(input, number))
        {
            result += number.m_value;
        }
    }

    return result;
}

std::pair<int, int> gearLocation(const std::vector<std::vector<char>>& input, int x, int y)
{
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (x + j >= 0 && x + j < input.at(y).size() && y + i >= 0 && y + i < input.size())
            {
                char ch = input[y+i][x+j];
                if (ch == '*')
                {
                    return std::make_pair(x+j, y+i);
                }
            }
        }
    }

    return std::make_pair(0, 0);
}

std::pair<int, int> gearLocation(const std::vector<std::vector<char>>& input, const Number& number)
{
    for (std::pair<int, int> coordinate : number.m_location)
    {
        std::pair<int, int> location = gearLocation(input, coordinate.first, coordinate.second);
        if (location.first != 0 && location.second != 0)
        {
            return location;
        }
    }

    return std::make_pair(0, 0);
}

/*
 * That's quiet bloat function, but it works
 */
Number findAdjacentGear(const std::vector<std::vector<char>>& input, const Number& number)
{
    std::pair<int, int> location = gearLocation(input, number);

    //find other number
    Number result;
    int x = location.first;
    int y = location.second;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (x + j >= 0 && x + j < input.at(y).size() && y + i >= 0 && y + i < input.size())
            {
                char ch = input[y+i][x+j];
                if (number.haveLocation(x+j, y+i)) // skip current number
                {
                    continue;
                }

                if (std::isdigit(ch))
                {
                    int left = x + j;
                    while (left - 1 >= 0 && std::isdigit(input[y+i][left - 1]))
                    {
                        left -= 1;
                    }

                    for (int k = left; k < input[y+i].size(); ++k)
                    {
                        ch = input[y+i][k];
                        if (std::isdigit(ch))
                        {
                            result.m_location.push_back(std::make_pair(k, y+i));
                            result.m_value = result.m_value*10 + (ch - '0');
                        }
                        else
                        {
                            return result;
                        }
                    }
                }
            }
        }
    }
    return result;
}

long solve2(const std::vector<std::vector<char>>& input)
{
    long result = 0;
    std::vector<Number> numbers = parseNumbers(input);
    std::vector<Number> gears;
    for (const Number& number : numbers)
    {
        std::pair<int, int> location = gearLocation(input, number);
        if(location.first != 0 && location.second != 0 )
        {
            Number otherGear = findAdjacentGear(input, number);
            if (otherGear.m_value != 0 && std::find(gears.begin(),  gears.end(), number) == gears.end())
            {
                gears.push_back(number);
                gears.push_back(otherGear);
                result += number.m_value * otherGear.m_value;
            }
        }
    }

    return result;
}


int main()
{
    START_TIMER(point_1)

    std::vector<std::vector<char>> input = readInput();
    std::cout << "Answer 1: " << solve1(input) << std::endl;

    std::cout << "Answer 2: " << solve2(input) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
