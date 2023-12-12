#include <iostream>

#include <map>

#include "helpers.h"

const char* INPUT = "input.txt";

enum Direction
{
    LEFT,
    TOP,
    RIGHT,
    BOTTOM,
    NONE
};

typedef std::pair<int, int> Coordinate;
typedef std::vector<char> Row;
typedef std::pair<Direction, Direction> PipeDirection;

static std::map<char, PipeDirection> PIPE_DIRECTION_MAP =
    {
        {'|', std::make_pair(TOP, BOTTOM)},
        {'-', std::make_pair(LEFT, RIGHT)},
        {'L', std::make_pair(TOP, RIGHT)},
        {'J', std::make_pair(LEFT, TOP)},
        {'7', std::make_pair(LEFT, BOTTOM)},
        {'F', std::make_pair(RIGHT, BOTTOM)}
    };


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

Coordinate findStart(const std::vector<Row>& map)
{
    int x = 0;
    int y = 0;
    for (const Row& row : map)
    {
        x = 0;
        for (char ch : row)
        {
            if (ch == 'S')
            {
                return std::make_pair(x, y);
            }
            ++x;
        }
        ++y;
    }

    return std::make_pair(x, y);
}

PipeDirection getDirection(char ch)
{
    return PIPE_DIRECTION_MAP[ch];
}

std::vector<Row> replaceStart(std::vector<Row> input, Coordinate start)
{
    std::vector<Row> result = input;
    PipeDirection left = std::make_pair(NONE, NONE);
    PipeDirection top = std::make_pair(NONE, NONE);
    PipeDirection right = std::make_pair(NONE, NONE);
    PipeDirection bottom = std::make_pair(NONE, NONE);

    if (start.first > 0)
    {
        left = getDirection(input[start.second][start.first - 1]);
    }

    if (start.second > 0)
    {
        top = getDirection(input[start.second - 1][start.first]);
    }

    if (start.first < input[0].size())
    {
        right = getDirection(input[start.second][start.first + 1]);
    }

    if (start.second < input.size())
    {
        bottom = getDirection(input[start.second + 1][start.first]);
    }

    bool leftConnected = left.second == RIGHT || left.first == RIGHT;
    bool topConnected = top.second == BOTTOM;
    bool rightConnected = right.first == LEFT;
    bool bottomConnected = bottom.first == TOP || bottom.second == TOP;

    char startReplacement = '+';
    if(leftConnected && rightConnected)
    {
        startReplacement = '-';
    }
    if (leftConnected && topConnected)
    {
        startReplacement = 'J';
    }
    if (leftConnected && bottomConnected)
    {
        startReplacement = '7';
    }
    if (topConnected && bottomConnected)
    {
        startReplacement = '|';
    }
    if (rightConnected && topConnected)
    {
        startReplacement = 'L';
    }
    if (rightConnected && bottomConnected)
    {
        startReplacement = 'F';
    }

    assert(startReplacement != '+');
    result[start.second][start.first] = startReplacement;
    return result;
}

Coordinate nextStep(Coordinate current, Direction direction)
{
    Coordinate next = current;
    if (direction == LEFT)
    {
        next.first -= 1;
    }
    else if(direction == TOP)
    {
        next.second -= 1;
    }
    else if (direction == RIGHT)
    {
        next.first += 1;
    }
    else if (direction == BOTTOM)
    {
        next.second += 1;
    }

    return next;
}

long findFurthestPoint(const std::vector<Row>& input)
{
    long result = 0;
    Coordinate start = findStart(input);
    std::vector<Row> map = replaceStart(input, start);

    Coordinate current = start;
    Coordinate previous = current;
    do
    {
        PipeDirection currentDirection = getDirection(map[current.second][current.first]);

        Direction nextDirection = currentDirection.first;
        Coordinate next = nextStep(current, nextDirection);
        if (next == previous)
        {
            nextDirection = currentDirection.second;
            next = nextStep(current, nextDirection);
        }

        previous = current;
        current = next;

        result++;

    } while (current != start);


    return result/2;
}

std::vector<Coordinate> getVertices(std::vector<Row> map, Coordinate start)
{
    std::vector<Coordinate> result;
    Coordinate current = start;
    Coordinate previous = current;
    do
    {
        PipeDirection currentDirection = getDirection(map[current.second][current.first]);

        Direction nextDirection = currentDirection.first;
        Coordinate next = nextStep(current, nextDirection);
        if (next == previous)
        {
            nextDirection = currentDirection.second;
            next = nextStep(current, nextDirection);
        }

        char currentChar = map[current.second][current.first];
        if (currentChar == '7' || currentChar == 'F' || currentChar == 'L' || currentChar == 'J')
        {
            result.push_back(current);
        }

        previous = current;
        current = next;

    } while (current != start);

    return result;
}

long getArea(std::vector<Coordinate> vertices)
{
    long area = 0;

    for (int i = 0; i < vertices.size(); ++i)
    {
        long index = (i + 1) % vertices.size();
        Coordinate coordinate = vertices[i];
        Coordinate next = vertices[index];
        area += coordinate.first * next.second - coordinate.second * next.first;
    }

    area = std::abs(area) / 2;
    return area;
}

long smartSolution(std::vector<Row> input)
{
    Coordinate start = findStart(input);
    std::vector<Row> map = replaceStart(input, start);

    std::vector<Coordinate> vertices = getVertices(map, start);
    long totalPoints = findFurthestPoint(input) * 2;
    long area = getArea(vertices);

    long result = area - totalPoints/2 + 1;
    return result;
}

int main()
{
    START_TIMER(point_1)

    std::vector<Row> input = readInput();

    std::cout << "Answer part 1: " << findFurthestPoint(input) << std::endl;

    std::vector<Coordinate> test;
    test.push_back(std::make_pair(2,1));
    test.push_back(std::make_pair(4,5));
    test.push_back(std::make_pair(7,8));
    long area = getArea(test);

    assert(area == 3);

    std::cout << "Answer part 2: " << smartSolution(input) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
