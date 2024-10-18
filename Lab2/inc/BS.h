#ifndef BSTAR_H
#define BSTAR_H
#include <bits/stdc++.h>
#include "MyClass.h"

#define F first 
#define S second

using namespace std;

struct contour {
    
};

class Node {

public:
    BLK *blk;
    Node *Parent;
    Node *Left, *Right;
    
    Node() { Parent = nullptr, Left = nullptr, Right = nullptr; }
    Node(BLK* block) { blk = block; Parent = nullptr, Left = nullptr, Right = nullptr; }
    Node(Node* p, Node* L, Node* R) { Parent = p, Left = L, Right = R; }

    string Name() { return blk->name; }
};

class BSTAR {

private:
    long long W_fp, H_fp; // max x, y of floorplan result
    long long Blk_area;
    // string rFlag; // Rotate flag
    
    Node *root;
    unordered_map<Node*, BLK*> Query;

public:
    float alpha; 
    int W, H; // outline width/height
    int Blk_num, Ter_num; // Block and Terminal number
    unordered_map<string, BLK*> BlockList;
    unordered_map<string, TER*> TerminalList;
    vector<NET> NetList;

    void GetLine(ifstream&, vector<string>&);
    void SkipEmpty(ifstream& infile, vector<string>& inst);
    void LoadUnit(string file);
    void LoadNet(string file);

    Node* BuildTree(); // initialize Query and B*tree
    Node* GetRandNode();
    void PrintTree(Node*, int);
    void GetCoordinate();

    void Init();



};

#endif