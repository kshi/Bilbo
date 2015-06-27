#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// vector<char> state: entries 0 = space, 1 = A, ..., 26 = Z. Index 0,...,29 rune values, 30 Bilbo location, 31 cost, 32 prev Bilbo location

static int fanin = 10;
static int numToKeep = 40;

int ringDist(int x, int y, int modulus) {
  int absDist = min(abs(y-x), min(abs(x-modulus)+abs(y), abs(y-modulus)+abs(x)));
  if ((x + absDist) % modulus == y) {
    return absDist;
  }
  else {
    return -absDist;
  }
}

vector<vector<int>>        nextState.at(31) = prevState.at(31) + difference[indices.at(i)];
nextState.at(32) = prevTableIndex;
nextStates.push_back(nextState);
}
delete[] difference;
return nextStates;
}

void prune(vector<vector<int>>& states) {
  sort(begin(states), end(states), [&](vector<int> a, vector<int> b){return a.at(31) < b.at(31);});
  size_t N = states.size();
  for (int i=numToKeep; i<N; i++) {
    states.pop_back();
  }
}

string getSteps(vector<int> prevState, vector<int> nextState) {
  size_t i=nextState.at(30);
  string instructions;
  int moves = ringDist(prevState.at(30), i, 30);
  int jumps = ringDist(prevState.at(i), nextState.at(i), 27);
  if (moves > 0) {
    for (int j=0; j<moves; j++) {instructions += ">";}
  }
  else {
    for (int j=0; j<abs(moves); j++) {instructions += "<";}
  }
  if (jumps > 0) {
    for (int j=0; j<jumps; j++) {instructions += "+";}
  }
  else {
    for (int j=0; j<abs(jumps); j++) {instructions += "-";}
  }
  return instructions;
}

int main()
{
vector<vector<vector<int>
	      vector<vector<int>> nextStates;
 for (int n=0; n<prevStates.size(); n++) {
   vector<vector<int>> newStates = greedyFan(prevStates.at(n),magicPhraseNumeric[i],n);
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
  vector<int> nextState = DPTable.at(i+1).at(optEndIndex);
  optEndIndex = nextState.at(32);
  instructions = getSteps(DPTable.at(i).at( optEndIndex), nextState) + "." + instructions;
 }


cout << instructions << endl;
}
