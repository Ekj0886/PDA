#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

int PLACEROW::GetRow(double y) {
    return (y - ycoor) / height;
}

int PLACEROW::GetTrack(CELL* cell) {
    return cell->GetH() / height;
}

BoundCell PLACEROW::Bcell(int row, CELL* cell) {
    BoundCell B;
    // cout << "B start" << endl;
    auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell->LEFT(),
                [](const CELL* c, double value) {
                return c->LEFT() < value; });
    // cout << "ptr get" << endl;
    auto lptr = uptr-1;
    B.Ucell = *uptr;
    B.Lcell = *lptr;
    return B; 
}

BoundPtr PLACEROW::BIndex(int row, CELL* cell) {
    BoundPtr B;
    auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell->LEFT(),
                [](const CELL* c, double value) {
                return c->LEFT() < value; });

    auto lptr = uptr-1;
    B.Uptr = uptr;
    B.Lptr = lptr;
    return B;
}

Rptr PLACEROW::U_ptr(int row, CELL* cell) {
    auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell->LEFT(),
                [](const CELL* c, double value) {
                return c->LEFT() < value; });
    return uptr;
}

Rptr PLACEROW::L_ptr(int row, CELL* cell) {
    auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell->LEFT(),
                [](const CELL* c, double value) {
                return c->LEFT() < value; });
    auto lptr = uptr - 1;
    return lptr;
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
    for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {
        // if(!InBound(row)) cout << "not inbound" << endl;
        BoundCell B = Bcell(row, cell);
        
        // Check cell upper bound & lower bound
        if(!B.Lcell->pseudo && B.Lcell->RIGHT() > cell->LEFT()) return false;
        if(!B.Ucell->pseudo && B.Ucell->LEFT() < cell->RIGHT()) return false;
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
    for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {
        BoundCell B = Bcell(row, cell);
        bool Umult = (B.Ucell->GetH() > height) || (B.Ucell->Fix());
        bool Lmult = (B.Lcell->GetH() > height) || (B.Lcell->Fix());
        // Check cell upper bound & lower bound
        if(!B.Lcell->pseudo && (B.Lcell->RIGHT() > cell->LEFT() || Lmult)) return false;
        if(!B.Ucell->pseudo && (B.Ucell->LEFT() < cell->RIGHT() || Umult)) return false;
    }

    return true;

}


void PLACEROW::PrintRow(int row) {
    cout << row << ": ";
    if(P_Row[row].empty()) {
        cout << "EMPTY" << endl;
        return; 
    }
    for(auto c : P_Row[row]) {
        cout << "(" << c->LEFT() << ", " << c->RIGHT() << ") ";
    }cout << endl;
}


void PLACEROW::PrintPR() {
    for(int i = row_num-1; i >= 0; i--) {
        PrintRow(i);
    }
}


bool PLACEROW::InBound(int row) {
    return (row >= 0 && row < row_num);
}

