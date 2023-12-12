#include <iostream>

#include <map>

#include "helpers.h"

const char* INPUT = "input.txt";

typedef std::pair<std::string, std::string> Node;

struct Network
{
public:
    std::string m_instructions;

    std::map<std::string, Node> m_map;
};

struct Point
{

public:
    std::string m_currentNode;
    long m_steps;
    std::string m_instructions;
};

Network readInput()
{
    std::ifstream file(INPUT);

    Network inputData;

    if(file.is_open())
    {
        std::string line;
        std::getline(file, line);
        inputData.m_instructions = line;

        while (std::getline(file, line))
        {
            std::vector<std::string> nodeParts = split(line, '=');
            if (nodeParts.size() == 2)
            {
                std::string nodeName = nodeParts[0].substr(0, nodeParts[0].size() - 1);

                std::vector<std::string> dstNodes = split(nodeParts[1], ',');
                std::string leftNode = dstNodes[0].substr(2, dstNodes[0].size() - 1);
                std::string rightNode = dstNodes[1].substr(1, dstNodes[1].size() - 2);

                inputData.m_map[nodeName] = std::make_pair(leftNode, rightNode);
            }
        }
    }

    return inputData;
}

long countSteps(Network input)
{
    long result = 0;

    std::string instructions = input.m_instructions;
    std::string currentLocation = "AAA";
    while (currentLocation != "ZZZ")
    {
        char direction = instructions.front();
        instructions.erase(instructions.begin());

        Node node = input.m_map[currentLocation];
        if (direction == 'L')
        {
            currentLocation = node.first;
        }
        else
        {
            currentLocation = node.second;
        }

        if (instructions.empty())
        {
            instructions = input.m_instructions;
        }

        result++;
    }

    return result;
}

Point countSingle(Network input, const Point& lastPoint)
{
    long result = lastPoint.m_steps;

    std::string instructions = lastPoint.m_instructions;
    std::string currentLocation = lastPoint.m_currentNode;
    do
    {
        char direction = instructions.front();
        instructions.erase(instructions.begin());

        Node node = input.m_map[currentLocation];
        if (direction == 'L')
        {
            currentLocation = node.first;
        }
        else
        {
            currentLocation = node.second;
        }

        if (instructions.empty())
        {
            instructions = input.m_instructions;
        }


        result++;
    } while (currentLocation.back() != 'Z');

    return {currentLocation, result, instructions};
}


long long gcd(long long a, long long b)
{
    while (true)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

long long lcm(long long a, long long b)
{
    long long temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

long countMulti(Network input)
{
    std::map<int, Point> map;
    int index = 0;
    for (std::pair<std::string, Node> pair : input.m_map)
    {
        if (pair.first.back() == 'A')
        {
            map[index] = {pair.first, 0, input.m_instructions};
            index++;
        }
    }

    //Check loop
    std::vector<long> steps;
    for (int i = 0; i < map.size(); ++i)
    {
        Point p1 = countSingle(input, map[i]);
        Point p2 = countSingle(input, p1);
        Point p3 = countSingle(input, p2);

        if (p2.m_steps/2 == p1.m_steps && p3.m_steps/3 == p1.m_steps)
        {
            steps.push_back(p1.m_steps);
        }
        else
        {
            std::cout << "wtf" << std::endl;
        }

    }


    return std::accumulate(steps.begin(),  steps.end(), 1ll, lcm);
}

int main()
{
    START_TIMER(point_1)

    Network input = readInput();

    std::cout << "Answer part 1: " << countSteps(input) << std::endl;

    std::cout << "Answer part 2: " << countMulti(input) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
