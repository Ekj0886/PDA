#ifndef GR_H
#define GR_H
#include <bits/stdc++.h>
#include "Type.h"
#include "Parser.h"

#define F first 
#define S second

using namespace std;

class GR {

public: // main structure member
    RouteArea RA;
    Chip chip1, chip2;
    unordered_map<int, Net> Bump_map;
    vector<vector<Gcell*>> Grid;
    double alpha, beta, gamma, delta;
    double via_cost;

public: // other member
    int cell_w, cell_h;
    int grid_w, grid_h;
    Gcell* pseudo_cell;
    Set open_list;
    set<Gcell*> close_list;

public: // functions
    void Parse(string&, string&, string&);
    void SetGrid();
    void DirectRoute(); // A* search
    bool A_star(Gcell*, Gcell*);
    void TracePath(Gcell*, Gcell*);

public: // util function
    Gcell* GetCell(int x, int y);
    double Cost(Gcell* cell);
    bool overflow(Gcell*);

};

#endif