#include <iostream>

#include <map>

#include "helpers.h"

const char* INPUT = "input_example.txt";

enum HandType {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_KIND,
    FULL_HOUSE,
    FOUR_KIND,
    FIVE_KIND
};

HandType handType(int pairs, int maxPair)
{
    if(maxPair == 5)
    {
        return FIVE_KIND;
    }
    else if(maxPair == 4)
    {
        return FOUR_KIND;
    }
    else if (maxPair == 3 && pairs == 1)
    {
        return FULL_HOUSE;
    }
    else if(maxPair == 3)
    {
        return THREE_KIND;
    }
    else if (pairs == 2)
    {
        return TWO_PAIR;
    }
    else if(pairs == 1)
    {
        return ONE_PAIR;
    }
    else
    {
        return HIGH_CARD;
    }
}

static int cardValue(const std::string& hand, int index, bool jokerWildcard = false)
{
    char card = hand[index];
    if (std::isdigit(card))
    {
        return card - '0';
    }

    std::map<char, int> value =
        {
            std::make_pair('T', 10),
            std::make_pair('Q', 12),
            std::make_pair('K', 13),
            std::make_pair('A', 14)
        };

    if (jokerWildcard)
    {
        value['J'] = 1;
    }
    else
    {
        value['J'] = 11;
    }

    return value[card];
}

struct Hand
{
public:

    Hand(const std::string& hand, int bid)
        : m_hand(hand),
          m_bid(bid),
          m_jokerWildcard(false)
    {
        detectType();
    }

    void detectType()
    {
        int jokerCount = std::count(m_hand.begin(), m_hand.end(), 'J');

        if (m_jokerWildcard && jokerCount == 0)
        {
            return; //no need to detect type again
        }

        std::map<char, int> cards;
        for (char card : m_hand)
        {
            if (cards.find(card) != cards.end())
            {
                cards[card] = cards[card] + 1;
            }
            else
            {
                cards[card] = 1;
            }
        }

        int pairCount = 0;
        int maxPair = 0;
        for (std::pair<char, int> pair : cards)
        {
            if (pair.second > 1)
            {
                pairCount ++;
            }

            if (maxPair < pair.second)
            {
                maxPair = pair.second;
            }
        }

        m_type = handType(pairCount, maxPair);

        if (m_jokerWildcard)
        {
            HandType best = m_type;
            std::map<char, int> jokerCards;
            for (std::pair<char, int> pair : cards)
            {
                jokerCards = cards;
                if (pair.first == 'J')
                {
                    continue;
                }
                jokerCards[pair.first] = jokerCards[pair.first] + jokerCount;

                int jokerPairCount = 0;
                int jokerMaxPair = 0;
                for (std::pair<char, int> jokerPair : jokerCards)
                {
                    if (jokerPair.second / 2 > 0)
                    {
                        jokerPairCount += jokerPair.second / 2;
                    }

                    if (jokerMaxPair < jokerPair.second)
                    {
                        jokerMaxPair = jokerPair.second;
                    }
                }

                HandType jokerType = handType(jokerPairCount, jokerMaxPair);
                best = std::max(best, jokerType);
                jokerCards.clear();
            }

            m_type = best;
        }
    }

    bool operator<(const Hand& other) const
    {
        if (m_type != other.m_type)
        {
            return m_type < other.m_type;
        }

        for (int i = 0; i < m_hand.size(); ++i)
        {
            int leftValue = cardValue(m_hand, i, m_jokerWildcard);
            int rightValue = cardValue(other.m_hand, i, m_jokerWildcard);
            if (leftValue != rightValue)
            {
                return leftValue < rightValue;
            }
        }

        return false;
    }

public:
    std::string m_hand;
    int m_bid;
    bool m_jokerWildcard;
    HandType m_type;
};

std::vector<Hand> readInput()
{
    std::ifstream file(INPUT);

    std::vector<Hand> inputData;

    if(file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> lineParts = split(line, ' ');
            assert(lineParts.size() == 2);

            int bid = std::stoi(lineParts[1]);
            inputData.push_back(Hand(lineParts[0], bid));
        }
    }

    return inputData;
}

long long countWinnings(const std::vector<Hand>& input)
{
    long long result = 0;

    for (int i = 0; i < input.size(); ++i)
    {
        Hand hand = input[i];
        result += hand.m_bid * (i + 1);
    }

    return result;
}


int main()
{
    START_TIMER(point_1)

    std::vector<Hand> input = readInput();
//    std::sort(input.begin(),  input.end());

//    std::cout << "Answer part 1: " << countWinnings(input) << std::endl;

    for (int i = 0; i < input.size(); ++i)
    {
        input[i].m_jokerWildcard = true;
        input[i].detectType();
    }

    std::sort(input.begin(),  input.end());


    std::cout << "Answer part 2: " << countWinnings(input) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
