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
    space.resize(row);
    CELL* pseudo_cell_left = new CELL("L", Die.lowerX-1, 0, 1, 0, 1);
    CELL* pseudo_cell_right = new CELL("R", Die.upperX, 0, 1, 0, 1);
    pseudo_cell_left->pseudo = true; 
    pseudo_cell_right->pseudo = true;
    for(int r = 0; r < row; r++) {
        placement_row[r] = new set<CELL*, CompareByX>();
        RowSet(r)->insert(pseudo_cell_left);
        RowSet(r)->insert(pseudo_cell_right);
        space[r] = site_num;
    }
}

bool PLACEROW::FindVacant(CELL* cell) {
    // cout << "== Find Vacant" << endl;
    int track = GetTrack(cell);
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    Rptr ub = RowSet(row)->lower_bound(cell);
    Rptr lb = prev(ub);

    q.push_back(ub);
    q.push_back(lb);

    for(int i = 1; i <= track; i++) {
        int Urow = row + i;
        if(Urow < row_num && Urow >= 0) {
            Rptr u = RowSet(Urow)->lower_bound(cell);
            Rptr l = prev(u);
            q.push_back(u);
            q.push_back(l);
        }
    }

    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();

        // cout << (*rptr)->GetName() << endl;
        
        if((*rptr)->GetName() != "R") {

            if((*rptr)->LEFT() > x_origin) cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            else cell->SetXY((*rptr)->LEFT()-cell->GetW(), cell->DOWN());
            // cout << "set " << cell->LEFT() << endl;
            if(Legal(cell)) return true;
            
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

bool PLACEROW::SingleVacant(CELL* cell) {
    // cout << "== Find Vacant" << endl;
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    Rptr ub = RowSet(row)->lower_bound(cell);
    Rptr lb = prev(ub);

    q.push_back(ub);
    q.push_back(lb);


    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();
        
        if((*rptr)->GetName() != "R") {
            if((*rptr)->LEFT() > x_origin) cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            else cell->SetXY((*rptr)->LEFT()-cell->GetW(), cell->DOWN());
            if(Legal(cell)) return true;
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

    double origin_x = cell->LEFT();
    double origin_y = cell->DOWN();

    int row = GetRow(cell->DOWN());
    int range = max(row, row_num-row);
    int new_row; 

    for(int r = GetTrack(cell); r < range; r += 1) {

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

    cell->SetXY(origin_x, origin_y);

    return false;
}

bool PLACEROW::FindSRVacant(CELL* cell) {
    // cout << "== Find SRVacant" << endl;

    if(SRLegal(cell)) return true;

    int track = GetTrack(cell);
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    Rptr ub = RowSet(row)->lower_bound(cell);
    Rptr lb = prev(ub);

    q.push_back(ub);
    q.push_back(lb);

    for(int i = 1; i <= track; i++) {
        int Urow = row + i;

        if(Urow < row_num && Urow >= 0) {
            Rptr u = RowSet(Urow)->lower_bound(cell);
            Rptr l = prev(u);
            q.push_back(u);
            q.push_back(l);
        }
    }

    // cout << "start find" << endl;

    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();

        if(SRLegal(cell)) return true;
        
        if((*rptr)->GetName() != "R") {
            if((*rptr)->LEFT() > x_origin) cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            else cell->SetXY((*rptr)->LEFT()-cell->GetW(), cell->DOWN());

            if(SRLegal(cell)) {
                distance += ( abs(cell->LEFT() - x_origin) + abs(cell->DOWN() - y_origin) );
                return true; 
            }

            else if(!(*rptr)->pseudo){
                if(cell->LEFT() > x_origin) {
                    if(!(*(++rptr))->pseudo) {
                        q.push_back(rptr);   
                    }
                }
                else {
                    if(!(*(prev(rptr)))->pseudo) {
                        rptr = prev(rptr);
                        q.push_back(rptr);  
                    }
                }
                
            }

        }
        // cout << "nxt" << endl;
    }
    cell->SetXY(x_origin, y_origin);
    // cout << "Not found" << endl;
    return false;

}

bool PLACEROW::Legalize(CELL* cell) {   
    
    CellMem.clear();
    overlap.clear();

    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {

        int row = GetRow(y);
        Rptr ub = RowSet(row)->lower_bound(cell);
        Rptr lb = prev(ub);

        // check lowerbound
        CELL* lb_cell = (*lb);
        if(Overlap(cell, lb_cell)) {
            overlap.push_back(lb_cell);
            Remove(lb_cell);
        }
        // check upper bound
        CELL* ub_cell = (*ub);
        while(Overlap(cell, ub_cell)) {
            overlap.push_back(ub_cell);
            ub++;
            Remove(ub_cell);
            ub_cell = (*ub);
        }

        y += height;
    
    }
    
    Insert(cell);
    
    while(!overlap.empty()) {

        CELL* c = overlap.front();
        overlap.pop_front();

        double origin_x = c->LEFT();
        double origin_y = c->DOWN();
        
        if(!DumbFill(c)) {
            Remove(cell);
            Restore();
            return false;
        }
        else {
            CellMem[c] = make_pair(origin_x, origin_y);
            Insert(c);
            distance += ( abs(c->LEFT() - origin_x) + abs(c->DOWN() - origin_y) );

            if(distance >= (Die.upperX - Die.lowerX + Die.upperY - Die.lowerY) / 10) {
                Remove(cell);
                Restore();
                return false;
            }
            
        }

    }

    return true;

}

void PLACEROW::Restore() {
    
    // restore modified cell
    for (auto& cptr : CellMem) {
        CELL* c = cptr.F;
        Remove(c); 
        c->SetXY(cptr.S.F, cptr.S.S);
        Insert(c);
    }

    // restore deleted cell
    for(auto c : overlap) {
        Insert(c);
    }

}

