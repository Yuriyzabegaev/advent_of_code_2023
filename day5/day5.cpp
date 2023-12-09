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

regex pattern("(\\d+) (\\d+)|(\n\n.+\n)");
regex transformationPattern("(\\d+) (\\d+) (\\d+)|(\n\n.+\n)");

struct Instruction
{
    long dstStart;
    long srcStart;
    long offset;

    bool operator<(const Instruction& other) const {
        return srcStart < other.srcStart;
    }
};

struct SeedOffset {
    long seedStart;
    long offset;

    bool operator<(const SeedOffset& other) const {
        return seedStart < other.seedStart;
    }
};

vector<Instruction> getInsructionBlock(string &inputData) {
    vector<Instruction> result;
    smatch match;
    while (regex_search(inputData, match, transformationPattern)) {
        if (match[0].str().substr(0, 1) == "\n")
        {
            inputData = match.suffix().str();
            break;
        }
        Instruction ins;
        ins.dstStart = stol(match[1].str());
        ins.srcStart = stol(match[2].str());
        ins.offset = stol(match[3].str());
        result.push_back(ins);
        inputData = match.suffix().str();
    }
    return result;
}

struct TransformationInterval {
    long start;
    long end;
};

long applyTransformation(const Instruction &inst, long input) {
    assert (input >= inst.srcStart && input < (inst.srcStart + inst.offset));
    return input + inst.dstStart - inst.srcStart;    
}

long applyTransformations(vector<vector<Instruction>> &transformations, long val) {
    for (auto block : transformations) {
        for (auto inst : block) {
            if (val >= inst.srcStart && (val < (inst.srcStart + inst.offset))) {
                val = applyTransformation(inst, val);
                break;
            }
        }
        // cout << val << endl;
    }
    return val;
}

TransformationInterval findInterval(vector<Instruction> &block, TransformationInterval input) {
    for (Instruction inst : block) {
        long instrEnd = inst.srcStart + inst.offset;
        long intersecStart = inst.srcStart > input.start ? inst.srcStart : input.start;
        long intersecEnd = instrEnd < input.end ? instrEnd : input.end;
        if (!(intersecStart < intersecEnd)) // No intersection
        {
            continue;
        }

        TransformationInterval res;
        if (input.start < inst.srcStart)  // If the rule starts later then the input, we're interested in unmodified input.
        {
            res.start = input.start;
            res.end = inst.srcStart;
        } else {
            res.start = applyTransformation(inst, intersecStart);
            res.end = applyTransformation(inst, intersecEnd - 1) + 1;
        }
        return res;
    }
    return input;
}

TransformationInterval findIntervalLoop(vector<vector<Instruction>> &transformations, TransformationInterval interval) {
    for (auto block : transformations) {
        interval = findInterval(block, interval);
    }
    return interval;
}

const long PRINT_INTERVAL = 1e4;

int main()
{
    string filename = "input.txt";

    ifstream inputFile(filename);
    assert(inputFile.is_open());

    string inputData((istreambuf_iterator<char>(inputFile)),
                     istreambuf_iterator<char>());

    inputFile.close();

    smatch match;

    vector<SeedOffset> seeds;
    long seedCounter = 0;

    while (regex_search(inputData, match, pattern))
    {
        if (match[0].str().substr(0, 1) == "\n")
        {
            inputData = match.suffix().str();
            break;
        }
        long initialSeed = stol(match[1].str());
        long offset = stol(match[2].str());
        SeedOffset seed;
        seed.offset = offset;
        seed.seedStart = initialSeed;
        seedCounter += offset;
        seeds.push_back(seed);
        inputData = match.suffix().str();
    }

    cout << "Total seeds: " << seedCounter << endl;

    vector<vector<Instruction>> transformations;
    vector<vector<Instruction>> reverseTransformations;
    
    while (inputData.size() != 0) {
        auto trans = getInsructionBlock(inputData);
        vector<Instruction> reverse;
        for (auto inst : trans) {
            Instruction rev;
            rev.srcStart = inst.dstStart;
            rev.dstStart = inst.srcStart;
            rev.offset = inst.offset;
            reverse.push_back(rev);
        }
        sort(trans.begin(), trans.end());
        transformations.push_back(trans);
        sort(reverse.begin(), reverse.end());
        reverseTransformations.push_back(reverse);
    }
    reverse(reverseTransformations.begin(), reverseTransformations.end());

    // sort(seeds.begin(), seeds.end());
    // END READING INPUT


    // const int num_cores = omp_get_max_threads(); // Get the number of available cores
    const int num_cores = 1;
    cout << "Num cores: " << num_cores << endl;
    long minResults[num_cores];
    for (int i = 0; i < num_cores; ++i) {
        minResults[i] = INT64_MAX;
    }

    for (int c = 0; c < seeds.size(); ++c) {
        auto seedOffset = seeds[c];
    // for (auto seedOffset : seeds) {
        long i = PRINT_INTERVAL;
        // int thread_id = omp_get_thread_num(); // Get the thread ID
        int thread_id = 0;

        TransformationInterval interval;
        interval.start = seedOffset.seedStart;
        long seedEnd = seedOffset.seedStart + seedOffset.offset;
        interval.end = seedEnd;


        while (interval.start < seedEnd) {
            auto oldStart = interval.start;
            interval = findIntervalLoop(transformations, interval);
            // auto res = applyTransformations(transformations, interval.start);
            if (interval.start < minResults[thread_id]) {
                minResults[thread_id] = interval.start;
            }
            assert(oldStart == applyTransformations(reverseTransformations, interval.start));
            interval.start = applyTransformations(reverseTransformations, interval.end - 1) + 1;
            interval.end = seedEnd;

        }


        i -= seedOffset.offset;
        if (i < 0) {
            i = PRINT_INTERVAL;
            cout << "Thread " << thread_id << endl;
        }
    }

    long minResult = INT64_MAX;
    for (auto i : minResults) {
        if (i < minResult) {
            minResult = i;
        }
    }

    cout << "Min result: " << minResult << endl;

    return 0;
}
