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

struct TravelData
{
    string directions;
    unordered_map<string, pair<string, string>> routeMap;
 
    unordered_map<string, string> cachedTransformation;
    unordered_map<string, vector<int>> cachedTransformationsToWin;
};

int countCompletion(const vector<string> &currentLocations)
{
    int res = 0;
    for (auto loc : currentLocations)
    {
        if (loc.at(2) == 'Z')
        {
            res += 1;
        }
    }
    return res;
}

bool checkWin(const string &location)
{
    return location.at(2) == 'Z';
}

string makeStep(const char direction, const pair<string, string> &localMap)
{
    if (direction == 'L')
    {
        return localMap.first;
    }
    else
    {
        return localMap.second;
    }
}

vector<int> vectorFromMultimap(const unordered_multimap<string, int> &data, const string &searched)
{
    vector<int> result;
    auto iter = data.equal_range(searched);
    for (auto it = iter.first; it != iter.second; ++it)
    {
        result.push_back(it->second);
    }
    return result;
}

string goForward(const string &initialLocation, TravelData &travelData)
{
    string currentLocation = initialLocation;
    auto winningStepsIter = travelData.cachedTransformationsToWin.find(initialLocation);
    if (winningStepsIter != travelData.cachedTransformationsToWin.end())
    {
        return travelData.cachedTransformation.at(initialLocation);
    }

    vector<int> stepsToWin;
    int i = 0;
    for (auto direction : travelData.directions)
    {
        currentLocation = makeStep(direction, travelData.routeMap.at(currentLocation));
        i++;
        if (checkWin(currentLocation))
        {
            stepsToWin.push_back(i);
        }
    }
    travelData.cachedTransformation[initialLocation] = currentLocation;
    travelData.cachedTransformationsToWin[initialLocation] = stepsToWin;
    return currentLocation;
}

struct TravelCycle {
    string start;
    int startIdx;
    int numSteps;
    vector<int> winSteps;
};

TravelCycle detectCycle(string currentLocation, TravelData &travelData) {
    int currentSteps = 0;
    string forwardLocation = currentLocation;
    while (true) {
        currentLocation = goForward(currentLocation, travelData);
        currentSteps += travelData.directions.size();
        
        forwardLocation = goForward(forwardLocation, travelData);
        forwardLocation = goForward(forwardLocation, travelData);

        if (forwardLocation == currentLocation) {
            break;
        }    
    }

    TravelCycle result;
    result.startIdx = currentSteps;
    result.start = currentLocation;

    currentSteps = 0;
    while(true) {
        currentLocation = goForward(currentLocation, travelData);
        currentSteps += travelData.directions.size();
        if (currentLocation == forwardLocation) {
            break;
        }
    }
    result.numSteps = currentSteps;

    for (int i = 0; i < result.numSteps; i += travelData.directions.size()) {
        auto winSteps = travelData.cachedTransformationsToWin.at(currentLocation);
        for (auto winStep : winSteps) {
            result.winSteps.push_back(winStep + i);
        }
        currentLocation = goForward(currentLocation, travelData);
    }
    return result;
}

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long lcm(long long a, long long b) {
    return (a * b) / gcd(a, b);
}

long long findLeastCommonDenominator(const vector<long long> &denominators) {
    if (denominators.size() == 1) {
        return denominators[0];
    }

    long long result = lcm(denominators[0], denominators[1]);

    for (int i = 2; i < denominators.size(); i++) {
        result = lcm(result, denominators[i]);
    }

    return result;
}

int main()
{
    string filename = "input.txt";
    string directions;

    ifstream inputFile(filename);
    assert(inputFile.is_open());

    assert(getline(inputFile, directions));

    TravelData travelData;
    travelData.directions = directions;
    vector<string> currentLocations;
    string line;
    getline(inputFile, line);
    while (getline(inputFile, line))
    {
        smatch match;
        assert(regex_search(line, match, mapPattern));
        auto location = match[1].str();
        auto left = match[2].str();
        auto right = match[3].str();
        travelData.routeMap[location] = make_pair(left, right);
        if (location.at(2) == 'A')
        {
            currentLocations.push_back(location);
        }
    }

    int stepsInThisRound = travelData.directions.size();
    bool done = false;
    auto forwardLocations = currentLocations;
    vector<long long> cycleRepeats;

    for (size_t i = 0; i < currentLocations.size(); ++i) {
        auto cycle = detectCycle(currentLocations[i], travelData);
        assert(cycle.winSteps.size() == 1);
        assert(checkWin(cycle.start));
        cycleRepeats.push_back(cycle.numSteps);
    }

    cout << findLeastCommonDenominator(cycleRepeats) << endl;

    return 0;
}
