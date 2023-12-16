#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <regex>
#include <array>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <list>
using namespace std;

struct Point
{
    long x;
    long y;

    Point(long x, long y) : x(x), y(y) {}
    Point(const Point &other) : x(other.x), y(other.y) {}

    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
};

namespace std
{
    template <>
    struct hash<Point>
    {
        size_t operator()(const Point &p) const
        {
            return hash<long>()(p.x) ^ hash<long>()(p.y);
        }
    };
}

template <typename K, typename V>
V takeFromMap(const unordered_map<K, V> &data, const K &key, V defaultVal)
{
    auto res = data.find(key);
    if (res != data.end())
    {
        return res->second;
    }
    return defaultVal;
}

using Dish = unordered_map<Point, char>;

void printDish(const Dish &dish, const Point &marker, const int xMax, const int yMax)
{
    for (int i = 0; i < yMax; ++i)
    {
        string tmp;
        for (int j = 0; j < xMax; ++j)
        {
            if (marker.x == j && marker.y == i)
            {
                tmp.push_back('x');
            }
            else
            {
                tmp.push_back(takeFromMap(dish, {j, i}, '.'));
            }
        }
        cout << tmp << endl;
    }
    cout << endl;
}

enum Direction
{
    west,
    east,
    north,
    south,
};

vector<Point> sortKeys(const Dish &src, const Direction dir, const int fixedAxis)
{
    vector<Point> keys;
    keys.reserve(src.size());
    bool vertical = dir == north || dir == south;
    bool horizontal = dir == west || dir == east;

    for (const auto &entry : src)
    {
        if (vertical && fixedAxis == entry.first.x)
        {
            keys.push_back(entry.first);
        }
        else if (horizontal && fixedAxis == entry.first.y)
        {
            keys.push_back(entry.first);
        }
    }

    if (dir == north)
    {
        sort(keys.begin(), keys.end(), [](const Point &left, const Point &right)
             { return left.y < right.y; });
    }
    else if (dir == south)
    {
        sort(keys.begin(), keys.end(), [](const Point &left, const Point &right)
             { return left.y > right.y; });
    }
    else if (dir == west)
    {
        sort(keys.begin(), keys.end(), [](const Point &left, const Point &right)
             { return left.x < right.x; });
    }
    else if (dir == east)
    {
        sort(keys.begin(), keys.end(), [](const Point &left, const Point &right)
             { return left.x > right.x; });
    }
    else
    {
        assert(false);
    }
    return keys;
}

Point shiftForward(const Point &src, const Direction dir)
{
    switch (dir)
    {
    case north:
        return Point(src.x, src.y + 1);
    case south:
        return Point(src.x, src.y - 1);
    case west:
        return Point(src.x + 1, src.y);
    case east:
        return Point(src.x - 1, src.y);
    default:
        assert(false);
    }
}

Point shiftToEnd(const Point &src, const Direction dir, int xMax, int yMax)
{
    switch (dir)
    {
    case north:
        return Point(src.x, 0);
    case south:
        return Point(src.x, yMax - 1);
    case west:
        return Point(0, src.y);
    case east:
        return Point(xMax - 1, src.y);
    default:
        assert(false);
    }
}

Dish tilt(const Dish &src, const int xMax, const int yMax, const Direction dir)
{
    Dish result;
    int slowMax = (dir == north || dir == south) ? xMax : yMax;

    for (int slowIdx = 0; slowIdx < slowMax; ++slowIdx)
    {
        auto keys = sortKeys(src, dir, slowIdx);
        for (int keyIdx = 0; keyIdx < keys.size(); ++keyIdx)
        {
            const Point point = keys[keyIdx];
            char sym = src.at(point);
            if (sym == '#')
            {
                result.insert_or_assign(point, '#');
            }
            else if (sym == 'O')
            {
                Point newPosition(-1, -1);
                if (keyIdx == 0)
                {
                    newPosition = shiftToEnd(point, dir, xMax, yMax);
                }
                else
                {
                    newPosition = shiftForward(keys[keyIdx - 1], dir);
                }
                keys[keyIdx] = newPosition;
                result.insert(make_pair(newPosition, 'O'));
            }
            // printDish(src, point, xMax, yMax);
            // printDish(result, point, xMax, yMax);
            // cout << "";
        }
    }

    // printDish(src, {-1, -1}, xMax, yMax);
    // printDish(result, {-1, -1}, xMax, yMax);
    return result;
}

struct PointMapHash
{
    std::size_t operator()(const std::unordered_map<Point, char> &pointMap) const
    {
        std::size_t h = 0;

        for (const auto &pair : pointMap)
        {
            const Point &key = pair.first;
            char value = pair.second;

            h ^= (hash<Point>()(key) + std::hash<char>()(value) + 0x9e3779b9 + (h << 6) + (h >> 2));
        }

        return h;
    }
};

long countAnswer(const Dish &dish, long yMax)
{
    long count = 0;
    for (const auto &entry : dish)
    {
        if (entry.second == 'O')
        {
            count += yMax - entry.first.y;
        }
    }
    return count;
}

int main()
{
    string filename = "input.txt";
    ifstream inputFile(filename);
    assert(inputFile.is_open());

    string line;
    Dish dish;
    long i = 0;
    long j = 0;
    for (i = 0; getline(inputFile, line); ++i)
    {
        for (j = 0; j < line.size(); ++j)
        {
            auto sym = line[j];
            if (sym == '#' || sym == 'O')
            {
                dish.insert(make_pair(Point(j, i), sym));
            }
        }
    }
    int xMax = j;
    int yMax = i;

    unordered_map<Dish, long, PointMapHash> memory;

    long cycleIterations = -1;
    long TOTAL_REPEATS = 1000000000;
    for (i = 0; i < TOTAL_REPEATS; ++i)
    {
        // printDish(dish, {-1, -1}, xMax, yMax);
        dish = tilt(dish, xMax, yMax, north);
        dish = tilt(dish, xMax, yMax, west);
        dish = tilt(dish, xMax, yMax, south);
        dish = tilt(dish, xMax, yMax, east);
        // if (((i + 1) % (1000000000 / 100000)) == 0)

        auto result = memory.find(dish);
        if (result != memory.end())
        {
            cycleIterations = i - result->second;
            i += 1;
            break;
        }
        memory.insert(pair(dish, i));
    }

    long repeats = (TOTAL_REPEATS - i) % cycleIterations;

    for (i = 0; i < repeats; ++i)
    {
        dish = tilt(dish, xMax, yMax, north);
        dish = tilt(dish, xMax, yMax, west);
        dish = tilt(dish, xMax, yMax, south);
        dish = tilt(dish, xMax, yMax, east);
    }

    cout << "total " << countAnswer(dish, yMax) << endl;

    return 0;
}
