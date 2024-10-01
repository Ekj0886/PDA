#ifndef TILE_H
#define TILE_H
#include <bits/stdc++.h>

#define F first 
#define S second

using namespace std;

class POINT {
public:
    POINT(int px, int py) {
        x = px;
        y = py;
    }
    POINT() { };
    int x;
    int y;
};

class TILE {
    
public:
    int ID;
    TILE* tr; // R
    TILE* rt; // T
    TILE* lb; // B
    TILE* bl; // L
    POINT pt;

    TILE() {}
    TILE(int id, POINT PT) {
        ID = id;
        tr = nullptr;
        rt = nullptr;
        lb = nullptr;
        bl = nullptr;
        pt = PT;
    }

    void Set_Neighbor(TILE*, TILE*, TILE*, TILE*);
    int BOTTOM() { return pt.y; }
    int LEFT()   { return pt.x; }
    int TOP()    { return rt->BOTTOM(); }
    int RIGHT()  { return tr->LEFT(); }
    int WIDTH()  { return RIGHT() - LEFT(); }
    int HEIGHT() { return TOP() - BOTTOM(); }
    bool Inside(int x, int y);
};

#endif