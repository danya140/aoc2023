#include <iostream>

#include <map>
#include <queue>

#include "helpers.h"

const char* INPUT = "input.txt";

typedef std::pair<int, int> Coordinate;
typedef std::vector<char> Row;
typedef std::pair<Coordinate, Coordinate> StarPair;

std::vector<Row> readInput()
{
    std::ifstream file(INPUT);

    std::vector<Row> inputData;

    if(file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            Row row;
            row.insert(row.begin(), line.begin(),  line.end());
            inputData.push_back(row);
        }
    }

    return inputData;
}

bool isRowEmpty(const Row& row)
{
    for (char ch : row)
    {
        if (ch == '#')
        {
            return false;
        }
    }

    return true;
}

bool isColumnEmpty(const std::vector<Row>& input, int column)
{
    for (const Row& row : input)
    {
        if (row[column] == '#')
        {
            return false;
        }
    }

    return true;
}

std::vector<Coordinate> findGalaxies(const std::vector<Row>& input)
{
    std::vector<Coordinate> galaxies;

    int x = 0;
    int y = 0;
    for (const Row& row : input)
    {
        x = 0;
        for (char ch : row)
        {
            if(ch == '#')
            {
                galaxies.push_back(std::make_pair(x, y));
            }
            x++;
        }

        y++;
    }

    return galaxies;
}

std::vector<StarPair> findPairs(const std::vector<Coordinate>& galaxies)
{
    std::vector<StarPair> pairs;

    for (int i = 0; i < galaxies.size(); ++i)
    {
        for (int j = i + 1; j < galaxies.size(); ++j)
        {
            pairs.push_back(std::make_pair(galaxies[i], galaxies[j]));
        }
    }

    return pairs;
}

long findDistance(StarPair starPair)
{
    Coordinate from = starPair.first;
    Coordinate to = starPair.second;

    long distance = std::abs(from.first - to.first) + std::abs(from.second - to.second);

    return distance;
}

long shortestPaths(const std::vector<Row>& input, long multiplier = 1)
{
    std::vector<Coordinate> stars = findGalaxies(input);

    //fill gaps without changing galaxy map
    for (int i = 0; i < stars.size(); ++i)
    {
        Coordinate star = stars[i];
        long newColumns = 0;
        for (int j = 0; j < star.first; ++j)
        {
            if (isColumnEmpty(input, j))
            {
                newColumns++;
            }
        }

        long newRows = 0;
        for (int j = 0; j < star.second; ++j)
        {
            if(isRowEmpty(input[j]))
            {
                newRows++;
            }
        }

        long newX = newColumns * multiplier + star.first - newColumns;
        long newY = newRows * multiplier + star.second - newRows;
        stars[i] = std::make_pair(newX, newY);
    }

    std::vector<StarPair> pairs = findPairs(stars);

    long result = 0;
    for (const StarPair& pair : pairs)
    {
        long distance = findDistance(pair);
        assert(distance > 0);
        result += distance;

    }

    return result;
}

int main()
{
    START_TIMER(point_1)

    std::vector<Row> input = readInput();
    std::cout << "Answer part 1: " << shortestPaths(input, 2) << std::endl;

    std::cout << "Answer part 2: " << shortestPaths(input, 1000000) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
