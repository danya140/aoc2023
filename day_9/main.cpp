#include <iostream>

#include "helpers.h"

const char* INPUT = "input.txt";

typedef std::vector<long> Sequence;

std::vector<Sequence> readInput()
{
    std::ifstream file(INPUT);

    std::vector<Sequence> inputData;

    if(file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            Sequence sequence = parseNumbers<long>(line, ' ');
            inputData.push_back(sequence);
        }
    }

    return inputData;
}

bool isZeroSequence(const Sequence& sequence)
{
    for (long number : sequence)
    {
        if (number != 0)
        {
            return false;
        }
    }

    return true;
}

Sequence findDiffSequence(const Sequence& sequence)
{
    std::vector<long> diffSequence;
    for (int i = 0; i < sequence.size() - 1; ++i)
    {
        long left = sequence[i];
        long right = sequence[i + 1];
        long diff = right - left;
        diffSequence.push_back(diff);
    }

    return diffSequence;
}

std::vector<Sequence> formPredictionTable(const Sequence& sequence)
{
    std::vector<Sequence> predictionTable;
    predictionTable.push_back(sequence);

    Sequence currentSequence = sequence;
    while (!isZeroSequence(currentSequence))
    {
        currentSequence = findDiffSequence(currentSequence);
        predictionTable.push_back(currentSequence);
    }

    return predictionTable;
}

std::vector<Sequence> extrapolate(const std::vector<Sequence>& predictions)
{
    std::vector<Sequence> extrapolation = predictions;
    for (int i = extrapolation.size() - 1; i > 0; --i)
    {
        long diff = extrapolation[i].back();
        long left = extrapolation[i - 1].back();

        long result = left + diff;

        extrapolation[i - 1].push_back(result);
    }

    return extrapolation;
}

std::vector<Sequence> extrapolateBackward(const std::vector<Sequence>& predictions)
{
    std::vector<Sequence> extrapolation = predictions;
    Sequence last = extrapolation[extrapolation.size() - 1];
    last.insert(last.begin(), 0);
    extrapolation[extrapolation.size() - 1] = last;
    for (int i = extrapolation.size() - 1; i > 0; --i)
    {
        long diff = extrapolation[i].front();
        long left = extrapolation[i - 1].front();

        long result = left - diff;

        Sequence next = extrapolation[i - 1];
        next.insert(next.begin(), result);
        extrapolation[i - 1] = next;
    }

    return extrapolation;
}

long analyzeOasis(const std::vector<Sequence>& sequences)
{
    long result = 0;

    for (const Sequence& sequence : sequences)
    {
        result += extrapolate(formPredictionTable(sequence))[0].back();
    }


    return result;
}

long analyzeOasisBackward(const std::vector<Sequence>& sequences)
{
    long result = 0;

    for (const Sequence& sequence : sequences)
    {
        result += extrapolateBackward(formPredictionTable(sequence))[0].front();
    }


    return result;
}


int main()
{
    START_TIMER(point_1)

    std::vector<Sequence> input = readInput();

    std::cout << "Answer part 1: " << analyzeOasis(input) << std::endl;

//    std::vector<Sequence> seq = extrapolateBackward(formPredictionTable(input[2]));
    std::cout << "Answer part 2: " << analyzeOasisBackward(input) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
