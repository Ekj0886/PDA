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

using RowIterator = std::optional<std::set<CELL*, CompareByX>::iterator>;
using Rptr = std::set<CELL*, CompareByX>::iterator;

class PLACEROW {
  
public:
    DIE Die;
    int row_num;
    int site_num;
    double height;
    double xcoor, ycoor;
    
    vector<set<CELL*, CompareByX>> placement_row;
    vector<double> space;


    // operator overwrite
    set<CELL*, CompareByX>& operator[](size_t index) {
        return placement_row[index];
    }

    // major function 
    void Init(int row, int site, double h, double x, double y);
    void Insert(CELL* cell);
    void Remove(CELL* cell);
    bool FindVacant(CELL*);
    bool SingleVacant(CELL*);
    void DumbFill(CELL*);

    // helper function
    int GetRow(double y);
    int GetTrack(CELL*); // get cell height in # of row
    bool Legal(CELL*);
    bool InBound(int);
    double TOP() { return ycoor + height*(row_num-1); }
    double LEFT() { return xcoor; }
    double RIGHT() { return xcoor + site_num - 1; }
    void PrintRow(int row);
    void PrintPR();

    RowIterator Xs(int row, double x);
    RowIterator Xe(int row, double x);

    void test();

};

#endif