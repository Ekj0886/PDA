#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

bool PLACEROW::H_overlap(CELL* c1, CELL* c2) {
    return c1->LEFT() <= c2->RIGHT() && c1->RIGHT() >= c2->LEFT();
}

bool PLACEROW::V_overlap(CELL* c1, CELL* c2) {
    return c1->DOWN() <= c2->TOP() && c1->TOP() >= c2->DOWN();
}

bool PLACEROW::isBlk(CELL* cell) {
    if(cell->Fix() || cell->TOP() > UB || cell->DOWN() < LB) return true;
    else                                                     return false;
}

void PLACEROW::PrintX() {
    cout << "x: " << endl;
    for(int i = LR; i <= UR; i++) {
        cout << i << ": " << x[i] << " | ";
    }cout << endl;
}

void PLACEROW::PrintPTR() {
    cout << endl << "PrintPTR " << PTR.size() << endl;
    for(auto c : PTR) {
        cout << c->GetName() << " ";
        // else cout << "NULL ";
    }cout << endl << endl;
}

bool PLACEROW::CheckPTR() {
    // cout << "Check" << endl;
    for(auto c : PTR) if(c != EMPTY) return true;
    return false;
}

void PLACEROW::UpdateRX(CELL* cell) {
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);
        x[row-LR] = cell->RIGHT();
        y += height;
    }
}


bool PLACEROW::FindR(CELL* cell) {
    // cout << "== Find Vacant" << endl;

    if(SRLegal(cell)) return true;

    // check Die bound
    if(cell->LEFT() < Die.lowerX || cell->RIGHT() > Die.upperX || cell->DOWN() < Die.lowerY || cell->TOP() > Die.upperY) {
        cout << "   Outside of Die" << endl;
        return false;
    }
    // check within placement row
    if(cell->LEFT() < xcoor || cell->LEFT() > xcoor + site_num - 1) {
        cout << "   Outside of Placement Row" << endl;
        return false;
    }
    int track = GetTrack(cell);
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    Rptr ub = RowSet(row)->lower_bound(cell);

    q.push_back(ub);

    for(int i = 1; i <= track; i++) {
        int Urow = row + i;

        if(Urow < row_num && Urow >= 0) {
            Rptr u = RowSet(Urow)->lower_bound(cell);
            q.push_back(u);
        }
    }

    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();
        
        if((*rptr)->GetName() != "R") {
            cell->SetXY((*rptr)->RIGHT(), cell->DOWN());
            if(SRLegal(cell)) return true;
            else if(!(*rptr)->pseudo){
                rptr++;
                q.push_back(rptr);   
            }
        }
    }
    cell->SetXY(x_origin, y_origin);
    return false;

}


double PLACEROW::CellRX(CELL* cell) {
    double max_x = xcoor;
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);
        max_x = max(max_x, x[row]);
        y += height;
    }
    return max_x;
}

bool PLACEROW::InitR(CELL* cell) {

    cout << "Initialize" << endl;

    CellMem.clear();
    
    // set bound
    UB = cell->DOWN() + (2 * GetTrack(cell) - 1) * height;
    LB = cell->DOWN() - (GetTrack(cell) - 1) * height;
    UB = min( max(UB, ycoor), Die.upperY ); 
    LB = min( max(LB, ycoor), Die.upperY ); 
    
    int size = (UB - LB) / height;

    LR = GetRow(LB);
    UR = LR + size - 1;

    x.resize(size, cell->LEFT());
    PTR.resize(size);

    // cout << cell->LEFT() << " " << cell->RIGHT() << endl;

    // Set overlap first cell position & x
    for(int row = LR; row <= UR; row++) {
        // check overlap with merge_cell
        Rptr ub = RowSet(row)->lower_bound(cell);
        CELL* ucell = *ub;
        PTR[row] = (*ub);
        // cout << "row: " << row << " " << ucell->GetName() << endl;
        if(!ucell->pseudo && H_overlap(cell, ucell) && V_overlap(cell, ucell)) {
            cout << "*Overlap " << ucell->GetName() << endl;
            CellMem[ucell] = ucell->LEFT();
            ub++;
            PTR[row] = (*ub);
            Remove(ucell);
            ucell->SetXY(max(x[row], cell->RIGHT()), ucell->DOWN());


            if(!SRLegal(ucell)) {
                if(!FindR(ucell)) return false;
            } 

            // cout << "*SetX " << ucell->LEFT() << endl; 
            UpdateRX(ucell); 
            // PrintX();
            
        }

    }


    cout << "== Init R succeed" <<endl << endl;
    PrintX();
    
    PrintPTR();
    
    cout << "CellMem " << CellMem.size() << endl;
    for (auto& cptr : CellMem) {
        CELL* c = cptr.F;
        cout << c->GetName() << " ";
        // else cout << "NULL ";
    }cout << endl << endl;
    
    
    return true;
    // PTR.resize(size);

}

bool PLACEROW::PushRight() {

    cout << "== Push Right" << endl;

    while(CheckPTR()) {
        for(int row = 0; row <= UR-LR; row++) {

            CELL* cell = PTR[row];
            if(cell == nullptr || isBlk(cell) || cell->pseudo) {
                cout << "   skip " << cell->GetName() << endl;
                PTR[row] = EMPTY;
                continue;
            }
            cout << "       push " << cell->GetName() << endl;
            CellMem[cell] = cell->LEFT();
            Remove(cell);
            cell->SetXY(CellRX(cell), cell->DOWN());
            if(!FindR(cell)) return false;
            UpdateRX(cell);
            PTR[row] = *( RowSet(row)->lower_bound(cell) );
            // cout << PTR[row]->GetName() << endl;

        }

        // PrintPTR();

    }


    return true;

}

void PLACEROW::DumpMem() {
    for (auto& cptr : CellMem) {
        CELL* c = cptr.F;
        Insert(c);
    }
}

void PLACEROW::Restore() {
    for (auto& cptr : CellMem) {
        CELL* c = cptr.F;
        c->SetXY(cptr.S, c->DOWN());
        Insert(c);
    }
}


