#include <iostream>

#include <map>

#include "helpers.h"

const char* INPUT = "input_example.txt";

typedef std::pair<std::string, std::string> Node;

struct Network
{
public:
    std::string m_instructions;

    std::map<std::string, Node> m_map;
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

int main()
{
    START_TIMER(point_1)

    Network input = readInput();

    std::cout << "Answer part 1: " << countSteps(input) << std::endl;

    std::cout << "Answer part 2: " << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
