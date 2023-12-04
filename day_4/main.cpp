#include <iostream>
#include <regex>

#include "helpers.h"

const char* INPUT = "input.txt";

struct Card
{
public:

    std::vector<int> wins() const
    {
        std::vector<int> intersection;

        std::vector<int> winning = m_winningNumbers;
        std::sort(winning.begin(),  winning.end());

        std::vector<int> numbers = m_numbers;
        std::sort(numbers.begin(),  numbers.end());

        std::set_intersection(winning.begin(),  winning.end(),
                              numbers.begin(),  numbers.end(),
                              std::back_inserter(intersection));

        return intersection;
    }

    int calcPoints() const
    {
        std::vector<int> intersection = wins();
        int result = 0;
        if (!intersection.empty())
        {
            result = 1;
            intersection.pop_back();

            for (int number : intersection)
            {
                result *= 2;
            }
        }

        return result;
    }

public:
    int m_number;
    std::vector<int> m_winningNumbers;
    std::vector<int> m_numbers;

    long m_copies = 1;

};

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

std::vector<Card> parseCards(const std::vector<std::string>& input)
{
    std::vector<Card> cards;
    for (const std::string& cardLine : input)
    {

        std::vector<std::string> cardParts = split(cardLine, ':');
        std::smatch cardNumberMatch;
        std::regex_match(cardParts.at(0), cardNumberMatch, std::regex(".*?(\\d+)"));
        int cardNumber = std::stoi(cardNumberMatch[1].str());

        std::vector<std::string> numberParts = split(cardParts[1], '|');

        std::vector<std::string> winningNumbersParts = split(numberParts[0], ' ');
        std::vector<int> winningNumbers;
        for (const std::string& number : winningNumbersParts)
        {
            winningNumbers.push_back(std::stoi(number));
        }

        std::vector<std::string> numbersParts = split(numberParts[1], ' ');
        std::vector<int> numbers;
        for (const std::string& number : numbersParts)
        {
            numbers.push_back(std::stoi(number));
        }

        Card card = {cardNumber, winningNumbers, numbers};
        cards.push_back(card);
    }

    return cards;
}



int solve1(const std::vector<std::string>& input)
{
    int result = 0;
    std::vector<Card> cards = parseCards(input);

    for (const Card& card : cards)
    {
        result += card.calcPoints();
    }

    return result;
}

long solve2(const std::vector<std::string>& input)
{
    std::vector<Card> cards = parseCards(input);
    for (int i = 0; i < cards.size(); ++i)
    {
        Card card = cards.at(i);
        int wins = card.wins().size();
        for (int j = i + 1; j < i + 1 + wins; ++j)
        {
            if(j < cards.size() && wins > 0)
            {
                cards.at(j).m_copies += 1 * card.m_copies;
            }
        }
    }

    long result = 0;
    for (const Card& card : cards)
    {
        result += card.m_copies;
    }

    return result;
}

int main()
{
    START_TIMER(point_1)

    std::vector<std::string> input = readInput();
    std::cout << "Answer 1: " << solve1(input) << std::endl;
    std::cout << "Answer 2: " << solve2(input) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
