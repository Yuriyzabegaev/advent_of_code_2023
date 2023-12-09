#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <regex>
#include <array>
#include <unordered_map>
#include <algorithm>
using namespace std;

regex mapPattern("^(\\w+) = \\((\\w+), (\\w+)\\)");

int main()
{
    string filename = "input.txt";
    string directions;

    ifstream inputFile(filename);
    assert(inputFile.is_open());

    assert(getline(inputFile, directions));

    unordered_map<string, pair<string, string>> routeMap;
    string line;
    getline(inputFile, line);
    while (getline(inputFile, line)) {
        smatch match;
        assert(regex_search(line, match, mapPattern));
        auto location = match[1].str();
        auto left = match[2].str();
        auto right = match[3].str();
        routeMap[location] = make_pair(left, right);
    }

    string currentLocation = "AAA";
    long numSteps = 0; 
    while (currentLocation != "ZZZ") {
        for (auto direction : directions) {
            auto localMap = routeMap[currentLocation];
            if (direction == 'L') {
                currentLocation = localMap.first;
            } else {
                currentLocation = localMap.second;
            }
        }
        numSteps += directions.size();
        
    }

    cout << numSteps << endl;

    return 0;
}
