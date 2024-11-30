#include <bits/stdc++.h>
#include "GR.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

Gcell* GR::GetCell(int x, int y) {
    x -= RA.x, y -= RA.y;
    int j = x / cell_w;
    int i = y / cell_h;
    return Grid[i][j];
}

double GR::Cost(Gcell* cell) {
    Gcell* parent = cell->parent;
    double mcost;
    bool via = false;
    if(cell->dir == Vertical) mcost = cell->m1_cost;
    else if(cell->dir == Horizontal) mcost = cell->m2_cost;
    if(cell->dir != parent->dir) {
        via = true;
        mcost += (parent->m1_cost + parent->m2_cost) / 2.0;
        if(cell->dir == Vertical) mcost -= parent->m2_cost;
        else                      mcost -= parent->m1_cost;
    }
    return alpha + beta * overflow(cell) + gamma * mcost + delta * via;
}

bool GR::overflow(Gcell* child) {

    Gcell* parent = child->parent;

    if(parent->R == child) return child->left_cap < 0;
    if(parent->L == child) return parent->left_cap < 0;
    if(parent->U == child) return child->down_cap < 0;
    if(parent->D == child) return parent->down_cap < 0;
    else {
        cout << "Wrong Neighboring Relation" << endl;
        return false;
    }

}
