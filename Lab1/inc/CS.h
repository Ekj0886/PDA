#ifndef CS_H
#define CS_H
#include <bits/stdc++.h>
#include "TILE.h"

#define F first 
#define S second

using namespace std;

class CS {

public:
    int w;
    int h;
    map<int, TILE*> Block;
    map<int, TILE*> Space; 
    TILE *R_TILE, *T_TILE, *B_TILE, *L_TILE; // pseudo tiles, ID = 0
    int space_index; // keep track of current space index num
    vector<POINT> Point_vec; // store point-finding coordinates answer 


    void Init_Outline();
    TILE* Find_Point(int x, int y);
    void Create_Blk(int id, int x, int y, int w, int h);
    void SplitY(TILE* tile, int y);
    bool SplitX(TILE* tile, int x);
    void Merge(TILE* U_tile, TILE* D_tile);
    TILE* MergeBLK(TILE* U_tile, TILE* D_tile, int y);
    

    // util function
    void Init_Case0();
    void Dump_Layout(string);
    void Dump_Output(string);
    void PrintMap();
};

#endif