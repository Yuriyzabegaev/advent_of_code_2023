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


bool checkMirror(const vector<string> &pattern, size_t mirrorIdx) {
    size_t down = mirrorIdx;
    for (size_t up = mirrorIdx; up < pattern.size(); ++up) {
        down -= 1;
        if (down == -1) {
            break;
        }
        if (pattern[up] != pattern[down]) {
            return false;
        }
    }
    return true;
}

vector<string> transposePattern(const vector<string> &pattern) {
    vector<string> result(pattern.at(0).size());
    for (int i = 0; i < pattern.size(); ++i) {
        for (int j = 0; j < pattern[i].size(); ++j) {
            result[j].push_back(pattern[i][j]);
        }
    }
    return result;
}


long proceedPattern(const vector<string> &pattern) {
    size_t i = -1;
    for (i = 1; i < pattern.size(); ++i) {
        if (checkMirror(pattern, i)) {
            return i;
        }
    }
    return 0;
}

long proceedSmudgePattern(const vector<string> &pattern) {
    size_t i = -1;
    for (i = 1; i < pattern.size(); ++i) {
        if (checkMirror(pattern, i)) {
            return i;
        }
    }
    return 0;
}

int main()
{
    string filename = "test_input.txt";
    ifstream inputFile(filename);
    assert(inputFile.is_open());

    string line;

    vector<vector<string>> patterns;
    vector<string> pattern;
    
    while (getline(inputFile, line))
    {
        if (line == "") {
            patterns.push_back(pattern);
            pattern.clear();
        } else {
            pattern.push_back(line);
        }
    }
    if (line != "") {
        patterns.push_back(pattern);
    }

    long count = 0;
    for (const auto &pattern : patterns) {
        auto correctHorizontalLocation = proceedPattern(pattern);
        auto correctVerticalLocation = proceedPattern(transposePattern(pattern));
        count += 100 * correctHorizontalLocation;
        count += correctVerticalLocation;
    }

    cout << "total " << count << endl;

    return 0;
}
