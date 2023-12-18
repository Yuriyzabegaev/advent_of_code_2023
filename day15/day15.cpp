#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <regex>
#include <array>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <list>
using namespace std;

inline long hashSymbol(char sym, long input)
{
    long result = static_cast<long>(sym);
    result += input;
    result *= 17;
    result %= 256;
    return result;
}

long hashString(const string &line)
{
    long result = 0;
    for (char sym : line)
    {
        result = hashSymbol(sym, result);
    }
    return result;
}

int main()
{
    string filename = "input.txt";
    ifstream inputFile(filename);
    assert(inputFile.is_open());

    vector<string> inputData;
    string inputLine;
    string tmp;
    assert(getline(inputFile, inputLine));
    for (char sym : inputLine)
    {
        if (sym == ',')
        {
            inputData.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp.push_back(sym);
        }
    }
    if (tmp != "") {
        inputData.push_back(tmp);
    }
    assert(!getline(inputFile, tmp));
    inputFile.close();

    long total = 0;
    for (const auto &line : inputData)
    {
        total += hashString(line);
    }

    cout << "result " << total << endl;

    return 0;
}
