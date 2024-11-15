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
        if(!lb_cell->pseudo && lb_cell->RIGHT() > cell->LEFT()) return false;
        if(!ub_cell->pseudo && ub_cell->LEFT() < cell->RIGHT()) return false;

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

        bool ub_sr = (ub_cell->GetH() > height) || (ub_cell->Fix());
        bool lb_sr = (lb_cell->GetH() > height) || (lb_cell->Fix());

        // Check if an upper bound & lower bound
        if(!lb_cell->pseudo && lb_cell->RIGHT() > cell->LEFT()) {
            if(lb_sr) return false;
        }
        if(!ub_cell->pseudo && ub_cell->LEFT() < cell->RIGHT()) {
            if(ub_sr) return false;
        }

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
        cout << "(" << c->LEFT() << ", " << c->RIGHT() << ") ";
    }cout << endl;
}


void PLACEROW::PrintPR() {
    for(int i = row_num-1; i >= 0; i--) {
        PrintRow(i);
    }
}




void PLACEROW::Insert(CELL* cell) {
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);
        RowSet(row)->insert(cell);
        // space[row] -= cell->GetW();
        y += height;
    }
}

void PLACEROW::Remove(CELL* cell) {
    if(cell->Fix()) {
        cout << "Cell fixed, can't remove" << endl;
        return;
    }
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);
        RowSet(row)->erase(cell);
        // space[row] += cell->GetW();
        y += height;
    }
}



bool PLACEROW::InBound(int row) {
    return (row >= 0 && row < row_num);
}
