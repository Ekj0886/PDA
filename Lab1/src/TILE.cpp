#include <bits/stdc++.h>
#include "TILE.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

void TILE::Set_Neighbor(TILE* TR, TILE* RT, TILE* LB, TILE* BL) {
    tr = TR;
    rt = RT;
    lb = LB;
    bl = BL;
}

bool TILE::Inside(int x, int y) {
    if(x >= LEFT() && x < RIGHT() && y >= BOTTOM() && y < TOP() ) return true;
    else return false;
}