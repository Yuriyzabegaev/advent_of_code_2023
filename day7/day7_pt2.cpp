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

regex pattern("(\\S+) (\\d+)");

enum CardCombination : int {
    FIVE = 6,
    FOUR = 5,
    FULLHOUSE = 4,
    THREE = 3,
    TWOPAIRS = 2,
    PAIR = 1,
    NONE = 0,
};

const int JOKER = 1;

const int CARDS_IN_HAND = 5;

array<int, CARDS_IN_HAND> parseCardValues(const string &cards) {
    array<int, CARDS_IN_HAND> res; 
    assert(cards.size() == CARDS_IN_HAND);
    for (size_t i = 0; i < CARDS_IN_HAND; ++i) {
        auto c = cards[i];
        switch (c)
        {
        case 'A':
            res[i] = 14;
            break;
        case 'K':
            res[i] = 13;
            break;
        case 'Q':
            res[i] = 12;
            break;
        case 'J':
            res[i] = JOKER;
            break;
        case 'T':
            res[i] = 10;
            break;
        default:
            res[i] = stoi(string(1, c));
            break;
        }
    }
    return res;
}

class Hand {
public:
    array<int, 5> cardsValues;
    string cards;
    int bid;

    Hand(string cards, int bid) {
        this->bid = bid;
        this->cards = cards;
        this->cardsValues = parseCardValues(cards);
    }

    bool operator<(const Hand &other) const {
        for (int i = 0; i < CARDS_IN_HAND; ++i) {
            if (cardsValues[i] != other.cardsValues[i]) {
                return cardsValues[i] < other.cardsValues[i];
            }
        }
        assert(false);
    }

    CardCombination checkCombination() const {
        unordered_map<int, int> valuesRepeats;
        int numJokers = 0;
        for (int i = 0; i < CARDS_IN_HAND; ++i) {
            auto card = cardsValues[i];
            if (card == JOKER) {
                numJokers += 1;
                continue;
            }
            if (valuesRepeats.find(card) == valuesRepeats.end()) {
                valuesRepeats[card] = 1;
            } else {
                valuesRepeats[card] += 1;
            }
        }

        int numPairs = 0;
        int numThrees = 0;
        int numFours = 0;
        int numFives = 0;
        for (auto keyVal : valuesRepeats) {
            int repeat = keyVal.second;
            switch (repeat)
            {
            case 5:
                numFives += 1;
                break;
            case 4:
                numFours += 1;
                break;
            case 3:
                numThrees +=1;
                break;
            case 2:
                numPairs += 1;
                break;
            }
        }

        if (numJokers >= 4) {
            return FIVE;
        } else if (numJokers == 3) {
            if (numPairs == 1) {
                return FIVE;
            } else {
                return FOUR;
            }
        } else if (numJokers == 2) {
            if (numThrees == 1) {
                return FIVE;
            } else if (numPairs == 1) {
                return FOUR;
            } else {
                return THREE;
            }
        } else if (numJokers == 1) {
            if (numFours == 1) {
                return FIVE;
            } else if (numThrees == 1) {
                return FOUR;
            } else if (numPairs == 2) {
                return FULLHOUSE;
            } else if (numPairs == 1) {
                return THREE;
            } else {
                return PAIR;
            }
        }
        
        assert(numJokers == 0);
        if (numFives == 1) {
            return FIVE;
        } else if (numFours == 1) {
            return FOUR;
        } else if (numThrees == 1 && numPairs == 1) {
            return FULLHOUSE;
        } else if (numThrees == 1 && numPairs == 0) {
            return THREE;
        } else if (numPairs == 2) {
            return TWOPAIRS;
        } else if (numPairs == 1) {
            return PAIR;
        } else {
            return NONE;
        }
    }
};


int main()
{
    string filename = "input.txt";

    ifstream inputFile(filename);
    assert(inputFile.is_open());
    string line;
    array<vector<Hand>, 7> inputData;
    while (getline(inputFile, line)) {
        smatch match;
        assert(regex_search(line, match, pattern));
        auto cards = match[1].str();
        auto bid = stoi(match[2].str());
        Hand hand(cards, bid);
        auto combination = hand.checkCombination();
        inputData[combination].push_back(hand);
    }

    int rank = 1;
    long result = 0;
    for (auto hands : inputData) {
        sort(hands.begin(), hands.end());
        for (auto hand : hands) {
            result += rank * hand.bid;
            rank++;
        }

    }

    cout << result << endl;

    return 0;
}
