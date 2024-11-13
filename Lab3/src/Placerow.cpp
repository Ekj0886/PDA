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
    
    //  Right hand side legalization
    if(PushRight(cell)) cout << "push right done" << endl;
    
    return false;
}