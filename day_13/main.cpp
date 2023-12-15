#include <iostream>

#include <map>

#include "helpers.h"

const char* INPUT = "input_example2.txt";

typedef std::pair<int, int> Match;

typedef std::vector<char> Line;
typedef std::vector<Line> Pattern;

std::vector<Pattern> readInput()
{
    std::ifstream file(INPUT);

    std::vector<Pattern> inputData;

    if(file.is_open())
    {
        std::string line;
        Pattern pattern;
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                inputData.push_back(pattern);
                pattern.clear();
            }
            else
            {
                std::vector<char> row;
                row.insert(row.begin(), line.begin(),  line.end());
                pattern.push_back(row);
            }
        }

        inputData.push_back(pattern);
    }

    return inputData;
}

std::vector<char> getColumn(const Pattern& pattern, int index)
{
    Line column;

    for (const Line& line : pattern)
    {
        column.push_back(line[index]);
    }

    return column;
}

std::map<int, int> findVerticalMatches(const Pattern& pattern)
{
    std::map<int, int> matches;

    for (int i = 0; i < pattern[0].size(); ++i)
    {
        int matchColumn = -1;
        for (int j = 0; j < pattern[0].size(); ++j)
        {
            Line left = getColumn(pattern, i);
            Line right = getColumn(pattern, pattern[0].size() - j);
            if (left == right && i != pattern[0].size() - j)
            {
                matchColumn = pattern[0].size() - j;
                break;
            }
        }

        matches[i] = matchColumn;
    }

    return matches;
}

std::map<int, int> findHorizontalMatches(const Pattern& pattern)
{
    std::map<int, int> matches;

    for (int i = 0; i < pattern.size(); ++i)
    {
        int matchRow = -1;
        for (int j = 0; j < pattern.size(); ++j)
        {
            if (pattern[i] == pattern[pattern.size() - j] && i != pattern.size() - j)
            {
                matchRow = pattern.size() - j;
                break;
            }
        }

        matches[i] = matchRow;
    }

    return matches;
}

int findReflection(std::map<int, int> matches)
{
    int direction = 1;
    int start = 1;
    std::pair<int, int> previous = *matches.begin();

    int reflected = -1;
    for (int i = start; i < matches.size() || i < 0; i += direction)
    {
        std::pair<int, int> current = std::make_pair(i, matches[i]);
        if(current.first - direction != previous.first || current.second + direction != previous.second)
        {
            reflected = -1;
            break;
        }
        else
        {
            if (current.first == previous.second && current.second == previous.first)
            {
                reflected = std::min(current.first, current.second);
                break;
            }
            previous = current;
        }
    }

    //right
    direction = -1;
    start = matches.size() - 1;
    previous = std::make_pair(start, matches[start]);
    start--;
    int reflectedRight = -1;
    for (int i = start; i < matches.size() || i < 0; i += direction)
    {
        std::pair<int, int> current = std::make_pair(i, matches[i]);
        if(current.first - direction != previous.first || current.second + direction != previous.second)
        {
            reflectedRight = -1;
            break;
        }
        else
        {
            if (current.first == previous.second && current.second == previous.first)
            {
                reflectedRight = std::min(current.first, current.second);
                break;
            }
            previous = current;
        }
    }

    reflected = std::max(reflected, reflectedRight);

    if (matches[0] == -1 && matches[matches.size() - 1] == -1)
    {
        reflected = -1;
    }

    return reflected + 1;
}

int findVerticalReflection(const Pattern& pattern)
{
    std::map<int, int> matches = findVerticalMatches(pattern);
    return findReflection(matches);
}

int findHorizontalReflection(const Pattern& pattern)
{
    std::map<int, int> matches = findHorizontalMatches(pattern);
    return findReflection(matches);
}

long calcReflection(const std::vector<Pattern>& patterns)
{
    long result = 0;
    int  index = 0;
    for (const Pattern& pattern : patterns)
    {
        int vertical = findVerticalReflection(pattern);
        int horizontal = findHorizontalReflection(pattern);

        assert((vertical > 0 && horizontal == 0) || (vertical == 0 && horizontal > 0));
        result += vertical + horizontal * 100;

        index++;
    }

    return result;
}

int main()
{
    START_TIMER(point_1)

    std::vector<Pattern> input = readInput();

    std::cout << "Answer part 1: " << calcReflection(input) << std::endl;

    std::cout << "Answer part 2: " << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
