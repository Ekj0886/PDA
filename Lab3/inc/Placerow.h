#ifndef PLACEROW_H
#define PLACEROW_H
#include <bits/stdc++.h>
#include "Cell.h"

#define F first 
#define S second

using namespace std;

struct CompareByX {
    bool operator()(const CELL* a, const CELL* b) const {
        return a->LEFT() < b->LEFT();
    }
};

class PLACEROW {
  
public:
    DIE Die;
    int row_num;
    int site_num;
    double height;
    double xcoor, ycoor;
    
    vector<set<CELL*, CompareByX>> placement_row;

    // operator overwrite
    set<CELL*, CompareByX>& operator[](size_t index) {
        return placement_row[index];
    }

    // major function 
    void Init(int row, int site, double h, double x, double y);
    void Insert(CELL* cell);
    void Remove(CELL* cell);
    pair<int, int> Search(int row); 
    
    // helper function
    int GetRow(double y);
    bool Legal(CELL* cell);
    void PrintRow(int row);
};

#endif