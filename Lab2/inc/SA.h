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
    int W_fp, H_fp; // max x, y of floorplan result
    int Blk_area;
    SequencePair SP;
    unordered_map<string, BLK*> BlockList;
    unordered_map<string, TER*> TerminalList;
    vector<NET> NetList;

    SA(int in_alpha) { alpha = in_alpha; }

    // Flow function
    void LoadUnit(string file);
    void LoadNet(string file);
    void Init();
    void Walk(); // traverse to random neighborhood structure

    // Neighborhood action
    void RotateBlk();
    void SwapX();
    void SwapY();
    void Swap();

    // Utility
    void GetCoordinate();
    void DumpFloorPlan(string);
    int DeadSpace() { return (W_fp*H_fp - Blk_area)*100/(W_fp*H_fp); }  // Cost = DeadSpace percentage
    bool OutofBound() { return W_fp > W || H_fp > H; } // total floorplan exceed of outline
    bool Outside(BLK*); // BLK outside of outline

};

#endif