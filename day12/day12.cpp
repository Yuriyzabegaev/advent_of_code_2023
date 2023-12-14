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

long countPermutations(string line, vector<long>::iterator springsCountBegin, vector<long>::iterator springsCountEnd, string currentSolution, int oldSolutionOffset, const string &fullLine)
{
    if (springsCountBegin == springsCountEnd)
    {
        // cout << currentSolution << endl;
        

        for (auto c : currentSolution)
        {
            if (c == '#') {
                return 0;
            }
            // assert(c != '#');
        }
        return 1;
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
        long linePermutations = countPermutations(currentSolution.substr(matchEnd, currentSolution.size()), springsCountBegin, springsCountEnd, currentSolution, matchEnd, fullLine);
        permutations += linePermutations;
    }
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
            springsMap.push_back(match[0].str());

            vector<long> springsInLine;
            line = match.suffix().str();
            while (regex_search(line, match, pattern))
            {
                springsInLine.push_back(stol(match[0].str()));
                line = match.suffix().str();
            }
            springsCount.push_back(springsInLine);
        }
    }

    long count = 0;
    for (int i = 0; i < springsMap.size(); ++i)
    {
        auto res = countPermutations(springsMap[i], springsCount[i].begin(), springsCount[i].end(), springsMap[i], 0, springsMap[i]);
        cout << springsMap[i] << " " << res << endl;
        count += res;

        assert(res > 0);
    }
    cout << "total " << count << endl;

    return 0;
}
