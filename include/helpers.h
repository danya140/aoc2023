#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <cmath>
#include <numeric>
#include <set>
#include <fstream>

/**
 * Split string using delimiter
 * @return vector of string after split
 */
inline std::vector<std::string> split(const std::string& str, char delimiter)
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

/**
 * Convert str to T
 */
template <typename T>
T convertTo (const std::string &str)
{
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
}

/**
 * symbols acceptable for converting to number
 */
bool acceptableValues(char ch)
{
    return std::isdigit(ch) || ch == ',' || ch == '.';
}

/**
 * convert string to vector of T
 * @param str - string containing numbers
 * @param delimiter - how values are splitted
 */
template <class T>
inline std::vector<T> parseNumbers(const std::string& str, char delimiter)
{
    std::vector<T> result;

    std::vector<std::string> splittedString = split(str, delimiter);
    for (const std::string& part : splittedString)
    {
        if (std::all_of(part.begin(),  part.end(), &acceptableValues))
        {
            result.push_back(convertTo<T>(part));
        }
    }

    return result;
}

/**
 * Join all elements of input vector
 */
template <class T>
inline std::string vector2String(const std::vector<T>& input, std::string delimiter = "")
{
    std::stringstream stream;
    std::copy(input.begin(), input.end(), std::ostream_iterator<T>(stream, delimiter.c_str()));

    return stream.str();
}

/**
 * Calculate median of input vector
 */
inline double median(const std::vector<int>& input)
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

/**
 * Check if @a str contains @a searchPattern
 */
inline bool containsAll(const std::string& str, const std::string& searchPattern)
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

/**
 * Create variable with current epoch time
 */
#define START_TIMER(point) int point_##point = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

/**
 * Calculate and print time between start point and current point
 */
#define STOP_TIMER(point) int stop_point_##point = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); \
    std::cout << "Execution time for point " << #point << ": " << stop_point_##point - point_##point << std::endl; \

