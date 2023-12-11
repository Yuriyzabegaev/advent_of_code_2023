#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <regex>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <thread>
#include <list>
using namespace std;

struct UniversePoint
{
    long xDist;
    long yDist;

    UniversePoint(long xDist, long yDist) : xDist(xDist), yDist(yDist) {}
};

long findDIstanceBetweenGalaxies(const vector<vector<UniversePoint>> &space, const pair<long, long> &src, const pair<long, long> &dst)
{
    long result = 0;
    int minY = src.first < dst.first ? src.first : dst.first;
    int maxY = src.first > dst.first ? src.first : dst.first;
    int minX = src.second < dst.second ? src.second : dst.second;
    int maxX = src.second > dst.second ? src.second : dst.second;
    for (int i = minY + 1; i <= maxY; i++)
    {
        result += space[i][minX].yDist;
    }
    for (int j = minX + 1; j <= maxX; j++)
    {
        result += space[maxY][j].xDist;
    }
    return result;
}

int main()
{
    string filename = "input.txt";
    ifstream inputFile(filename);
    assert(inputFile.is_open());

    string line;
    smatch match;

    vector<vector<UniversePoint>> space;
    vector<pair<long, long>> galaxies;

    for (int i = 0; getline(inputFile, line); i++)
    {
        vector<UniversePoint> tmp;
        for (int j = 0; j < line.size(); j++)
        {
            tmp.push_back({1, 1});
            if (line[j] == '#')
            {
                galaxies.push_back({i, j});
            }
        }
        space.push_back(tmp);
    }

    unordered_set<int> rowsWithoutGalaxies;
    unordered_set<int> colsWithoutGalaxies;
    for (int i = 0; i < space.size(); ++i)
        rowsWithoutGalaxies.insert(i);
    for (int j = 0; j < space[0].size(); j++)
        colsWithoutGalaxies.insert(j);

    for (auto gal : galaxies)
    {
        rowsWithoutGalaxies.erase(gal.first);
        colsWithoutGalaxies.erase(gal.second);
    }

    for (auto &line : space) {
        for (auto col : colsWithoutGalaxies) {
            line[col].xDist = 1000000;
        }
    }

    for (auto row: rowsWithoutGalaxies) {
        for (int j = 0; j < space[row].size(); j++) {
            space[row][j].yDist = 1000000;
        }
    }

    long sumDistances = 0;
    for (int i = 0; i < galaxies.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            sumDistances += findDIstanceBetweenGalaxies(space, galaxies[j], galaxies[i]);
        }
    }

    cout << sumDistances << endl;

    return 0;
}
