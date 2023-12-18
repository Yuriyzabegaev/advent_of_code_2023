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
#include <variant>
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

struct Lens
{
    string label;
    long focalLength;

    Lens(const string label, long focalLength) : label(label), focalLength(focalLength) {}
    Lens(const Lens &lens) : label(lens.label), focalLength(lens.focalLength) {}
};

struct Storage
{
    array<list<Lens>, 256> data;

    void insert(Lens lens)
    {
        size_t bin = hashString(lens.label);
        list<Lens> &box = data[bin];
        auto lensFound = find_if(box.begin(), box.end(), [lens](const Lens &a)
                                 { return a.label == lens.label; });
        if (lensFound != box.end())
        {
            *lensFound = lens;
        }
        else
        {
            box.push_back(lens);
        }
    }

    void remove(const string &label)
    {
        size_t bin = hashString(label);
        list<Lens> &box = data[bin];
        auto lensFound = find_if(box.begin(), box.end(), [label](const Lens &a)
                                 { return a.label == label; });
        if (lensFound != box.end())
        {
            box.erase(lensFound);
        }
    }

    long countTotalFocusingPower() const {
        long result = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            const auto &box = data[i];
            size_t ii = 0;
            for (const auto &lens : box) {
                ii += 1;
                result += (i + 1) * ii * lens.focalLength;
            }
        }
        return result;
    }
};

variant<Lens, string> parseEntry(const string &input)
{
    string label;
    string tmp;
    int i = 0;
    bool deleteSymbol = false;
    bool equalSymbol = false;
    for (i = 0; i < input.size(); ++i)
    {
        char sym = input[i];
        if (sym == '=')
        {
            ++i;
            equalSymbol = true;
            break;
        }
        if (sym == '-')
        {
            deleteSymbol = true;
            break;
        }
        label.push_back(sym);
    }
    assert(deleteSymbol || equalSymbol);
    if (deleteSymbol)
    {
        return label;
    }

    string focalLengthStr;
    for (; i < input.size(); ++i)
    {
        focalLengthStr.push_back(input[i]);
    }
    long focalLength = stol(focalLengthStr);
    return Lens(label, focalLength);
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
    if (tmp != "")
    {
        inputData.push_back(tmp);
    }
    assert(!getline(inputFile, tmp));
    inputFile.close();

    Storage storage;

    long total = 0;
    for (const auto &entry : inputData)
    {
        auto lensOrRemoveLabel = parseEntry(entry);
        if (holds_alternative<Lens>(lensOrRemoveLabel))
        {
            Lens &value = get<Lens>(lensOrRemoveLabel);
            storage.insert(value);
        }
        else
        {
            string &removeLabel = get<string>(lensOrRemoveLabel);
            storage.remove(removeLabel);
        }
    }

    cout << "result " << storage.countTotalFocusingPower() << endl;

    return 0;
}
