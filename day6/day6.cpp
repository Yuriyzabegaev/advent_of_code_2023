#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <regex>
// #include <omp.h>
#include <algorithm>
using namespace std;

regex pattern("(\\d+)");

vector<int> parse_input(string &line) {
    smatch match;
    vector<int> result;
    while(regex_search(line, match, pattern)) {
        result.push_back(stoi(match[1].str()));
        line = match.suffix().str();
    }
    return result;
    
}

int computeDistance(int totalTime, int holdTime) {
    int moveTime = totalTime - holdTime;
    int velosity = holdTime;
    return moveTime * velosity;
}

int countDistancesBetter(int totalTime, int baselineDistance) {
    int score = 0;
    for (int holdTime = 1; holdTime < totalTime; ++holdTime) {
        int distance = computeDistance(totalTime, holdTime);
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
    vector<int> times = parse_input(line);
    assert(getline(inputFile, line));
    vector<int> distances = parse_input(line);
    inputFile.close();

    int totalScore = 1;
    for (int i = 0; i < times.size(); ++i) {
        int res = countDistancesBetter(times.at(i), distances.at(i));
        totalScore *= res;
    }

    cout << "Total score: " << totalScore << endl;

    return 0;
}
