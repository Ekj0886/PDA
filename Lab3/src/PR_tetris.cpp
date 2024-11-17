#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

// ============================================ //
//                 Helper Function              //
// ============================================ //

void PLACEROW::SetBound(CELL* cell) {
    // set bound
    UB = cell->DOWN() + (2 * GetTrack(cell) - 1) * height;
    LB = cell->DOWN() - (GetTrack(cell) - 1) * height;
    UB = min( max(UB, ycoor), Die.upperY ); 
    LB = min( max(LB, ycoor), Die.upperY ); 
}


bool PLACEROW::isBlk(CELL* cell) {
    if(cell->Fix() || cell->TOP() > UB || cell->DOWN() < LB) return true;
    else                                                     return false;
}

void PLACEROW::PrintX() {
    cout << "<< X >>  | ";
    for(int i = 0; i <= UR-LR; i++) {
        cout << fixed << setprecision(0) << i << ": " << x[i] << " | ";
    }cout << endl;
}

void PLACEROW::PrintPTR() {
    cout << endl << "PrintPTR " << PTR.size() << endl;
    for(auto c : PTR) {
        cout << c->GetName() << " ";
        // else cout << "NULL ";
    }cout << endl << endl;
}

void PLACEROW::PrintCellMem() {
    cout << "CellMem " << CellMem.size() << endl;
    for (auto& cptr : CellMem) {
        CELL* c = cptr.F;
        cout << c->GetName() << " ";
        // else cout << "NULL ";
    }cout << endl << endl;
}

bool PLACEROW::CheckPTR() {
    // cout << "Check" << endl;
    for(auto c : PTR) if(c != EMPTY) return true;
    return false;
}

void PLACEROW::DumpMem() {
    cout << "== Dump memory" << endl;
    for (auto& cptr : CellMem) {
        CELL* c = cptr.F;
        Insert(c);
    }
}



// ============================================ //
//            Push Right function               //
// ============================================ //
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

    Rptr ub = Uptr(row, cell);

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
        max_x = max(max_x, x[row-LR]);
        y += height;
    }
    return max_x;
}

bool PLACEROW::InitR(CELL* cell) {

    cout << endl << "== Initialize R " << endl;

    // CellMem.clear();
    
    int size = (UB - LB) / height;
    // cout << "size: " << size << endl;

    LR = GetRow(LB);
    UR = LR + size - 1;

    x.resize(size, cell->LEFT());
    PTR.resize(size, EMPTY);

    // cout << cell->LEFT() << " " << cell->RIGHT() << endl;

    // Set overlap first cell position & x
    for(int row = LR; row <= UR; row++) {
        // check overlap with merge_cell
        Rptr ub = Uptr(row, cell);
        CELL* ucell = *ub;
        PTR[row-LR] = (*ub);
        // cout << "row: " << row << " " << ucell->GetName() << endl;
        if(!ucell->pseudo && H_overlap(cell, ucell) && V_overlap(cell, ucell) && GetRow(ucell->DOWN()) == row) {
            cout << "*Overlap " << ucell->GetName() << endl;
            if(ucell->Fix()) {
                cout << "   overlap fix cell" << endl;
                // cout << ucell->LEFT() << " " << ucell->RIGHT() << endl;
                // cout << cell->LEFT() << " " << cell->RIGHT() << endl;
                return false;
            }
            CellMem[ucell] = ucell->LEFT();
            ub++;
            while((*ub)->Fix()) ub++;
            PTR[row-LR] = (*ub);
            Remove(ucell);
            ucell->SetXY(max(x[row-LR], cell->RIGHT()), ucell->DOWN());


            if(!SRLegal(ucell)) {
                if(!FindR(ucell)) return false;
            } 

            // cout << "*SetX " << ucell->LEFT() << endl; 
            UpdateRX(ucell); 
            // PrintX();
            
        }

    }


    cout << "== Init R succeed" << endl;
    
    PrintX();
    PrintPTR();
    PrintCellMem();
    
    return true;
    // PTR.resize(size);

}

bool PLACEROW::PushRight() {

    cout << "== Push Right" << endl;

    while(CheckPTR()) {
        for(int row = 0; row <= UR-LR; row++) {

            CELL* cell = PTR[row];
            if(cell == nullptr || isBlk(cell) || cell->pseudo || cell->LEFT() >= CellRX(cell)) {
                // cout << "   skip " << cell->GetName() << endl;
                PTR[row] = EMPTY;
                continue;
            }
            cout << "       push " << cell->GetName() << endl;

            CellMem[cell] = cell->LEFT();

            Remove(cell);
            cell->SetXY(CellRX(cell), cell->DOWN());
            if(!FindR(cell)) return false;
            UpdateRX(cell);

            CELL* nxt_cell = *Uptr(row, cell);
            PTR[row] = nxt_cell;
            cout << cell->LEFT() << " " << nxt_cell->GetName() << endl;

        }

        // PrintPTR();

    }


    return true;

}


