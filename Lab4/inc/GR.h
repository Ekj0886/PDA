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
    PriorityQ open_list;
    unordered_set<Gcell*> open_set;
    unordered_set<Gcell*> close_list;
    unordered_map<int, deque<Gcell*>*> path_list;
    deque<Gcell*>* path;
    string outfile_name;
    string outimage_name;

public: // functions
    void Parse(string&, string&, string&, string&, string&);
    void SetGrid();
    void DirectRoute(); // A* search
    bool A_star(Gcell*, Gcell*);
    void TracePath(int, Gcell*, Gcell*);
    void UpdateCap();
    void DumpOutput(int id, Net n, Gcell* src, Gcell* snk);
    void DumpImage();

public: // util function
    Gcell* GetCell(int x, int y);
    double Cost(Gcell* neighbor, Gcell* current);
    bool overflow(Gcell*);


};

#endif