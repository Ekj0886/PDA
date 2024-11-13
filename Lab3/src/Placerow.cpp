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

// BoundCell PLACEROW::Bcell(int row, CELL* cell) {
//     BoundCell B;
//     cout << "Start B" << endl;
//     auto uptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell->LEFT(),
//         [](const CELL* c, double value) {
//             return c->LEFT() < value;
//         });

//     auto lptr = (uptr == P_Row[row].begin()) ? uptr : uptr - 1;

//     B.Ucell = (uptr == P_Row[row].end()) ? nullptr : *uptr;
//     B.Lcell = *lptr;
//     cout << "End Bcell" << endl;
//     return B;
// }


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



void PLACEROW::Init(int row, int site, double h, double x, double y) {
    row_num = row;
    site_num = site;
    height = h;
    xcoor = x;
    ycoor = y;

    P_Row.resize(row);
    space.resize(row);
    CELL* pseudo_cell_left = new CELL("L", xcoor-1, 0, 1, 0, 1);
    CELL* pseudo_cell_right = new CELL("R", xcoor + site_num - 1, 0, 1, 0, 1);
    pseudo_cell_left->pseudo = true; 
    pseudo_cell_right->pseudo = true;
    for(int r = 0; r < row; r++) {
        P_Row[r].push_back(pseudo_cell_left);
        P_Row[r].push_back(pseudo_cell_right);
        space[r] = site_num;
    }
}


void PLACEROW::Insert(CELL* cell) {

    for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {
        auto vptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell,
            [](CELL* a, CELL* b) {
                return a->LEFT() < b->LEFT();
            });
        
        P_Row[row].insert(vptr, cell);
    }

}

void PLACEROW::Remove(CELL* cell) {
    if(cell->Fix()) {
        cout << "Cell fixed, can't remove" << endl;
        return;
    }

    for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {
        auto vptr = std::lower_bound(P_Row[row].begin(), P_Row[row].end(), cell,
            [](CELL* a, CELL* b) {
                return a->LEFT() < b->LEFT();
            });
        
        P_Row[row].erase(vptr);
    }
}


bool PLACEROW::FindVacant(CELL* cell) {
    
    // cout << "Load Vacant" << endl;

    int track = GetTrack(cell);
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    BoundPtr Bptr = BIndex(row, cell);
    Rptr ub = Bptr.Uptr;
    Rptr lb = Bptr.Lptr;

    q.push_back(ub);
    q.push_back(lb);

    for(int i = 1; i <= track; i++) {
        int Urow = row + i;
        int Lrow = row - i;

        if(Urow < row_num && Urow >= 0) {
            Bptr = BIndex(Urow, cell);
            ub = Bptr.Uptr;
            lb = Bptr.Lptr;
            q.push_back(ub);
            q.push_back(lb);
        }

        if(Lrow >= 0 && Lrow < row_num) {
            Bptr = BIndex(Lrow, cell);
            ub = Bptr.Uptr;
            lb = Bptr.Lptr;
            q.push_back(ub);
            q.push_back(lb);
        }
    }

    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();

        // cout << "Name: " << endl;
        // cout << (*rptr)->GetName() << endl;

        if((*rptr)->GetName() != "R") {

            if((*rptr)->LEFT() > x_origin) cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            else                           cell->SetXY((*rptr)->LEFT()-cell->GetW(), cell->DOWN());
            
            if(Legal(cell)) {
                // cout << "Found" << endl;
                return true;
            }
            else if(!(*rptr)->pseudo){
                // cout << "check" << " " << (*rptr)->pseudo << endl;
                if(cell->LEFT() > x_origin) rptr++;
                else rptr = rptr - 1;
                q.push_back(rptr);   
            }

        }
    }

    // cout << "Not Found" << endl;

    cell->SetXY(x_origin, y_origin);
    return false;

}

bool PLACEROW::SingleVacant(CELL* cell) {
    // cout << "== Find Vacant" << endl;
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    BoundPtr Bptr = BIndex(row, cell);
    Rptr ub = Bptr.Uptr;
    Rptr lb = Bptr.Lptr;

    q.push_back(ub);
    q.push_back(lb);


    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();
        
        if((*rptr)->GetName() != "R") {

            if((*rptr)->LEFT() > x_origin) cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            else                           cell->SetXY((*rptr)->LEFT()-cell->GetW(), cell->DOWN());
            
            if(Legal(cell)) {
                return true;
            }
            else if(!(*rptr)->pseudo){
                if(cell->LEFT() > x_origin) rptr++;
                else rptr = prev(rptr);
                q.push_back(rptr);   
            }

        }
    }

    cell->SetXY(x_origin, y_origin);
    return false;

}

