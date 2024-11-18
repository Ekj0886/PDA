#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

int PLACEROW::GetRow(double y) {
    return (y - ycoor) / height;
}

int PLACEROW::GetTrack(CELL* cell) {
    return cell->GetH() / height;
}

bool PLACEROW::Overlap(CELL* c1, CELL* c2) {
    bool H_overlap = c1->LEFT() < c2->RIGHT() && c1->RIGHT() > c2->LEFT();
    bool V_overlap = c1->DOWN() < c2->TOP() && c1->TOP() > c2->DOWN();
    return H_overlap && V_overlap;
}

bool PLACEROW::Legal(CELL* cell) {

    // fix cell y if not on site
    if((double)((cell->DOWN() - ycoor) / height) != (int)((cell->DOWN() - ycoor) / height) ) {
        cout << "Tweak y" << endl;
        double legal_y = ycoor + (double)height * (int)((cell->DOWN() - ycoor) / height);
        cell->SetXY(cell->LEFT(), legal_y);
    }

    // check Die bound
    if(cell->LEFT() < Die.lowerX || cell->RIGHT() > Die.upperX || cell->DOWN() < Die.lowerY || cell->TOP() > Die.upperY) return false;

    // check within placement row
    if(cell->LEFT() < xcoor || cell->LEFT() > xcoor + site_num - 1) return false;
    
    // check overlap
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {

        int row = GetRow(y);

        Rptr ub = RowSet(row)->lower_bound(cell);
        Rptr lb = prev(ub);

        CELL* ub_cell = *ub;
        CELL* lb_cell = *lb;
        
        // Check if an upper bound & lower bound
        if(Overlap(cell, lb_cell)) return false;
        if(Overlap(cell, ub_cell)) return false;

        y += height;
    }
    
    return true;

}

bool PLACEROW::SRLegal(CELL* cell) {
    // fix cell y if not on site
    if((double)((cell->DOWN() - ycoor) / height) != (int)((cell->DOWN() - ycoor) / height) ) {
        cout << "Tweak y" << endl;
        double legal_y = ycoor + (double)height * (int)((cell->DOWN() - ycoor) / height);
        cell->SetXY(cell->LEFT(), legal_y);
    }

    // check Die bound
    if(cell->LEFT() < Die.lowerX || cell->RIGHT() > Die.upperX || cell->DOWN() < Die.lowerY || cell->TOP() > Die.upperY) return false;

    // check within placement row
    if(cell->LEFT() < xcoor || cell->LEFT() > xcoor + site_num - 1) return false;
    
    // check overlap
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);

        Rptr ub = RowSet(row)->lower_bound(cell);
        Rptr lb = prev(ub);

        CELL* ub_cell = *ub;
        CELL* lb_cell = *lb;

        bool ub_sr = (ub_cell->Fix() || ub_cell->merge);
        bool lb_sr = (lb_cell->Fix() || lb_cell->merge);

        // Check if an upper bound & lower bound
        if(!lb_cell->pseudo && Overlap(cell, lb_cell) && lb_sr) return false;
        if(!ub_cell->pseudo && Overlap(cell, ub_cell) && ub_sr) return false;

        y += height;
    }

    return true;

}

void PLACEROW::PrintRow(int row) {
    cout << row << ": ";
    if(RowSet(row)->empty()) {
        cout << "EMPTY" << endl;
        return; 
    }
    for(const auto& c : *RowSet(row)) {
        // cout << "(" << c->LEFT() << ", " << c->RIGHT() << ") ";
        cout << c->GetName() << " ";
    }cout << endl;
}

void PLACEROW::PrintPR() {
    for(int i = row_num-1; i >= 0; i--) {
        PrintRow(i);
    }
}

void PLACEROW::Insert(CELL* cell) {
    if(!Legal(cell)) {
        cout << cell->GetName() << " insert Not Legal" << endl;
    }
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {

        int row = GetRow(y);
        RowSet(row)->insert(cell);
        y += height;
    }
}

void PLACEROW::Remove(CELL* cell) {
    if(cell->Fix()) {
        cout << "Cell fixed, can't remove" << endl;
        return;
    }

    int start_row = GetRow(cell->DOWN());
    int end_row   = min( (int)ceil(cell->GetH() / height) + start_row - 1, row_num - 1);

    for(int row = start_row; row <= end_row; row++) {
        if(RowSet(row)->find(cell) == RowSet(row)->end()) cout << "no cell " << cell->GetName() << endl;
        RowSet(row)->erase(cell);
    }
}

bool PLACEROW::InBound(int row) {
    return (row >= 0 && row < row_num);
}

Rptr PLACEROW::Uptr(int row, CELL* cell) {
    return RowSet(row)->lower_bound(cell);
}

Rptr PLACEROW::Lptr(int row, CELL* cell) {
    Rptr ub = RowSet(row)->lower_bound(cell);
    return prev(ub);
}

