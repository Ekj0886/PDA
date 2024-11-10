#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

int PLACEROW::GetRow(double y) {
    return (y - ycoor) / height;
}

bool PLACEROW::Legal(CELL* cell) {
    // check Die bound
    if(cell->LEFT() < Die.lowerX || cell->RIGHT() > Die.upperX || cell->DOWN() < Die.lowerY || cell->TOP() > Die.upperY) return false;

    // check within placement row
    if(cell->LEFT() < xcoor || cell->LEFT() > xcoor + site_num ) return false;

    // check overlap
    int row_num = GetRow(cell->DOWN());
    auto ub = placement_row[row_num].upper_bound(cell);

    // Check if an upper bound was found and use it to determine legality
    if (ub != placement_row[row_num].end()) {
        CELL* upper_bound_cell = *ub;
        return upper_bound_cell->LEFT() >= cell->RIGHT();
    }

    return true;
}

void PLACEROW::PrintRow(int row) {
    for(const auto& c : placement_row[row]) {
        cout << c->GetName() << " ";
    }cout << endl;
}

void PLACEROW::Init(int row, int site, double h, double x, double y) {
    row_num = row;
    site_num = site;
    height = h;
    xcoor = x;
    ycoor = y;

    placement_row.resize(row);
}

void PLACEROW::Insert(CELL* cell) {
    int y = cell->DOWN();
    while(y < cell->TOP()) {
        int row = GetRow(y);
        placement_row[row].insert(cell);
        y += height;
    }
}

void PLACEROW::Remove(CELL* cell) {
    if(cell->Fix()) {
        cout << "Cell fixed, can't remove" << endl;
        return;
    }
    int y = cell->DOWN();
    while(y < cell->TOP()) {
        int row = GetRow(y);
        placement_row[row].erase(cell);
        y += height;
    }
}

