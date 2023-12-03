#include <iostream>

#include <regex>

#include "helpers.h"

const char* INPUT = "input.txt";

struct Game
{
public:
    static Game parseGame(const std::string& line)
    {
        std::vector<std::string> lineParts;
        std::vector<std::string> gameParts = split(line, ':');

        std::smatch gameNumberMatch;
        std::regex_match(gameParts.at(0), gameNumberMatch, std::regex(".*?(\\d+)"));
        int number = std::stoi(gameNumberMatch[1].str());

        int red = INT8_MIN;
        int green = INT8_MIN;
        int blue = INT8_MIN;
        std::vector<std::string> gameSessions = split(gameParts.at(1), ';');

        for (const std::string& session : gameSessions)
        {
            std::vector<std::string> cubes = split(session, ',');
            for (const std::string& cube : cubes)
            {
                std::smatch cubeNumberMatch;
                std::regex_match(cube, cubeNumberMatch, std::regex(".*?(\\d+).*?"));
                int cubeNumber = std::stoi(cubeNumberMatch[1].str());

                if(containsAll(cube, "red") && red < cubeNumber)
                {
                    red = cubeNumber;
                }
                else if(containsAll(cube, "green") && green < cubeNumber)
                {
                    green = cubeNumber;
                }
                else if(containsAll(cube, "blue") && blue < cubeNumber)
                {
                    blue = cubeNumber;
                }
            }
        }

        return {number, red, green, blue};
    }


    int m_number;
    int m_maxRed;
    int m_maxGreen;
    int m_maxBlue;
};

std::vector<Game> readInput()
{
    std::ifstream file(INPUT);

    std::vector<Game> inputData;

    if(file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            inputData.push_back(Game::parseGame(line));
        }
    }

    return inputData;
}

int solve1(const std::vector<Game>& input)
{
    int result = 0;

    for (const Game& game : input)
    {
        if (game.m_maxRed <= 12 && game.m_maxGreen <= 13 && game.m_maxBlue <= 14)
        {
            result += game.m_number;
        }
    }

    return result;
}

long solve2(const std::vector<Game>& input)
{
    long result = 0;

    for (const Game& game : input)
    {
        result += game.m_maxRed * game.m_maxGreen * game.m_maxBlue;
    }

    return result;
}

int main()
{
    START_TIMER(point_1)

    std::vector<Game> input = readInput();
    std::cout << "Answer 1: " << solve1(input) << std::endl;
    std::cout << "Answer 2: " << solve2(input) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
