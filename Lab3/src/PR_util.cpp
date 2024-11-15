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

    P_Row.resize(row);
    space.resize(row);
    CELL* pseudo_cell_left = new CELL("L", Die.lowerX-1, 0, 1, 0, 1);
    CELL* pseudo_cell_right = new CELL("R", Die.upperX, 0, 1, 0, 1);
    pseudo_cell_left->pseudo = true; 
    pseudo_cell_right->pseudo = true;
    for(int r = 0; r < row; r++) {
        P_Row[r].push_back(pseudo_cell_left);
        P_Row[r].push_back(pseudo_cell_right);
        space[r] = true;
    }
}


void PLACEROW::Insert(CELL* cell) {
    for(int row = GetRow(cell->DOWN()); row <= min(GetRow(cell->TOP()), row_num-1); row++) {
        auto vptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell,
            [](CELL* a, CELL* b) {
                return a->LEFT() < b->LEFT();
            });
        
        P_Row[row].insert(vptr, cell);
    }
}

void PLACEROW::Remove(CELL* cell) {
    if(cell->Fix()) {
        cout << cell->GetName() << " Cell fixed, can't remove" << endl;
        return;
    }

    for(int row = GetRow(cell->DOWN()); row <= min(GetRow(cell->TOP()), row_num-1); row++) {
        auto vptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell,
            [](CELL* a, CELL* b) {
                return a->LEFT() < b->LEFT();
            });
        
        P_Row[row].erase(vptr);
    }
}


int PLACEROW::GetRow(double y) {
    return (y - ycoor) / height;
}

int PLACEROW::GetTrack(CELL* cell) {
    return cell->GetH() / height;
}

int PLACEROW::U_index(int row, double d) {
    auto it = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), d,
        [](const CELL* cell, double target) {
            return cell->LEFT() < target;  // Compare the LEFT() member function of CELL to `d`
        });

    // Check if we found a valid element, or if `it` is at the end of the vector
    if (it != P_Row[row].end()) {
        return std::distance(P_Row[row].begin(), it);  // Return the index of the found element
    } else {
        return -1;  // If no such element exists, return -1 or handle as needed
    }
}

int PLACEROW::L_index(int row, double d) {
    auto it = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), d,
        [](const CELL* cell, double target) {
            return cell->LEFT() < target;
        });

    // Move iterator back by one position if it’s not at the beginning
    if (it != P_Row[row].begin()) {
        --it;
        return std::distance(P_Row[row].begin(), it);  // Return the index of the found element
    } else {
        return -1;  // If all elements are greater than or equal to `d`, return -1
    }
}

BoundCell PLACEROW::Bcell(int row, double d) {
    BoundCell B;
    // cout << "B start" << endl;
    // cout << "size: " << P_Row[row].size() << endl;
    // PrintRow(row);

    auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), d,
                [](const CELL* cell, double target) {
                    return cell->LEFT() < target;  // Compare the LEFT() member function of CELL to `d`
                });
    // cout << "ptr get" << endl;
    auto lptr = uptr-1;

    
    B.Ucell = *uptr;
    B.Lcell = *lptr;
    return B; 
}

// BoundCell PLACEROW::Bcell(int row, CELL* cell) {
//     BoundCell B;
//     // cout << "B start" << endl;
//     auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell->LEFT(),
//                 [](const CELL* c, double value) {
//                 return c->LEFT() < value; });
//     // cout << "ptr get" << endl;
//     auto lptr = uptr-1;
//     B.Ucell = *uptr;
//     B.Lcell = *lptr;
//     return B; 
// }

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

Rptr PLACEROW::U_ptr(int row, double d) {
    auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), d,
                [](const CELL* cell, double target) {
                    return cell->LEFT() < target;  // Compare the LEFT() member function of CELL to `d`
                });
    return uptr; // Return the iterator to the position found
}

Rptr PLACEROW::L_ptr(int row, double d) {
    auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), d,
                [](const CELL* cell, double target) {
                    return cell->LEFT() < target;  // Compare the LEFT() member function of CELL to `d`
                });
    auto lptr = uptr - 1;
    return lptr;
}

bool PLACEROW::Legal(CELL* cell) {
    // cout << "check legal " << cell->GetName() << endl;
    // fix cell y if not on site
    if((double)((cell->DOWN() - ycoor) / height) != (int)((cell->DOWN() - ycoor) / height) ) {
        cout << "Tweak y" << endl;
        double legal_y = ycoor + (double)height * (int)((cell->DOWN() - ycoor) / height);
        cell->SetXY(cell->LEFT(), legal_y);
    }

    // check Die bound
    // cout << "check Die Bound" << endl;
    if(cell->LEFT() < Die.lowerX || cell->RIGHT() > Die.upperX || cell->DOWN() < Die.lowerY || cell->TOP() > Die.upperY) {
        // cout << "Out of Die" << endl;
        return false;
    }
    // check within placement row
    // cout << "check PR Bound" << endl;
    if(cell->LEFT() < xcoor || cell->LEFT() > xcoor + site_num - 1) {
        // cout << "Out of PR" << endl;
        return false;
    }
    // check overlap
    // cout << "check Overlap" << endl;
    for(int row = GetRow(cell->DOWN()); row <= min(GetRow(cell->TOP()), row_num-1); row++) {
        // if(!InBound(row)) cout << "not inbound" << endl;
        // cout << "row: " << row << endl;
        // int Ui = U_index(row, cell->LEFT());
        // int Li = L_index(row, cell->LEFT());
        // cout << Li << " " << Ui << endl;
        
        BoundCell B = Bcell(row, cell->LEFT());
        // CELL* Lcell = P_Row[row][Li];
        // CELL* Ucell = P_Row[row][Ui];
        CELL* Lcell = B.Lcell;
        CELL* Ucell = B.Ucell;

        // cout << Lcell->GetName() << " " << Ucell->GetName() << endl;

        // Check cell upper bound & lower bound
        if(!Lcell->pseudo && Lcell->RIGHT() > cell->LEFT()) {
            // cout << "Overlap with Lcell: " << Lcell->GetName() << endl;
            return false;
        } 
        if(!Ucell->pseudo && Ucell->LEFT() < cell->RIGHT()) {
            // cout << "Overlap with Ucell: " << Ucell->GetName() << endl;
            return false;
        } 
        

    }

    // cout << "check legal" << endl;

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
        BoundCell B = Bcell(row, cell->LEFT());
        bool Umult = (B.Ucell->GetH() > height) || (B.Ucell->Fix());
        bool Lmult = (B.Lcell->GetH() > height) || (B.Lcell->Fix());
        // Check cell upper bound & lower bound
        if(!B.Lcell->pseudo && B.Lcell->RIGHT() > cell->LEFT() && Lmult) return false;
        if(!B.Ucell->pseudo && B.Ucell->LEFT() < cell->RIGHT() && Umult) return false;
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
        // cout << "(" << c->LEFT() << ", " << c->RIGHT() << ") ";
        cout << c->GetName() << " ";
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

void PLACEROW::PrintSpace() {
    cout << "   space_row: ";
    int i = 0;
    for(auto b : space) {
        if(b) i++;
    }
    cout << i << endl;
}

void PLACEROW::GetSPace(CELL* cell) {
    int r = GetRow(cell);
    
}