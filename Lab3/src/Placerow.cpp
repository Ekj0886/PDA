#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

int PLACEROW::GetRow(double y) {
    return (y - ycoor) / height;
}

bool PLACEROW::Legal(CELL* cell) {

    // set cell y if not on site
    if((double)((cell->DOWN() - ycoor) / height) != (int)((cell->DOWN() - ycoor) / height) ) {
        cout << "Tweak y" << endl;
        double legal_y = ycoor + (double)height * (int)((cell->DOWN() - ycoor) / height);
        cell->SetXY(cell->LEFT(), legal_y);
    }

    // check Die bound
    if(cell->LEFT() < Die.lowerX || cell->RIGHT() > Die.upperX || cell->DOWN() < Die.lowerY || cell->TOP() > Die.upperY) return false;

    // check within placement row
    if(cell->LEFT() < xcoor || cell->LEFT() > xcoor + site_num ) return false;
    
    // check overlap
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TopRow()) {
        int row = GetRow(y);

        // cout << cell->LEFT() << " " << cell->RIGHT() << endl;

        auto ub = placement_row[row].lower_bound(cell);
        auto lb = prev(ub);

        CELL* ub_cell = *ub;
        CELL* lb_cell = *lb;

        // cout << lb_cell->GetName() << " " << lb_cell->LEFT() << " "  << ub_cell->GetName() << " " << ub_cell->LEFT() << endl;
        // cout << "row: " << row << endl;
        
        // Check if an upper bound 
        if(!lb_cell->pseudo && lb_cell->RIGHT() > cell->LEFT()) {
            // cout << "lb_cell overlap" << endl;
            return false;
        }
        if(!ub_cell->pseudo && ub_cell->LEFT() < cell->RIGHT()) {
            // cout << "ub_cell overlap" << endl;
            return false;
        }
        y += height;
    }
    return true;
}

void PLACEROW::PrintRow(int row) {
    cout << row << ": ";
    if(placement_row[row].empty()) {
        cout << "EMPTY" << endl;
        return; 
    }
    for(const auto& c : placement_row[row]) {
        cout << "(" << c->LEFT() << ", " << c->RIGHT() << ") ";
    }cout << endl;
}


void PLACEROW::PrintPR() {
    for(int i = row_num-1; i >= 0; i--) {
        PrintRow(i);
    }
}

RowIterator PLACEROW::Xs(int row, double x) {
    // Ensure the row index is valid
    if (row < 0 || row >= (int)placement_row.size()) return std::nullopt; 
    CELL temp_cell;
    temp_cell.SetXY(x, 0); 
    auto it = placement_row[row].upper_bound(&temp_cell);
    if (it != placement_row[row].end()) return it;
    return std::nullopt;  // Return an empty optional if no element is found
}

RowIterator PLACEROW::Xe(int row, double x) {
    // Ensure the row index is valid
    if (row < 0 || row >= (int)placement_row.size()) return std::nullopt; 
    CELL temp_cell;
    temp_cell.SetXY(x, 0); 
    auto it = placement_row[row].lower_bound(&temp_cell);
    if (it != placement_row[row].end()) return it;
    return std::nullopt;  // Return an empty optional if no element is found
}


void PLACEROW::Init(int row, int site, double h, double x, double y) {
    row_num = row;
    site_num = site;
    height = h;
    xcoor = x;
    ycoor = y;

    placement_row.resize(row);
    CELL* pseudo_cell_left = new CELL("L", xcoor-1, 0, 0, 0, 1);
    CELL* pseudo_cell_right = new CELL("R", xcoor + site_num, 0, 0, 0, 1);
    pseudo_cell_left->pseudo = true; 
    pseudo_cell_right->pseudo = true;
    for(int r = 0; r < row; r++) {
        placement_row[r].insert(pseudo_cell_left);
        placement_row[r].insert(pseudo_cell_right);
    }
}

void PLACEROW::Insert(CELL* cell) {
    double y = cell->DOWN();
    while(y < cell->TOP()) {
        int row = GetRow(y);
        placement_row[row].insert(cell);
        y += height;
        if(y > TopRow()) break;
    }
}

void PLACEROW::Remove(CELL* cell) {
    if(cell->Fix()) {
        cout << "Cell fixed, can't remove" << endl;
        return;
    }
    double y = cell->DOWN();
    while(y < cell->TOP()) {
        int row = GetRow(y);
        placement_row[row].erase(cell);
        y += height;
        if(y > TopRow()) break;
    }
}

void PLACEROW::test() {

    PrintRow(500);

    auto xs = Xs(500, 300000);
    auto xe = Xe(500, 500000);

    cout << "test" << endl;
    for (auto c = *xs; c != *xe; ++c) {
        std::cout << (*c)->LEFT() << " " << (*c)->Fix() << " ";
    }
    cout << endl;

}
