#ifndef SA_H
#define SA_H
#include <bits/stdc++.h>
#include "MyClass.h"

#define F first 
#define S second

using namespace std;

class SA { // Simulated Annealing
    
public:
    SequencePair sp;
    unordered_map<string, BLK*> BlockList;
    unordered_map<string, TER*> TerminalList;
    vector<NET> NetList;

    void LoadUnit(string file);
    void LoadNet(string file);

};

#endif