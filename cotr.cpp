#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;
 
 // vector<char> state: entries 0 = space, 1 = A, ..., 26 = Z. Index 0,...,29 rune values, 30 Bilbo location, 31 cost, 32 prev Bilbo location
 // 33 prev-to-current instructions: 1:<, 2:>, 3: -, 4:+, 5: [, 6:]
 
static int fanin = 4;
static int numToKeep = 70;
 
int ringDist(int x, int y, int modulus) {
    int absDist = min(abs(y-x), min(abs(x-modulus)+abs(y), abs(y-modulus)+abs(x)));
    if ((x + absDist) % modulus == y) {
        return absDist;
    }
    else {
        return -absDist;
    }
}

vector<vector<long>> greedyFan(vector<long> prevState, int nextChar, size_t prevTableIndex) 
{    
    int* zoneDifference = new int[30];
    int* zoneLoopDifference = new int[30];
    int* runeDifference = new int[30];
    int* runeLoopDifference = new int[30];
    int* difference = new int[30];
    for (int i=0; i<30; i++) {
        zoneDifference[i] = abs(ringDist(i, prevState.at(30), 30));
        runeDifference[i] = abs(ringDist(prevState.at(i), nextChar, 27));
        zoneLoopDifference[i] = 99;
        runeLoopDifference[i] = 3 + abs(ringDist(0,nextChar,27));
    }
    size_t iterator = prevState.at(30);
    while (prevState.at(iterator) != 0) {
        iterator = (iterator + 1) % 30;
        if (iterator == prevState.at(30)) {break;}
    }
    if (iterator != prevState.at(30)) {zoneLoopDifference[iterator] = 3;}
    size_t zoneRight = iterator;
    iterator = prevState.at(30);
    while (prevState.at(iterator) != 0) {
        iterator = (iterator + 29) % 30;
        if (iterator == prevState.at(30)) {break;}
    }
    if (iterator != prevState.at(30)) {zoneLoopDifference[iterator] = 3;}
    size_t zoneLeft = iterator;
    for (int i=0; i<30; i++) {
        difference[i] = min(zoneDifference[i], zoneLoopDifference[i]) + min(runeDifference[i], runeLoopDifference[i]);
    }
    
    vector<size_t> indices;
    for (size_t i=0; i<30; i++) {
        indices.push_back(i);
    }
    sort(begin(indices), end(indices), [&](size_t a, size_t b){ return difference[a] < difference[b];});
    
    vector<vector<long>> nextStates;
    for (int i=0; i<fanin; i++) {
        size_t index = indices.at(i);
        vector<long> nextState = prevState;
        nextState.at(30) = index;
        nextState.at(index) = nextChar;
        nextState.at(32) = prevTableIndex;
        nextState.at(31) = prevState.at(31) + difference[index];

        string instructions;
        if (zoneDifference[index] <= zoneLoopDifference[index]){
            int moves = ringDist(prevState.at(30), nextState.at(30), 30);
            if (moves > 0) {
                for (int j=0; j<moves; j++) {instructions += "2";}
            }
            else {
                for (int j=0; j<abs(moves); j++) {instructions += "1";}
            }
        }
        else {
            if (zoneRight == index) {instructions += "526";}
            else if (zoneLeft == index) {instructions += "516";}
            else {cout << "loop error" << endl;}
        }
        
        if (runeDifference[index] <= runeLoopDifference[index]){
            int jumps = ringDist(prevState.at(index), nextState.at(index), 27);
            if (jumps > 0) {
                for (int j=0; j<jumps; j++) {instructions += "4";}
            }
            else {
                for (int j=0; j<abs(jumps); j++) {instructions += "3";}
            }
        }
        else {
            instructions += "546";
            int jumps = ringDist(0, nextState.at(index), 27);
            if (jumps > 0) {
                for (int j=0; j<jumps; j++) {instructions += "4";}
            }
            else {
                for (int j=0; j<abs(jumps); j++) {instructions += "3";}
            }            
        } 
        if (instructions.size() > 0) {
            nextState.at(33) = stol(instructions);
        }
        else {
            nextState.at(33) = 0;
        }
        nextStates.push_back(nextState);
    }
    delete[] difference;
    return nextStates;
}

void prune(vector<vector<long>>& states) {
    sort(begin(states), end(states), [&](vector<long> a, vector<long> b){return a.at(31) < b.at(31);});
    size_t N = states.size();
    for (int i=numToKeep; i<N; i++) {
        states.pop_back();
    }
}

string parseInstructions(long code) {
    string numInstructions = to_string(code);
    const char* cstr = numInstructions.c_str();    
    string instructions;
    for (int i=0; i<strlen(cstr); i++) {
        int nextChar = static_cast<int>(cstr[i]) - 48;
        switch(nextChar) {
            case 0:
            instructions += "";
            break;
            case 1:
            instructions += "<";
            break;
            case 2:
            instructions += ">";
            break;
            case 3:
            instructions += "-";
            break;
            case 4:
            instructions += "+";
            break;
            case 5:
            instructions += "[";
            break;
            case 6:
            instructions += "]";
            break;
            default:
            cout << "instructions error" << endl;
            break;
        }
    }
    return instructions;
}

int main()
{
    vector<vector<vector<long>>> DPTable;
    string magicPhrase;
    getline(cin, magicPhrase);
    const char* cstr = magicPhrase.c_str();
    int* magicPhraseNumeric = new int[strlen(cstr)];
    for (int i=0; i<strlen(cstr); i++) {
        if (cstr[i] == 32) {
            magicPhraseNumeric[i] = 0;
        }
        else {
            magicPhraseNumeric[i] = cstr[i] - 64;
        }
    }
    
    vector<long> initState (34,0);
    vector<vector<long>> prevStates;
    prevStates.push_back(initState);
    DPTable.push_back(prevStates);
    string instructions;
    for (int i=0; i<strlen(cstr); i++) {
        
        vector<vector<long>> nextStates;
        for (int n=0; n<prevStates.size(); n++) {
            vector<vector<long>> newStates = greedyFan(prevStates.at(n),magicPhraseNumeric[i],n);
            nextStates.reserve(nextStates.size() + newStates.size());
            nextStates.insert(nextStates.end(),newStates.begin(),newStates.end());
        }
        prune(nextStates);
        prevStates = nextStates;
        DPTable.push_back(nextStates);
    }
    delete[] magicPhraseNumeric;    
    
    // build path from DPTable
    int optEndPath = 9000;
    size_t optEndIndex = 0;
    for (int i=0; i<DPTable.at(DPTable.size()-1).size(); i++) {
        if (DPTable.at(DPTable.size()-1).at(i).at(31) < optEndPath) {
            optEndIndex = i;
            optEndPath = DPTable.at(DPTable.size()-1).at(i).at(31);
        }
    }
    for (int i=DPTable.size()-2; i>=0; i--) {
        vector<long> nextState = DPTable.at(i+1).at(optEndIndex);
        optEndIndex = nextState.at(32);
        instructions = parseInstructions(nextState.at(33)) + "." + instructions;
    }
    
    cout << instructions << endl;
}