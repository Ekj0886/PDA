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
    
    // load search list
    Search.clear();
    int start_row = GetRow(cell->DOWN());
    int end_row   = min( (int)ceil(cell->GetH() / height) + start_row - 1, row_num - 1);

    for(int row = start_row; row <= end_row; row++) {
        
        for(const CELL* c : *RowSet(row)) {
            for(int y = start_row; y <= end_row; y++) {
                
                double y_point = ycoor + y*height;

                PT lpt(c->LEFT() - cell->GetW(), y_point);
                lpt.dis = abs(lpt.x - origin_x) + abs(lpt.y - origin_y);    
                Search.insert(lpt);

                PT rpt(c->RIGHT(), y_point);
                rpt.dis = abs(rpt.x - origin_x) + abs(rpt.y - origin_y);    
                Search.insert(rpt);
            }
        }

    } 

    for(const PT p : Search) {
        cell->SetXY(p.x, p.y);
        if(Legal(cell)) return true;
    }

    cell->SetXY(origin_x, origin_y);
    return false;

}

bool PLACEROW::SingleVacant(CELL* cell) {
    
    // load search list
    Search.clear();
    int start_row = GetRow(cell->DOWN());

    int row = start_row;
    for(const CELL* c : *RowSet(row)) {
        PT lpt(c->LEFT() - cell->GetW(), cell->DOWN());
        lpt.dis = abs(lpt.x - origin_x) + abs(lpt.y - origin_y);    
        Search.insert(lpt);

        PT rpt(c->RIGHT(), cell->DOWN());
        rpt.dis = abs(rpt.x - origin_x) + abs(rpt.y - origin_y);    
        Search.insert(rpt);
    }
   
    for(const PT p : Search) {
        cell->SetXY(p.x, p.y);
        if(Legal(cell)) return true;
    }

    cell->SetXY(origin_x, origin_y);
    return false;

}

bool PLACEROW::DumbFill(CELL* cell) {

    int row = GetRow(cell->DOWN());
    int range = max(row, row_num-row);
    int new_row; 

    for(int r = GetTrack(cell); r < range; r += 1) {

        new_row = row + r;

        if( InBound(new_row) ) {
            cell->SetXY(cell->LEFT(), ycoor + new_row*height);
            if(SingleVacant(cell)) return true;
        }
        
        new_row = row - r;

        if( InBound(new_row) ) {
            cell->SetXY(cell->LEFT(), ycoor + new_row*height);
            if(SingleVacant(cell)) return true;
        }

    }

    cell->SetXY(origin_x, origin_y);

    return false;
}

bool PLACEROW::FindSRVacant(CELL* cell) {
    // load search list
    Search.clear();
    int start_row = GetRow(cell->DOWN());
    int end_row   = min( (int)ceil(cell->GetH() / height) + start_row - 1, row_num - 1);

    for(int row = start_row; row <= end_row; row++) {
        
        for(const CELL* c : *RowSet(row)) {
            if(!c->Fix()) continue;
            for(int y = start_row; y <= end_row; y++) {
                
                double y_point = ycoor + y*height;

                PT lpt(c->LEFT() - cell->GetW(), y_point);
                lpt.dis = abs(lpt.x - origin_x) + abs(lpt.y - origin_y);    
                Search.insert(lpt);

                PT rpt(c->RIGHT(), y_point);
                rpt.dis = abs(rpt.x - origin_x) + abs(rpt.y - origin_y);    
                Search.insert(rpt);
            }
        }

    } 

    for(const PT p : Search) {
        cell->SetXY(p.x, p.y);
        if(SRLegal(cell)) {
            return true;
        }
    }

    cell->SetXY(origin_x, origin_y);
    return false;

}

bool PLACEROW::Legalize(CELL* cell) {   
    CellMem.clear();
    overlap.clear();
    overlap_check.clear();

    if(Legal(cell)) {
        Insert(cell);
        return true;
    }
    distance += cell->DIFF();
    

    int start_row = GetRow(cell->DOWN());
    int end_row   = min( (int)ceil(cell->GetH() / height) + start_row - 1, row_num - 1);
    
    for(int row = start_row; row <= end_row; row++) {

        for(CELL* c : *RowSet(row)) {
            if(Overlap(cell, c) && overlap_check.find(c) == overlap_check.end()) {
                if(c->Fix()) return false; // overlap with fix cell
                overlap_check.insert(c);
                overlap.push_back(c);
            }
        }

    }
    
    for(CELL* rmc : overlap) {
        Remove(rmc);
    }

    Insert(cell);
    
    
    while(!overlap.empty()) {

        CELL* c = overlap.front();

        double xo = c->LEFT();
        double yo = c->DOWN();
        
        if(distance > (Die.upperX - Die.lowerX) / 10) {
            Remove(cell);
            Restore();
            return false;
        } 

        if(FastVacant(c)) {
            Insert(c);
            Mem mem(c, xo, yo);
            CellMem.push_back(mem);
            distance += c->DIFF();
            overlap.pop_front();
        }
        else {
            Remove(cell);
            Restore();
            return false;
        }

    }
    
    return true;

}

void PLACEROW::Restore() {
    for (auto mem : CellMem) {
        CELL* c = mem.c;
        Remove(c); 
        c->SetXY(mem.xo, mem.yo);
        Insert(c);
    }

    // restore deleted cell
    for(auto c : overlap) {
        Insert(c);
    }

}

bool PLACEROW::FastVacant(CELL* cell) {

    double xo = cell->LEFT(), yo = cell->DOWN();

    Search.clear();
    int start_row = GetRow(cell->DOWN());

    int row = start_row;
    for(const CELL* c : *RowSet(row)) {
        PT lpt(c->LEFT() - cell->GetW(), cell->DOWN());
        lpt.dis = abs(lpt.x - xo) + abs(lpt.y - yo);    
        Search.insert(lpt);

        PT rpt(c->RIGHT(), cell->DOWN());
        rpt.dis = abs(rpt.x - xo) + abs(rpt.y - yo);    
        Search.insert(rpt);
        
    }
    

    for(const PT p : Search) {
        cell->SetXY(p.x, p.y);
        if(Legal(cell)) return true;
    }

    cell->SetXY(xo, yo);
    return false;

}