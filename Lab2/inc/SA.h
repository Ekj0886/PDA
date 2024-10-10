#ifndef SA_H
#define SA_H
#include <bits/stdc++.h>
#include "MyClass.h"
#include "VEB.h"

#define F first 
#define S second

using namespace std;

class SA { // Simulated Annealing
    
public:
    int alpha; 
    int W, H; // outline width/height
    int Blk_num, Ter_num; // Block and Terminal number

    SequencePair SP;
    unordered_map<string, BLK*> BlockList;
    unordered_map<string, TER*> TerminalList;
    vector<NET> NetList;

    SA(int in_alpha) { alpha = in_alpha; }

    void LoadUnit(string file);
    void LoadNet(string file);
    void GetCoordinate();
    void Init();


};

#endif