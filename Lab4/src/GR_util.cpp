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

double GR::Cost(Gcell* neighbor, Gcell* current) {
    
    double mcost;
    bool via = false;
    
    if(neighbor->dir == Vertical) mcost = neighbor->m1_cost;
    else                          mcost = neighbor->m2_cost;

    if(neighbor->dir != current->dir) {
        via = true;
        mcost += (current->m1_cost + current->m2_cost) / 2.0;
    }
    
    double gcost = alpha + beta * overflow(neighbor) + gamma * mcost + delta * via;

    return gcost;

}

bool GR::overflow(Gcell* child) {
    Gcell* parent = child->parent;

    if(parent->R == child) return child->left_cap <= 0;
    if(parent->L == child) return parent->left_cap <= 0;
    if(parent->U == child) return child->down_cap <= 0;
    if(parent->D == child) return parent->down_cap <= 0;
    else {
        cout << "Wrong Neighboring Relation" << endl;
        return false;
    }
}