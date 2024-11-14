#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

bool PLACEROW::Overlap(CELL* L_cell, CELL* R_cell) {
    return L_cell->RIGHT() > R_cell->LEFT();
}

// functions defined in header file
bool PLACEROW::PushRight(CELL* cell) {
    cout << "Push Right" << endl;

    RowMem.clear();
    CellMem.clear();
    
    for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {

        Rptr cur = U_ptr(row, cell->LEFT());
        CELL* cur_cell = *cur;

        Rptr nxt = cur + 1;
        CELL* nxt_cell = *nxt; 

        if(!Overlap(cell, cur_cell) || cell->pseudo) continue;

        CellMem[row][cur_cell] = cur_cell->LEFT();
        
        double pos = cell->RIGHT() + cur_cell->GetW();
        Rptr check = U_ptr(row, cell->RIGHT());
        // Rptr trace = cur;

        while((*check)->LEFT() < pos) {
            CELL* c = *check;
            cout << c->GetName() << endl;
            if(c->Fix() || GetTrack(c) > 1) {
                pos = c->RIGHT() + cur_cell->GetW();
                P_Row[row].erase(check);
                P_Row[row].insert(cur, c);
                cur++;
            }
            check++;
        }

        PrintRow(row);
        cur_cell->SetXY(pos - cur_cell->GetW(), cur_cell->DOWN());

        while(Overlap(cur_cell, nxt_cell)) {
            // cout << cur_cell->GetName() << " " << nxt_cell->GetName() << endl;
            // cout << "(" << cur_cell->LEFT() << ", " << cur_cell->RIGHT() << ")" << "(" << nxt_cell->LEFT() << ", " << nxt_cell->RIGHT() << ")" << endl;  
            if(nxt_cell->pseudo) {
                GoBack(cell);
                return false;
            }
            if(nxt_cell->Fix() || GetTrack(nxt_cell) > 1) {
                CellMem[row][cur_cell] = cur_cell->LEFT();
                cur_cell->SetXY(nxt_cell->RIGHT(), cur_cell->DOWN());
                RowMem[row][cur] = cur_cell;
                RowMem[row][nxt] = nxt_cell;
                std::swap(*cur, *nxt);
            } 
            else{
                // cout << "Don't swap" << endl;
                nxt_cell->SetXY(cur_cell->RIGHT(), nxt_cell->DOWN());
            }

            cur = nxt;
            cur_cell = *cur;
            nxt = cur + 1;
            nxt_cell = *nxt; 
        }
        PrintRow(row);
    
    }
    
    return true;

}

bool PLACEROW::PushLeft(CELL* cell) {
    cout << "Push Left" << endl;

    RowMem.clear();
    CellMem.clear();
    
    for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {

        Rptr cur = L_ptr(row, cell->LEFT());
        CELL* cur_cell = *cur;

        Rptr nxt = cur - 1;
        CELL* nxt_cell = *nxt; 

        if(!Overlap(cur_cell, cell) || cell->pseudo) continue;

        CellMem[row][cur_cell] = cur_cell->LEFT();
        
        double pos = cell->LEFT() - cur_cell->GetW();
        Rptr check = L_ptr(row, cur_cell->LEFT());
        // Rptr trace = cur;

        while((*check)->RIGHT() > pos) {
            CELL* c = *check;
            cout << c->GetName() << endl;
            if(c->Fix() || GetTrack(c) > 1) {
                pos = c->LEFT() - cur_cell->GetW();
                P_Row[row].erase(check);
                P_Row[row].insert(cur, c);
                cur--;
            }
            check--;
        }

        PrintRow(row);
        cur_cell->SetXY(pos, cur_cell->DOWN());

        while(Overlap(nxt_cell, cur_cell)) {
            // cout << cur_cell->GetName() << " " << nxt_cell->GetName() << endl;
            // cout << "(" << cur_cell->LEFT() << ", " << cur_cell->RIGHT() << ")" << "(" << nxt_cell->LEFT() << ", " << nxt_cell->RIGHT() << ")" << endl;  
            if(nxt_cell->pseudo) {
                GoBack(cell);
                return false;
            }
            if(nxt_cell->Fix() || GetTrack(nxt_cell) > 1) {
                // cout << "Swap" << endl;
                CellMem[row][cur_cell] = cur_cell->LEFT();
                cur_cell->SetXY(nxt_cell->LEFT() - cell->GetW(), cur_cell->DOWN());
                RowMem[row][cur] = cur_cell;
                RowMem[row][nxt] = nxt_cell;
                std::swap(*cur, *nxt);
            } 
            else{
                // cout << "Don't swap" << endl;
                nxt_cell->SetXY(cur_cell->LEFT() - cur_cell->GetW(), nxt_cell->DOWN());
            }

            cur = nxt;
            cur_cell = *cur;
            nxt = cur - 1;
            nxt_cell = *nxt; 
        }
    
    }

    return true;

}


void PLACEROW::GoBack(CELL* cell) {

    for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {
        for(auto& p : RowMem[row]) {
            Rptr ptr = p.F;
            CELL* origin = p.S;
            P_Row[row].emplace(ptr, origin);
        }
        for(auto& p : CellMem[row]) {
            CELL* mcell = p.F;
            double origin_x = p.S;
            mcell->SetXY(origin_x, mcell->DOWN());
        }
    }

}