bool PLACEROW::InBound(int row) {
    return (row >= 0 && row < row_num);
}

bool PLACEROW::DumbFill(CELL* cell) {
    // cout << "Dumbfill" << endl;
    int row = GetRow(cell->DOWN());
    int range = max(row, row_num-row);
    int new_row; 

    for(int r = GetTrack(cell); r < range; r += 2*GetTrack(cell)) {

        new_row = row + r;

        if( space[r] > 10*cell->GetW() && InBound(new_row) ) {
            cell->SetXY(cell->LEFT(), ycoor + new_row*height);
            if(SingleVacant(cell)) return true;
        }
        
        new_row = row - r;

        if( space[r] > 10*cell->GetW() && InBound(new_row) ) {
            cell->SetXY(cell->LEFT(), ycoor + new_row*height);
            if(SingleVacant(cell)) return true;
        }

    }
    return false;
}

bool PLACEROW::FindSRVacant(CELL* cell) {
    // cout << "== Find Vacant" << endl;
    int track = GetTrack(cell);
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    BoundPtr Bptr = BIndex(row, cell);
    Rptr ub = Bptr.Uptr;
    Rptr lb = Bptr.Lptr;

    q.push_back(ub);
    q.push_back(lb);

    for(int i = 1; i <= track; i++) {
        int Urow = row + i;
        int Lrow = row - i;

        if(Urow < row_num && Urow >= 0) {
            Bptr = BIndex(Urow, cell);
            ub = Bptr.Uptr;
            lb = Bptr.Lptr;
            q.push_back(ub);
            q.push_back(lb);
        }

        if(Lrow >= 0 && Lrow < row_num) {
            Bptr = BIndex(Lrow, cell);
            ub = Bptr.Uptr;
            lb = Bptr.Lptr;
            q.push_back(ub);
            q.push_back(lb);
        }
    }


    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();
        
        if((*rptr)->GetName() != "R") {

            if((*rptr)->LEFT() > x_origin) cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            else                           cell->SetXY((*rptr)->LEFT()-cell->GetW(), cell->DOWN());
            
            if(SRLegal(cell)) return true;
            else if(!(*rptr)->pseudo){
                if(cell->LEFT() > x_origin) rptr++;
                else rptr = prev(rptr);
                q.push_back(rptr);   
            }

        }
    }
    cell->SetXY(x_origin, y_origin);
    return false;

}

bool PLACEROW::Legalize(CELL* cell) {
    
//     SRcellmap.clear();
//     SRcell.clear();
//     x.clear();

//     SRcell.resize(GetTrack(cell)); 
//     x.resize(GetTrack(cell)); // Row x coor indicator
    
// //  Right hand side legalization

//     // load SRcell information
//     for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {
        
//         x[row] = cell->RIGHT();
//         Rptr ub = P_Row[row]->lowerbound(cell);
//         CELL* ucell = (*ub);
//         while(!ucell->pseudo) {
//             if(!ucell->Fix() && GetTrack(ucell) == 1) {
//                 SRcellmap[ucell] = ucell->LEFT();
//                 SRcell[row].push_back(ucell);
//             }
//             ub++;
//             ucell = (*ub);
//         }

//     }

//     // start legalize
//     for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {

//         while(!SRcell[row].empty()) {

//             CELL* ucell = SRcell.front();
//             SRcell.pop_front();

//             ucell->SetXY(x[row], ucell->DOWN());

//             if(Legal(ucell)) {
                
//             }

//         }

//     }
    return false;
}

// WINDOW PLACEROW::SetWindow(CELL* cell) {
//     int num = 3;
//     WINDOW W;
//     W.xs = max( min(cell->LEFT() - num * cell->GetW(), RIGHT()), LEFT());
//     W.xe = max( min(cell->RIGHT() + num * cell->GetW(), RIGHT()), LEFT());
//     W.rs = max( min(GetRow(cell->DOWN()) - GetTrack(cell), row_num-1), 0);
//     W.re = max( min(GetRow(cell->TOP()) + GetTrack(cell) - 1, row_num-1), 0);
//     return W;
// }

// void PLACEROW::FindSegment(WINDOW W) {

//     // cout << "Window Height: " << W.GetTrack() << endl;

//     Xseg.resize(W.GetTrack(), W.xs);

//     for(int row = W.rs; row <= W.re; row++) {

//         auto xs = Xs(row, W.xs);
//         auto xe = Xe(row, W.xe);

//         for (auto c = *xs; c != *xe; ++c) {
//             CELL* cell = *c;

//         }   

//     }

// }


