#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

regex pattern("(\\d+)");

long parse_input(string &line) {
    smatch match;
    long result;
    line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
    assert(regex_search(line, match, pattern));
    return stol(match[1].str());
}

long computeDistance(long totalTime, long holdTime) {
    long moveTime = totalTime - holdTime;
    long velosity = holdTime;
    return moveTime * velosity;
}

long countDistancesBetter(long totalTime, long baselineDistance) {
    long score = 0;

    for (long holdTime = 1; holdTime < totalTime; ++holdTime) {
        long distance = computeDistance(totalTime, holdTime);
        if (distance > baselineDistance) {
            score++;
        }
    }
    
    return score;
}

int main()
{
    string filename = "input.txt";

    ifstream inputFile(filename);
    assert(inputFile.is_open());
    string line;
    smatch match;
    assert(getline(inputFile, line));
    long totalTime = parse_input(line);
    assert(getline(inputFile, line));
    long baselineDistance = parse_input(line);
    inputFile.close();

    long res = countDistancesBetter(totalTime, baselineDistance);

    cout << "Score: " << res << endl;

    return 0;
}