// ============================================ //
//            Push Left function               //
// ============================================ //
void PLACEROW::UpdateLX(CELL* cell) {
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);
        x[row-LR] = cell->LEFT();
        y += height;
    }
}


bool PLACEROW::FindL(CELL* cell) {
    // cout << "== Find Vacant" << endl;
    // cout << "FindL" << endl;

    if(SRLegal(cell)) return true;

    // check Die bound
    if(cell->LEFT() < Die.lowerX || cell->RIGHT() > Die.upperX || cell->DOWN() < Die.lowerY || cell->TOP() > Die.upperY) {
        // cout << cell->GetName() << " " << cell->LEFT() << " " << cell->DOWN() << " " << cell->TOP() << " " << cell->RIGHT() << endl;
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

    Rptr lb = Lptr(row, cell);

    q.push_back(lb);

    for(int i = 1; i <= track; i++) {
        int Urow = row + i;

        if(Urow < row_num && Urow >= 0) {
            Rptr l = Lptr(row, cell);
            q.push_back(l);
        }
    }

    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();
        
        // if((*rptr)->GetName() != "R") {
            cell->SetXY((*rptr)->LEFT() - cell->GetW(), cell->DOWN());
            if(SRLegal(cell)) return true;
            else if(!(*rptr)->pseudo){
                rptr = prev(rptr);
                q.push_back(rptr);   
            }
        // }
    }
    cell->SetXY(x_origin, y_origin);
    return false;

}


double PLACEROW::CellLX(CELL* cell) {
    double min_x = RIGHT();
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);
        min_x = min(min_x, x[row-LR]);
        y += height;
    }
    return min_x - cell->GetW();
}

bool PLACEROW::InitL(CELL* cell) {

    cout << endl << "== Initialize L" << endl;

    // CellMem.clear();
    
    int size = (UB - LB) / height;

    LR = GetRow(LB);
    UR = LR + size - 1;

    x.resize(size, cell->LEFT());
    PTR.resize(size, EMPTY);

    // cout << cell->LEFT() << " " << cell->RIGHT() << endl;

    // Set overlap first cell position & x
    for(int row = LR; row <= UR; row++) {
        // check overlap with merge_cell
        Rptr lb = Lptr(row, cell);
        CELL* lcell = *lb;
        PTR[row-LR] = (*lb);
        // if(!lcell->Fix() && !lcell->pseudo) CellMem[lcell] = lcell->LEFT();
        // cout << "row: " << row << " " << ucell->GetName() << endl;
        if(!lcell->pseudo && H_overlap(cell, lcell) && V_overlap(cell, lcell) && GetRow(lcell->DOWN()) == row) {
            cout << "*Overlap " << lcell->GetName() << endl;
            cout << lcell->GetName() << " " << lcell->LEFT() << " " << lcell->DOWN() << " " << lcell->TOP() << " " << lcell->RIGHT() << endl;
            if(lcell->Fix()) {
                cout << "   overlap fix cell" << endl;
                // cout << lcell->LEFT() << " " << lcell->RIGHT() << endl;
                // cout << cell->LEFT() << " " << cell->RIGHT() << endl;
                return false;
            }
            CellMem[lcell] = lcell->LEFT();
            lb = prev(lb);
            while((*lb)->Fix()) lb= prev(lb);
            PTR[row-LR] = (*lb);
            Remove(lcell);
            lcell->SetXY(min(x[row-LR], cell->LEFT()) - lcell->GetW(), lcell->DOWN());


            if(!SRLegal(lcell)) {
                if(!FindL(lcell)) return false;
            } 

            // cout << "*SetX " << lcell->LEFT() << endl; 
            UpdateLX(lcell); 
            // PrintX();
            
        }

    }


    cout << "== Init L succeed" <<endl << endl;
    
    PrintX();
    PrintPTR();
    PrintCellMem();
    
    return true;
    // PTR.resize(size);

}

bool PLACEROW::PushLeft() {

    cout << "== Push Left" << endl;

    while(CheckPTR()) {
        for(int row = 0; row <= UR-LR; row++) {

            CELL* cell = PTR[row];
            if(cell == nullptr || isBlk(cell) || cell->pseudo || cell->LEFT() <= CellLX(cell)) {
                // cout << "   skip " << cell->GetName() << endl;
                PTR[row] = EMPTY;
                continue;
            }
            cout << "       push " << cell->GetName() << endl;
            CellMem[cell] = cell->LEFT();
            Remove(cell);
            cell->SetXY(CellLX(cell), cell->DOWN());
            if(!FindL(cell)) return false;
            UpdateLX(cell);

            CELL* nxt_cell = *Lptr(row, cell);
            PTR[row] = nxt_cell;
            // cout << cell->LEFT() << " " << nxt_cell->GetName() << endl;

        }

        // PrintPTR();

    }


    return true;

}



