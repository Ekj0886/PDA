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
    CELL* pseudo_cell_left = new CELL("L", xcoor-1, 0, 1, 0, 1);
    CELL* pseudo_cell_right = new CELL("R", xcoor + site_num - 1, 0, 1, 0, 1);
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
        // int Lrow = row - i;

        if(Urow < row_num && Urow >= 0) {
            Rptr u = RowSet(Urow)->lower_bound(cell);
            Rptr l = prev(u);
            q.push_back(u);
            q.push_back(l);
        }

        // if(Lrow >= 0 && Lrow < row_num) {
        //     Rptr u = RowSet(Lrow)->lower_bound(cell);
        //     Rptr l = prev(u);
        //     q.push_back(u);
        //     q.push_back(l);
        // }
    }


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
    // cout << "Dumbfill" << endl;
    int row = GetRow(cell->DOWN());
    int range = max(row, row_num-row);
    int new_row; 

    for(int r = GetTrack(cell); r < range; r += 1) {
    // for(int r = GetTrack(cell); r < range; r += 2*GetTrack(cell)) {

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

    Rptr ub = RowSet(row)->lower_bound(cell);
    Rptr lb = prev(ub);

    q.push_back(ub);
    q.push_back(lb);

    for(int i = 1; i <= track; i++) {
        int Urow = row + i;
        int Lrow = row - i;

        if(Urow < row_num && Urow >= 0) {
            Rptr u = RowSet(Urow)->lower_bound(cell);
            Rptr l = prev(u);
            q.push_back(u);
            q.push_back(l);
        }

        if(Lrow >= 0 && Lrow < row_num) {
            Rptr u = RowSet(Lrow)->lower_bound(cell);
            Rptr l = prev(u);
            q.push_back(u);
            q.push_back(l);
        }
    }


    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();
        
        if((*rptr)->GetName() != "R") {
            if((*rptr)->LEFT() > x_origin) cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            else cell->SetXY((*rptr)->LEFT()-cell->GetW(), cell->DOWN());
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
//     for(int row = GetRow(cell->DOWN()); row <= GetRow(cell->TOP()); row++) {
        
//         x[row] = cell->RIGHT();
//         Rptr ub = RowSet(row)->lowerbound(cell);
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
//     for(int row = GetRow(cell->DOWN()); row <= GetRow(cell->TOP()); row++) {

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


