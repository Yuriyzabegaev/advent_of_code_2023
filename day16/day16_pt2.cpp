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
#include <variant>
#include <list>
#include <queue>
using namespace std;

using Position = pair<int, int>;

enum Direction : char
{
    Top = '^',
    Bottom = 'v',
    Left = '<',
    Right = '>'
};

struct Ray
{
    Direction dir;
    Position position;

    Ray(Direction dir, Position position) : dir(dir), position(position) {}
};

struct CellData
{
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};

namespace std
{
    template <>
    struct hash<Position>
    {
        size_t operator()(const Position &p) const
        {
            return ((size_t)p.first) << 32 | ((size_t)p.second);
        }
    };
}

void rotateOrSplitRay(char sym, Ray &ray, queue<Ray> &rays)
{
    if (sym == '/')
    {
        if (ray.dir == Top)
        {
            ray.dir = Right;
        }
        else if (ray.dir == Bottom)
        {
            ray.dir = Left;
        }
        else if (ray.dir == Left)
        {
            ray.dir = Bottom;
        }
        else if (ray.dir == Right)
        {
            ray.dir = Top;
        }
        else
        {
            assert(false);
        }
    }
    else if (sym == '\\')
    {
        if (ray.dir == Top)
        {
            ray.dir = Left;
        }
        else if (ray.dir == Bottom)
        {
            ray.dir = Right;
        }
        else if (ray.dir == Left)
        {
            ray.dir = Top;
        }
        else if (ray.dir == Right)
        {
            ray.dir = Bottom;
        }
        else
        {
            assert(false);
        }
    }
    else if (sym == '|' && (ray.dir == Left || ray.dir == Right))
    {
        ray.dir = Top;
        rays.push(Ray(Bottom, {ray.position.first + 1, ray.position.second}));
    }
    else if (sym == '-' && (ray.dir == Bottom || ray.dir == Top))
    {
        ray.dir = Left;
        rays.push(Ray(Right, {ray.position.first, ray.position.second + 1}));
    }
}

void printRoom(const vector<string> &room, Ray &ray)
{
    for (int i = 0; i < room.size(); ++i)
    {
        const auto &line = room[i];
        for (int ii = 0; ii < line.size(); ++ii)
        {
            if (i == ray.position.first && ii == ray.position.second)
            {
                cout << ray.dir;
            }
            else
            {
                cout << line[ii];
            }
        }
        cout << endl;
    }
    cout << endl;
}

long countEnergizedCells(const vector<string> &room)
{
    unordered_map<Position, CellData> cellsData;
    vector<string> energizedRoom(room.size());
    for (int i = 0; i < room.size(); ++i)
    {
        const string &line = room[i];
        energizedRoom[i] = string(line.size(), '.');
        for (int ii = 0; ii < line.size(); ++ii)
        {
            if (line[ii] != '.')
            {
                cellsData.insert_or_assign(Position(i, ii), CellData());
            }
        }
    }

    vector<Ray> candidatePositions;
    candidatePositions.reserve((room.size() + room[0].size()) * 2);
    for (int i = 0; i < room.size(); ++i)
    {
        candidatePositions.push_back(Ray(Right, {i, 0}));
        candidatePositions.push_back(Ray(Left, {i, room[0].size() - 1}));
    }
    for (int ii = 0; ii < room[0].size(); ++ii)
    {
        candidatePositions.push_back(Ray(Bottom, {0, ii}));
        candidatePositions.push_back(Ray(Top, {room.size() - 1, ii}));
    }

    long maxEnergized = 0;
    Ray rayMaxEnergized(Left, {-1, -1});

    for (Ray initialRay : candidatePositions)
    {

        queue<Ray> rays;
        rays.push(initialRay);
        for (auto &line : energizedRoom) {
            for (auto &sym : line) {
                sym = '.';
            }
        }
        for (auto &cellData : cellsData) {
            cellData.second.down = false;
            cellData.second.left = false;
            cellData.second.up = false;
            cellData.second.right = false;
        }
        while (!rays.empty())
        {
            Ray ray = move(rays.front());
            rays.pop();
            while (true)
            {
                if (ray.position.first < 0 || ray.position.first >= room.size())
                {
                    break;
                }
                if (ray.position.second < 0 || ray.position.second >= room[0].size())
                {
                    break;
                }
                auto sym = room[ray.position.first][ray.position.second];
                energizedRoom[ray.position.first][ray.position.second] = '#';
                // printRoom(energizedRoom, ray);

                if (sym != '.')
                {
                    auto cellDataIter = cellsData.find(ray.position);
                    assert(cellDataIter != cellsData.end());
                    CellData &myCellData = cellDataIter->second;
                    if (ray.dir == Left)
                    {
                        if (myCellData.left)
                            break;
                        else
                            myCellData.left = true;
                    }
                    else if (ray.dir == Right)
                    {
                        if (myCellData.right)
                            break;
                        else
                            myCellData.right = true;
                    }
                    else if (ray.dir == Top)
                    {
                        if (myCellData.up)
                            break;
                        else
                            myCellData.up = true;
                    }
                    else if (ray.dir == Bottom)
                    {
                        if (myCellData.down)
                            break;
                        else
                            myCellData.down = true;
                    }

                    rotateOrSplitRay(sym, ray, rays);
                }

                if (ray.dir == Top)
                {
                    ray.position.first -= 1;
                }
                else if (ray.dir == Bottom)
                {
                    ray.position.first += 1;
                }
                else if (ray.dir == Right)
                {
                    ray.position.second += 1;
                }
                else if (ray.dir == Left)
                {
                    ray.position.second -= 1;
                }
            }
        }
        long countEnergized = 0;
        for (const auto &line : energizedRoom)
        {
            for (const auto &sym : line)
            {
                if (sym == '#')
                    countEnergized++;
            }
        }

        if (countEnergized > maxEnergized) {
            maxEnergized = countEnergized;
            rayMaxEnergized = initialRay;
        }
    }
    return maxEnergized;
}

int main()
{
    string filename = "input.txt";
    ifstream inputFile(filename);
    assert(inputFile.is_open());

    vector<string> inputData;
    string inputLine;
    while (getline(inputFile, inputLine))
    {
        inputData.push_back(inputLine);
    }
    inputFile.close();

    long result = countEnergizedCells(inputData);

    cout << "result " << result << endl;

    return 0;
}
