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


Dish tilt(const Dish &src, const int xMax, const int yMax, const Direction dir)
{
    Dish result;

    vector<Point> keys;
    keys.reserve(src.size());
    for (const auto &entry : src)
        keys.push_back(entry.first);

    if (dir == north) {
        sort(keys.begin(), keys.end(), [](const Point &left, const Point &right)
                { return left.y < right.y; });
    } else if (dir == south) {
        sort(keys.begin(), keys.end(), [](const Point &left, const Point &right)
                { return left.y > right.y; });
    } else if (dir == west) {
        sort(keys.begin(), keys.end(), [](const Point &left, const Point &right)
                { return left.x < right.x; });
    } else if (dir == east) {
        sort(keys.begin(), keys.end(), [](const Point &left, const Point &right)
                { return left.x > right.x; });
    } else {
        assert(false);
    }

    for (const auto &point : keys)
    {
        char sym = src.at(point);
        if (sym == '#')
        {
            result.insert_or_assign(point, '#');
        }
        else if (sym == 'O')
        {
            int yNew = -1;
            for (yNew = point.y - 1; yNew >= 0; --yNew)
            {
                char encountered = takeFromMap(result, {point.x, yNew}, '.');
                if (encountered != '.')
                {
                    break;
                }
            }
            result.insert_or_assign({point.x, yNew + 1}, 'O');
        }
        // printDish(src, point, xMax, yMax);
        // printDish(result, point, xMax, yMax);
        // cout << "";
    }
    return result;
}

int main()
{
    string filename = "input.txt";
    ifstream inputFile(filename);
    assert(inputFile.is_open());

    string line;
    Dish dish;
    int i = 0;
    int j = 0;
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

    Dish newDish = tilt(dish, xMax, yMax, north);
    long count = 0;
    for (const auto &entry : newDish)
    {
        if (entry.second == 'O')
        {
            count += yMax - entry.first.y;
        }
    }

    cout << "total " << count << endl;

    return 0;
}
