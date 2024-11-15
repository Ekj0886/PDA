#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

bool PLACEROW::FindVacant(CELL* cell) {
    // cout << "FV" << endl;
    if(Legal(cell)) return true;
    
    int track = GetTrack(cell);
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;
    BoundPtr Bptr = BIndex(row, cell);
    Rptr ub = Bptr.Uptr;
    Rptr lb = Bptr.Lptr;
    if(!(*ub)->pseudo) q.push_back(ub);
    if(!(*lb)->pseudo) q.push_back(lb);

    for(int i = 1; i < track; i++) {
        int Urow = row + i;
        if(Urow < row_num && Urow >= 0) {
            Bptr = BIndex(Urow, cell);
            ub = Bptr.Uptr;
            lb = Bptr.Lptr;
            if(!(*ub)->pseudo) q.push_back(ub);
            if(!(*lb)->pseudo) q.push_back(lb);
        }
        // int Lrow = row - i;
        // if(Lrow < row_num && Lrow >= 0) {
        //     Bptr = BIndex(Lrow, cell);
        //     ub = Bptr.Uptr;
        //     lb = Bptr.Lptr;
        //     if(!(*ub)->pseudo) q.push_back(ub);
        //     if(!(*lb)->pseudo) q.push_back(lb);
        // }
    }

    while(!q.empty()) {
        Rptr rptr = q.front();
        CELL* bcell = *rptr;
        q.pop_front();

        if(Legal(cell)) return true;   

        if(bcell->LEFT() > x_origin) {
            cell->SetXY(bcell->RIGHT(), cell->DOWN());
            rptr++;
        }
        else {                           
            cell->SetXY(bcell->LEFT() - cell->GetW(), cell->DOWN());
            rptr--; 
        }

        bcell = *rptr;
        if(! bcell->pseudo ) q.push_back(rptr);  

    }

    // cout << "Not Found" << endl;

    cell->SetXY(x_origin, y_origin);
    return false;

}

bool PLACEROW::SingleVacant(CELL* cell) {

    if(Legal(cell)) return true;

    // cout << "== Find Vacant" << endl;
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    BoundPtr Bptr = BIndex(row, cell);
    Rptr ub = Bptr.Uptr;
    Rptr lb = Bptr.Lptr;

    if(!(*ub)->pseudo) q.push_back(ub);
    if(!(*lb)->pseudo) q.push_back(lb);


    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();

        if(Legal(cell)) return true;   

        if((*rptr)->LEFT() > x_origin) {
            cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            rptr++;
        }
        else {                           
            cell->SetXY((*rptr)->LEFT() - cell->GetW(), cell->DOWN());
            rptr--; 
        }

        if(! (*rptr)->pseudo ) q.push_back(rptr);  

    }

    cell->SetXY(x_origin, y_origin);
    return false;

}

bool PLACEROW::DumbFill(CELL* cell) {
    
    // PrintSpace();
    if(dumb_row == row_num - 1) dumb_row = 0;
    // cout << "Dumbfill" << endl;
    // int row = GetRow(cell->DOWN());
    // int range = max(row, row_num-row);
    // int new_row; 

    for(int r = dumb_row; r < row_num; r++) {

        // new_row = row + r;
        // new_row = r;

        if(InBound(r) && space[r]) {
            cell->SetXY(cell->LEFT(), ycoor + r*height);
            if(FindVacant(cell)) {
                dumb_row = r;
                return true;
            }
            else {
                space[r] = false;
            }
        }
        
        // new_row = row - r;

        // if(InBound(new_row) ) {
        //     cell->SetXY(cell->LEFT(), ycoor + new_row*height);
        //     if(SingleVacant(cell)) return true;
        // }

    }
    return false;
}

bool PLACEROW::FindSRVacant(CELL* cell) {
    // cout << "== Find Vacant" << endl;
    int track = GetTrack(cell);
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();

    if(SRLegal(cell)) return true;
    
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
    bool right = PushRight(cell);
    bool left  = PushLeft(cell);
    
    if(right) cout << "push right success" << endl;
    else      cout << "push right fail" << endl;
    if(left) cout << "push left success" << endl;
    else      cout << "push left fail" << endl;
    


    // if(PushRight(cell)) {
    //     cout << "push right done" << endl;
    // }
    // else {
    //     cout << "can't push right" << endl;
    // }
    // if(PushLeft(cell)) {
    //     cout << "push left done" << endl;
    // }
    // else {
    //     cout << "can't push left" << endl;
    // }

    if(Legal(cell)) return true;
    else return false;

}