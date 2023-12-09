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

regex pattern("-?\\d+");

void buildDifferenceSequence(const vector<long> &input, vector<vector<long>> &output) {
    vector<long> result(input.size() - 1);
    long prev = input.at(0);
    bool quit = true;
    for (int i = 1; i < input.size(); ++i) {
        long current = input[i];
        long res = current - prev;
        prev = current;
        if (res != 0) {
            quit = false;
        }
        result[i - 1] = res;
    }
    output.push_back(result);

    if (!quit) {
        buildDifferenceSequence(result, output);
    }
}

long extrapolate(const vector<long> &input) {
    vector<vector<long>> output;
    output.push_back(input);
    buildDifferenceSequence(input, output);

    long diff = 0;
    for (int i = output.size() - 2; i >= 0; i--) {
        auto line = output[i];
        auto last = line[0];
        diff = last - diff;
    }
    return diff;
}

int main()
{
    string filename = "input.txt";

    ifstream inputFile(filename);
    assert(inputFile.is_open());

    string line;
    smatch match;
    vector<vector<long>> inputData;
    while (getline(inputFile, line))
    {
        vector<long> values;
        while (regex_search(line, match, pattern)) {
            values.push_back(stol(match.str()));
            line = match.suffix().str();
        }
        inputData.push_back(values);
    }

    long sum = 0;
    for (auto sequence : inputData) {
        sum += extrapolate(sequence);
    }
    cout << sum << endl;
    return 0;
}
