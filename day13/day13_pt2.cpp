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

void printPattern(const vector<string> &pattern)
{
    for (const auto &line : pattern)
    {
        cout << line << endl;
    }
    cout << endl;
}

bool checkMirror(const vector<string> &pattern, size_t mirrorIdx)
{
    size_t down = mirrorIdx;
    for (size_t up = mirrorIdx; up < pattern.size(); ++up)
    {
        down -= 1;
        if (down == -1)
        {
            break;
        }
        if (pattern[up] != pattern[down])
        {
            return false;
        }
    }
    return true;
}

vector<string> transposePattern(const vector<string> &pattern)
{
    vector<string> result(pattern.at(0).size());
    for (int i = 0; i < pattern.size(); ++i)
    {
        for (int j = 0; j < pattern[i].size(); ++j)
        {
            result[j].push_back(pattern[i][j]);
        }
    }
    return result;
}

long proceedPattern(const vector<string> &pattern, size_t skip = 0)
{
    size_t i = -1;
    for (i = 1; i < pattern.size(); ++i)
    {
        if (i == skip) {
            continue;
        }
        if (checkMirror(pattern, i))
        {
            return i;
        }
    }
    return 0;
}

bool checkSmudge(long smudge, long correct)
{
    if (smudge == 0)
    {
        return false;
    }
    return correct != smudge;
}

long proceedSmudgePattern(const vector<string> &pattern)
{
    long result = 0;
    auto correctH = proceedPattern(pattern);
    auto correctV = proceedPattern(transposePattern(pattern));

    long smudgeH = 0;
    long smudgeV = 0;
    for (size_t i = 0; i < pattern.size(); ++i)
    {
        vector<string> smudgePattern = pattern;
        for (size_t j = 0; j < pattern[i].size(); ++j)
        {
            smudgePattern[i] = pattern[i];
            smudgePattern[i][j] = smudgePattern[i][j] == '#' ? '.' : '#';
            // printPattern(smudgePattern);
            smudgeH = proceedPattern(smudgePattern, correctH);
            smudgeV = proceedPattern(transposePattern(smudgePattern), correctV);

            if (checkSmudge(smudgeH, correctH)) {
                result += 100 * smudgeH;
            }
            if (checkSmudge(smudgeV, correctV))
            {
                result += smudgeV;
            }
            if (result != 0)
            {
                return result;
            }
        }
    }
    printPattern(pattern);
    assert(false);
}

int main()
{
    string filename = "input.txt";
    ifstream inputFile(filename);
    assert(inputFile.is_open());

    string line;

    vector<vector<string>> patterns;
    vector<string> pattern;

    while (getline(inputFile, line))
    {
        if (line == "")
        {
            patterns.push_back(pattern);
            pattern.clear();
        }
        else
        {
            pattern.push_back(line);
        }
    }
    if (line != "")
    {
        patterns.push_back(pattern);
    }

    long count = 0;
    for (const auto &pattern : patterns)
    {
        count += proceedSmudgePattern(pattern);
    }

    cout << "total " << count << endl;

    return 0;
}
