#pragma once

#include "helpers.h"

#include <map>

// dst; src; length
typedef std::tuple<long, long, long> Range;

static bool isInRange(long value, const Range& range);
static bool isInRange(const Range& src, const Range& dst);
static bool isInRange(long start, long length, const Range& range);

struct Map
{

public:
    long getMappedValue(long value) const;

public:
    std::vector<Range> m_ranges;

};

class Almanac
{

public:
    Almanac(const std::string& input);

    long findLowestLocation();

    long findLowestLocationInRange();

private:
    void findLocations();

    long srcToDst(long value, long map);
    long srcToDst(long value, long index, const std::vector<Map>& map);

    Range lowestRange(int map);
    std::vector<Range> dstToSrc(int map, Range range);

private:
    std::vector<Map> m_maps;
    std::map<long, long> m_seedLocation;
    std::vector<std::pair<long, long>> m_seedRange;
};


