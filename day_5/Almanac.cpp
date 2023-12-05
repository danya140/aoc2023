#include "Almanac.h"

bool isInRange(long value, const Range &range)
{
    return value >= std::get<1>(range) && value <= std::get<1>(range) + std::get<2>(range) - 1;
}

bool isInRange(const Range &src, const Range &dst)
{
    long srcStart = std::get<1>(src);
    long srcEnd = std::get<1>(src) + std::get<2>(src) - 1;

    long dstStart = std::get<0>(dst);
    long dstEnd = std::get<0>(dst) + std::get<2>(dst) - 1;

    return (dstStart <= srcStart && srcStart <= dstEnd) ||
            (dstStart <= srcEnd && srcEnd <= dstEnd);
}

bool isInRange(long start, long length, const Range& range)
{
    long srcStart = std::get<1>(range);
    long srcEnd = std::get<1>(range) + std::get<2>(range) - 1;

    return (start <= srcStart && srcStart <= start + length - 1) ||
        (start <= srcEnd && srcEnd <= start + length - 1);
}

Range limitRange(const Range& newRange, const Range& limitRange)
{
    // new dst
    long dst = std::get<0>(newRange);
    long dstDiff = 0;
    if (std::get<1>(limitRange) > dst)
    {
        dst = std::get<1>(limitRange);
        dstDiff = dst - std::get<0>(newRange);
    }

    //new src
    long src = std::get<1>(newRange);
    if (dstDiff != 0)
    {
        src += dstDiff;
    }

    //new length
    long length = std::get<2>(newRange);
    if (dst + length > std::get<1>(limitRange) + std::get<2>(limitRange) - 1)
    {
        length = std::get<1>(limitRange) + std::get<2>(limitRange) - dst;
    }

    return std::make_tuple(dst, src, length);
}

std::pair<long, long> limitRange(long start, long length, const Range& range)
{
    std::pair<long, long> result;

    if (start >= std::get<1>(range))
    {
        result.first = start;
    }
    else
    {
        result.first = std::get<1>(range);
    }

    if (start + length - 1 <= std::get<1>(range) + std::get<2>(range) - 1)
    {
        result.second = length;
    }
    else
    {
        result.second = std::get<2>(range);
    }

    return result;
}

long Map::getMappedValue(long value) const
{
    long result = value;
    for (const Range& range : m_ranges)
    {
        if (isInRange(value, range))
        {
            long diff = value - std::get<1>(range);
            return std::get<0>(range) + diff;
        }
    }

    return result;
}

Almanac::Almanac(const std::string &input)
{
    std::vector<std::string> lines = split(input, '\n');
    std::string seedLine = lines.at(0);
    std::vector<std::string> seedParts = split(split(seedLine, ':').at(1), ' ');
    for (const std::string& seed : seedParts)
    {
        m_seedLocation[std::stol(seed)] = 0;
    }

    for (int i = 0; i < seedParts.size(); i+= 2)
    {
        long start = std::stol(seedParts[i]);
        long length = std::stol(seedParts[i + 1]);

        m_seedRange.push_back(std::make_pair(start, length));
    }

    Map map;
    for (int i = 1; i < lines.size(); ++i)
    {
        std::string line = lines.at(i);
        if (!std::isdigit(line.at(0)))
        {
            if (!map.m_ranges.empty())
            {
                m_maps.push_back(map);
            }
            map = Map();
        }
        else if(std::isdigit(line.at(0)))
        {
            std::vector<std::string> lineParts = split(line, ' ');
            long dst = std::stol(lineParts[0]);
            long src = std::stol(lineParts[1]);
            long length = std::stol(lineParts[2]);
            Range range = std::make_tuple(dst, src, length);

            map.m_ranges.push_back(range);
        }
    }

    if (!map.m_ranges.empty())
    {
        m_maps.push_back(map);
    }
}

long Almanac::srcToDst(long value, long map)
{
    long dst = m_maps[map].getMappedValue(value);
    map++;
    if (map >= m_maps.size())
    {
        return dst;
    }
    else
    {
        return srcToDst(dst, map);
    }
}

long Almanac::srcToDst(long value, long index, const std::vector<Map>& map)
{
    long dst = map[index].getMappedValue(value);
    index++;
    if (index >= map.size())
    {
        return dst;
    }
    else
    {
        return srcToDst(dst, index, map);
    }
}

void Almanac::findLocations()
{
    for (std::pair<long, long> pair : m_seedLocation)
    {
        m_seedLocation[pair.first] = srcToDst(pair.first, 0);
    }
}

long Almanac::findLowestLocation()
{
    findLocations();
    long result = INT32_MAX;
    for (std::pair<long, long> pair : m_seedLocation)
    {
        if (result > pair.second)
        {
            result = pair.second;
        }
    }

    return result;
}

Range Almanac::lowestRange(int map)
{
    Map locationMap = m_maps.at(map);
    long lowest = INT32_MAX;
    Range lowestRange;
    for (Range range : locationMap.m_ranges)
    {
        long dst = std::get<0>(range);
        if (lowest > dst)
        {
            lowest = dst;
            lowestRange = range;
        }
    }

    return lowestRange;
}

std::vector<Range> Almanac::dstToSrc(int map, Range range)
{
    std::vector<Range> ranges;
    Map locationMap = m_maps.at(map);

    for(Range dst : locationMap.m_ranges)
    {
        if (isInRange(range, dst))
        {
            ranges.push_back(dst);
        }
    }

    if (ranges.empty())
    {
        long dst = std::get<1>(range);
        long src = std::get<1>(range);
        long length = std::get<2>(range);
        ranges.push_back(std::make_tuple(dst, src, length));
    }

    return ranges;
}

long Almanac::findLowestLocationInRange()
{
    Range lowRange = lowestRange(m_maps.size() - 1);

    std::vector<Range> newRanges;
    std::vector<Range> currentRanges;
    currentRanges.push_back(lowRange);

    std::vector<Map> maps;
    Map lowestMap = {currentRanges};
    maps.push_back(lowestMap);

    for (int i = 2; i <= m_maps.size(); ++i)
    {
        Map map;
        for (Range range : currentRanges)
        {
            std::vector<Range> ranges = dstToSrc(m_maps.size() - i, range);

            for (Range newRange : ranges)
            {
                newRange = limitRange(newRange, range);
                if (std::find(newRanges.begin(),  newRanges.end(), newRange) == newRanges.end())
                {
                    newRanges.push_back(newRange);
                }
            }
        }
        map.m_ranges = newRanges;
        maps.push_back(map);
        currentRanges.clear();
        currentRanges = newRanges;
        newRanges.clear();
    }

    //seeds in range
    std::vector<std::pair<long, long>> seedInRange;
    for (std::pair<long, long> pair : m_seedRange)
    {
        for (Range range : currentRanges)
        {
            std::pair<long, long> limitPair = limitRange(pair.first, pair.second, range);
            if (isInRange(pair.first, pair.second, range) && std::find(seedInRange.begin(),  seedInRange.end(), limitPair) == seedInRange.end())
            {
                seedInRange.push_back(limitPair);
            }
        }
    }

    std::reverse(maps.begin(), maps.end());
    long result = INT32_MAX;

    START_TIMER(point_2)
    for (std::pair<long, long> pair : seedInRange)
    {
        for (long i = pair.first; i < pair.first + pair.second; ++i)
        {
            long seedLocation = srcToDst(pair.first, 0, maps);
            if (result > seedLocation)
            {
                result = seedLocation;
            }
        }
    }
    STOP_TIMER(point_2)

    return result;
}
