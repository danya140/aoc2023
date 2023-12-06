#include "helpers.h"

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> result;

    auto position = str.find(delimiter);
    unsigned int i = 0;

    while (position != std::string::npos)
    {
        auto substr = str.substr(i, position - i);
        if (!substr.empty())
        {
            result.push_back(substr);
        }
        i = position + 1;

        position = str.find(delimiter, i);
    }

    std::string last = str.substr(i, std::min(position, str.size()) - i + 1);
    if(!last.empty())
    {
        result.push_back(last);
    }

    return result;
}

bool acceptableValues(char ch)
{
    return std::isdigit(ch) || ch == ',' || ch == '.';
}

double median(const std::vector<int>& input)
{
    int size = input.size();
    if (size == 0)
    {
        return 0;
    }
    std::vector<int> inputCopy(input);
    std::sort(inputCopy.begin(), inputCopy.end());

    double median = 0;

    if(size % 2 == 0)
    {
        int d = inputCopy[(size/2)] + inputCopy[(size/2) -1];
        median = d/2;
    }
    else
    {
        median = inputCopy[size/2];
    }

    return median;
}


bool containsAll(const std::string& str, const std::string& searchPattern)
{
    std::string sortedStr(str);
    std::string pattern(searchPattern);

    std::sort(sortedStr.begin(), sortedStr.end());
    std::sort(pattern.begin(), pattern.end());

    std::string intersection;

    std::set_intersection(sortedStr.begin(), sortedStr.end(),
                          pattern.begin(), pattern.end(),
                          std::back_inserter(intersection));

    return intersection.size() == pattern.size();
}


