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
std::vector<std::string> split(const std::string& str, char delimiter);

/**
 * symbols acceptable for converting to number
 */
bool acceptableValues(char ch);

/**
 * Calculate median of input vector
 */
double median(const std::vector<int>& input);

/**
 * Check if @a str contains @a searchPattern
 */
bool containsAll(const std::string& str, const std::string& searchPattern);


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
 * convert string to vector of T
 * @param str - string containing numbers
 * @param delimiter - how values are splitted
 */
template <class T>
std::vector<T> parseNumbers(const std::string& str, char delimiter)
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
std::string vector2String(const std::vector<T>& input, const std::string& delimiter = "")
{
    std::stringstream stream;
    std::copy(input.begin(), input.end(), std::ostream_iterator<T>(stream, delimiter.c_str()));

    return stream.str();
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

