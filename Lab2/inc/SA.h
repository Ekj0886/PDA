#ifndef SA_H
#define SA_H
#include <bits/stdc++.h>
#include "MyClass.h"
#include "VEB.h"

#define F first 
#define S second

using namespace std;

class SA { // Simulated Annealing

private:
    long long W_fp, H_fp; // max x, y of floorplan result
    long long Blk_area;
    string rFlag; // Rotate flag
    SequencePair SP_mem;


public:
    int alpha; 
    int W, H; // outline width/height
    int Blk_num, Ter_num; // Block and Terminal number
    SequencePair SP;
    unordered_map<string, BLK*> BlockList;
    unordered_map<string, TER*> TerminalList;
    vector<NET> NetList;

    SA(int in_alpha) { alpha = in_alpha; rFlag = ""; }

    // Flow function
    void Init();
    void Stage0(float T);
    void Walk(); // traverse to random neighborhood structure
    void ReverseWalk();

    // Utility Defined in SA_util.cpp
    void LoadUnit(string file);
    void LoadNet(string file);
    void GetCoordinate();
    void DumpFloorPlan(string);
    bool OutofBound() { return W_fp > W || H_fp > H; } // total floorplan exceed of outline
    bool Outside(BLK*); // BLK outside of outline
    
    // Neighborhood action
    void RotateBlk();
    void SwapX();
    void SwapY();
    void Swap();

    // Evaluate
    int DeadSpace() { return (W_fp*H_fp - Blk_area)*100/(W_fp*H_fp); }  // Cost = DeadSpace percentage
    float OutArea();
};

#endif