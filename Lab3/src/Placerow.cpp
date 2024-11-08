#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

void PLACEROW::Init(int row, int site, double h, double x, double y) {
    row_num = row;
    site_num = site;
    height = h;
    xcoor = x;
    ycoor = y;

    placement_row.resize(row);
}

void PLACEROW::Insert(CELL* cell) {
    int row = (cell->GetY() - ycoor) / height;
    // cout << row << endl;
    placement_row[row].insert(cell);
}


