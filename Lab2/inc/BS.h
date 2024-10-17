#ifndef BSTAR_H
#define BSTAR_H
#include <bits/stdc++.h>
#include "MyClass.h"

#define F first 
#define S second

using namespace std;

struct Node {
    BLK *blk;
    Node *parent;
    Node *Left, *Right;
};

class BSTAR {

private:
    long long W_fp, H_fp; // max x, y of floorplan result
    long long Blk_area;
    string rFlag; // Rotate flag
public:
    float alpha; 
    int W, H; // outline width/height
    int Blk_num, Ter_num; // Block and Terminal number
    unordered_map<string, BLK*> BlockList;
    unordered_map<string, TER*> TerminalList;
    vector<NET> NetList;

    void LoadUnit(string file);
    void LoadNet(string file);
    
};

#endif