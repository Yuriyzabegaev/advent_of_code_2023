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

regex pattern("[#?.]+|\\d+");

class MyHash
{

public:
    auto operator()(const pair<string, vector<long>> &pair) const -> size_t
    {
        size_t result = 0;
        for (auto val : pair.second)
        {
            result |= val;
            result <<= 1;
        }
        return hash<string>{}(pair.first) << 32 | result;
    }
};

long countPermutations(string line, vector<long>::iterator springsCountBegin, vector<long>::iterator springsCountEnd, string currentSolution, int oldSolutionOffset, const string &fullLine, unordered_map<pair<string, vector<long>>, long, MyHash> &memory)
{

    if (springsCountBegin == springsCountEnd)
    {
        // cout << currentSolution << endl;

        for (auto c : line)
        {
            if (c == '#')
            {
                return 0;
            }
        }
        return 1;
    }

    vector<long> remainingSprings(springsCountBegin, springsCountEnd);
    auto result = memory.find(make_pair(line, remainingSprings));
    if (result != memory.end()) {
        return result->second;
    }

    const long candidateGroup = *springsCountBegin;
    ostringstream oss;
    oss << "(?=([#?]{" << candidateGroup << "}))";
    string workingSpringsStr = oss.str();
    regex workingSprings(workingSpringsStr);

    long permutations = 0;
    ++springsCountBegin;

    string restoreCurrentSolution = currentSolution;

    sregex_iterator next(line.begin(), line.end(), workingSprings);
    sregex_iterator end;
    while (next != end)
    {
        smatch match = *(next++);

        currentSolution = restoreCurrentSolution;
        auto solutionOffset = oldSolutionOffset + match.position(1);
        auto matchEnd = solutionOffset + match.length(1);
        for (int i = solutionOffset; i < matchEnd; ++i)
            currentSolution[i] = '*';

        if (solutionOffset > 0 && (currentSolution[solutionOffset - 1] == '*' || currentSolution[solutionOffset - 1] == '#'))
        {
            continue;
        }
        if (line.size() > 0 && currentSolution[matchEnd] == '#')
        {
            continue;
        }

        bool badMiddle = false;
        for (int i = oldSolutionOffset; i < solutionOffset; ++i)
        {
            if (currentSolution[i] == '#')
            {
                badMiddle = true;
                break;
            }
        }
        if (badMiddle)
        {
            continue;
        }
        long linePermutations = countPermutations(currentSolution.substr(matchEnd, currentSolution.size()), springsCountBegin, springsCountEnd, currentSolution, matchEnd, fullLine, memory);
        permutations += linePermutations;
    }

    memory[make_pair(line, remainingSprings)] = permutations;

    return permutations;
}

int main()
{
    string filename = "input.txt";
    ifstream inputFile(filename);
    assert(inputFile.is_open());

    string line;
    smatch match;

    vector<string> springsMap;
    vector<vector<long>> springsCount;

    while (getline(inputFile, line))
    {
        if (regex_search(line, match, pattern))
        {
            string original = match[0].str();
            string fiveTimes;
            for (int i = 0; i < 5; ++i)
            {
                fiveTimes.append(original);
                if (i != 4)
                {
                    fiveTimes.append("?");
                }
            }
            springsMap.push_back(fiveTimes);

            vector<long> springsInLine;
            line = match.suffix().str();
            while (regex_search(line, match, pattern))
            {
                springsInLine.push_back(stol(match[0].str()));
                line = match.suffix().str();
            }
            vector<long> springsInLineX5;
            for (int i = 0; i < 5; ++i)
            {
                for (auto val : springsInLine)
                {
                    springsInLineX5.push_back(val);
                }
            }
            springsCount.push_back(springsInLineX5);
        }
    }

    unordered_map<pair<string, vector<long>>, long, MyHash> memory;

    long count = 0;
    for (int i = 0; i < springsMap.size(); ++i)
    {
        auto res = countPermutations(springsMap[i], springsCount[i].begin(), springsCount[i].end(), springsMap[i], 0, springsMap[i], memory);
        // cout << springsMap[i] << " " << res << endl;
        if (i % 10 == 0) {
            cout << "Cache size: " << memory.size() << endl;
        }
        count += res;

        assert(res > 0);
    }
    cout << "total " << count << endl;

    return 0;
}
