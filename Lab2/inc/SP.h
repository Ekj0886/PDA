#ifndef SP_FP_H
#define SP_FP_H
#include <bits/stdc++.h>
#include "MyClass.h"
#include "VEB.h"

#define F first 
#define S second

using namespace std;

class SP_FP { // Simulated Annealing

private:
    long long W_fp, H_fp; // max x, y of floorplan result
    long long Blk_area;
    string rFlag; // Rotate flag
    SequencePair SP_mem;
    int Best_Cost;
    SequencePair SP_Best;
    unordered_map<BLK*, pair<int, int>> Ratio_mem;
    double time_limit = 295;
    

public:
    double alpha; 
    int W, H; // outline width/height
    int Blk_num, Ter_num; // Block and Terminal number
    SequencePair SP;
    unordered_map<string, BLK*> BlockList;
    unordered_map<string, TER*> TerminalList;
    vector<NET> NetList;

    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;

    SP_FP(double in_alpha) { alpha = in_alpha; rFlag = ""; start = std::chrono::high_resolution_clock::now(); }

    // Flow function (Defined in SP.h)
    void Walk(); // traverse to random neighborhood structure
    void ReverseWalk();
    void Init();
    void Stage0(float T);
    void Stage1(float T);
    void Stage2(float T);
    void DumpOutput(string, float);

    // Utility (Defined in SP_util.cpp)
    void GetLine(ifstream& infile, vector<string>& inst);
    void SkipEmpty(ifstream& infile, vector<string>& inst);
    void LoadUnit(string file);
    void LoadNet(string file);
    void GetCoordinate();
    void DumpFloorPlan(string);
    bool OutofBound() { return W_fp > W || H_fp > H; } // total floorplan exceed of outline
    bool Outside(BLK*); // BLK outside of outline
    void DumpRatio();
    void LoadRatio();
    void SaveBest();
    void LoadBest();
    float GetTime();
    
    // Neighborhood action
    void RotateBlk();
    void SwapX();
    void SwapY();
    void Swap();

    // Evaluate
    float DeadSpace(); // Cost = DeadSpace percentage
    float OutArea();
    int   Wire();
    int   Cost();

};

#endif