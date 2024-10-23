#ifndef BSTAR_H
#define BSTAR_H
#include <bits/stdc++.h>
#include "MyClass.h"

#define F first 
#define S second

using namespace std;

struct Seg {
    int start;
    int end;
    int y;
    Seg() : start(0), end(0), y(0) {}
    Seg(int s, int e, int y) : start(s), end(e), y(y) {}
};

class Node {

private:
    BLK *last_blk;
    Node *last_Parent;
    Node *last_Left, *last_Right;

public:
    BLK *blk;
    Node *Parent;
    Node *Left, *Right;
    
    Node() { Parent = nullptr, Left = nullptr, Right = nullptr; }
    Node(BLK* block) { blk = block, blk->x = 0, blk->y = 0, 
                        Parent = nullptr, Left = nullptr, Right = nullptr; }
    Node(Node* p, Node* L, Node* R) { Parent = p, Left = L, Right = R; }

    string Name() { return blk->name; }

    int W() { return blk->w; }
    int H() { return blk->h; }
    int X() { return blk->x; }
    int Y() { return blk->y; }
    void SetX(int x) { blk->x = x; }
    void SetY(int y) { blk->y = y; }
    void RotateBlk() { blk->Rotate(); }
    void UpdateMem() { last_blk = blk; last_Parent = Parent; last_Left = Left; last_Right = Right; }
    void Recall() { blk = last_blk; Parent = last_Parent; Left = last_Left; Right = last_Right; }

};

class BSTAR {

private:
    long long W_fp, H_fp; // max x, y of floorplan result
    long long Blk_area;
    string rFlag; // Rotate flag
    
    Node *root;
    unordered_map<Node*, BLK*> Query;

public:
    float alpha; 
    int W, H; // outline width/height
    int Blk_num, Ter_num; // Block and Terminal number
    unordered_map<string, BLK*> BlockList;
    unordered_map<string, TER*> TerminalList;
    vector<NET> NetList;
    vector<Node*> NodeList;
    vector<Seg> Contour;

    BSTAR(float in_alpha) { alpha = in_alpha; rFlag = ""; }

    // Flow function (Defined in BS.cpp)
    void Walk();
    void ReverseWalk();
    void Init();
    void Stage0(float);
    void Stage1(float);
    void Stage2(float);
    void DumpFloorPlan(string);
    void DumpOutput(string, float);
    
    // Utility (Defined in BS_util.cpp)
    void GetLine(ifstream& infile, vector<string>& inst);
    void SkipEmpty(ifstream& infile, vector<string>& inst);
    void LoadUnit(string file);
    void LoadNet(string file);
    Node* BuildTree(); // initialize Query and B*tree
    Node* GetRandNode();
    Node* GetLeaf();
    void GetCoordinate();
    int  UpdateContour(int start, int end, int h);
    bool OutofBound() { return W_fp > W || H_fp > H; }
    bool Outside(BLK*); // BLK outside of outline
    
    // Neighborhood function
    void RotateBlk();
    void SwapNode();
    void MoveNode();

    // evaluation function
    float DeadSpace(); // Cost = DeadSpace percentage
    float OutArea();
    int   Wire();
    int   Cost();

    // Print function (for debug)
    void PrintTree();
    void PrintContour();

};

#endif