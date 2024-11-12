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
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);

        Rptr ub = placement_row[row].lower_bound(cell);
        Rptr lb = prev(ub);

        CELL* ub_cell = *ub;
        CELL* lb_cell = *lb;
        
        // Check if an upper bound & lower bound
        if(!lb_cell->pseudo && lb_cell->RIGHT() > cell->LEFT()) return false;
        if(!ub_cell->pseudo && ub_cell->LEFT() < cell->RIGHT()) return false;

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
    space.resize(row);
    CELL* pseudo_cell_left = new CELL("L", xcoor-1, 0, 1, 0, 1);
    CELL* pseudo_cell_right = new CELL("R", xcoor + site_num - 1, 0, 1, 0, 1);
    pseudo_cell_left->pseudo = true; 
    pseudo_cell_right->pseudo = true;
    for(int r = 0; r < row; r++) {
        placement_row[r].insert(pseudo_cell_left);
        placement_row[r].insert(pseudo_cell_right);
        space[r] = site_num;
    }
}

void PLACEROW::Insert(CELL* cell) {
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);
        placement_row[row].insert(cell);
        // space[row] -= cell->GetW();
        y += height;
    }
}

void PLACEROW::Remove(CELL* cell) {
    if(cell->Fix()) {
        cout << "Cell fixed, can't remove" << endl;
        return;
    }
    double y = cell->DOWN();
    while(y < cell->TOP() && y <= TOP()) {
        int row = GetRow(y);
        placement_row[row].erase(cell);
        // space[row] += cell->GetW();
        y += height;
    }
}


bool PLACEROW::FindVacant(CELL* cell) {
    // cout << "== Find Vacant" << endl;
    int track = GetTrack(cell);
    int row = GetRow(cell->DOWN());

    double x_origin = cell->LEFT(), y_origin = cell->DOWN();
    
    deque<Rptr> q;

    Rptr ub = placement_row[row].lower_bound(cell);
    Rptr lb = prev(ub);

    q.push_back(ub);
    q.push_back(lb);

    for(int i = 1; i <= track; i++) {
        int Urow = row + i;
        int Lrow = row - i;

        if(Urow < row_num && Urow >= 0) {
            Rptr u = placement_row[Urow].lower_bound(cell);
            Rptr l = prev(u);
            q.push_back(u);
            q.push_back(l);
        }

        if(Lrow >= 0 && Lrow < row_num) {
            Rptr u = placement_row[Lrow].lower_bound(cell);
            Rptr l = prev(u);
            q.push_back(u);
            q.push_back(l);
        }
    }


    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();
        
        // if(!(*rptr)->pseudo) {
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

    Rptr ub = placement_row[row].lower_bound(cell);
    Rptr lb = prev(ub);

    q.push_back(ub);
    q.push_back(lb);


    while(!q.empty()) {
        Rptr rptr = q.front();
        q.pop_front();
        
        // if(!(*rptr)->pseudo) {
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
