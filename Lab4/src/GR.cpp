#include <bits/stdc++.h>
#include "GR.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

void GR::Parse(string& input_gmp, string& input_gcl, string& input_cst, string& output_lg, string& out_image) {
    PARSER Parser(input_gmp, input_gcl, input_cst);
    Parser.Parse(*this);
    outfile_name = output_lg;
    outimage_name = "layout/image_" + out_image;
    ofstream outfile(outfile_name);
    ofstream outimage(outimage_name);

    cout << "== Parse files" << endl;
    cout << "   -- Grid Width : " << grid_w << endl;
    cout << "   -- Grid Height: " << grid_h << endl;
}

void GR::SetGrid() {

    pseudo_cell = new Gcell();
    pseudo_cell->left_cap = -1;
    pseudo_cell->down_cap = -1;

    // set pointers
    for(int i = 0; i < grid_h; i++) {
        for(int j = 0; j < grid_w; j++) {
            Gcell* cur = Grid[i][j];
            cur->x = j;
            cur->y = i;

            if(j + 1 >= grid_w) cur->R = pseudo_cell;
            else                cur->R = Grid[i][j+1];

            if(j - 1 < 0) cur->L = pseudo_cell;
            else          cur->L = Grid[i][j-1];

            if(i + 1 >= grid_h) cur->U = pseudo_cell;
            else                cur->U = Grid[i+1][j];

            if(i - 1 < 0) cur->D = pseudo_cell;
            else          cur->D = Grid[i-1][j];
        }
    }

    // set left/bottom capacity
    for(int i = 0; i < grid_h; i++) {
        Grid[i][0]->left_cap = -1;
    }

    for(int j = 0; j < grid_w; j++) {
        Grid[0][j]->down_cap = -1;
    }
    
}

void GR::DirectRoute() {

    path_list.clear();
    int succeed_num = 0;
    int fail_num    = 0;

    for(auto& net : Bump_map) {

        int id = net.F;
        Net n = net.S;

        Gcell* src = GetCell(n.src_x, n.src_y);
        Gcell* snk = GetCell(n.snk_x, n.snk_y);
        
        // cout << "== Route net " << id << endl;
        // cout << "       src: " << src->x << " " << src->y << endl;
        // cout << "       snk: " << snk->x << " " << snk->y << endl;

        src->cost = 0;
        src->parent= src;
        src->dir = Vertical; // start on m1
        
        PriorityQ empty;
        open_list.swap(empty);
        open_set.clear();
        close_list.clear();

        if(A_star(src, snk)) succeed_num++;
        else                 fail_num++;
        
        // cout << "== Route net " << id << endl;
        // cout << "       cost " << snk->cost << endl;

        TracePath(id, src, snk);
        UpdateCap();
        DumpOutput(id, n, src, snk);

    }

    cout << "Pass num: " << succeed_num << endl;
    cout << "Fail num: " << fail_num << endl;
    
    DumpImage();
    

}

bool GR::A_star(Gcell* src, Gcell* snk) {
    
    open_list.push(src);
    open_set.insert(src);

    while(!open_list.empty()) {

        Gcell* current = open_list.top();
        open_list.pop();

        if (open_set.find(current) == open_set.end()) {
            continue;
        }

        if (current == snk) {
            return true; // Path found
        }

        close_list.insert(current);
        
        for(Gcell* neighbor : {current->L, current->R, current->D, current->U}) {
            
            if(neighbor == current->parent || neighbor == pseudo_cell || close_list.count(neighbor)) continue;

            Direction origin_dir = neighbor->dir;
            Gcell* origin_parent = neighbor->parent;

            if(current->L == neighbor || current->R == neighbor) neighbor->dir = Horizontal;
            else                                                 neighbor->dir = Vertical;

            neighbor->parent = current;
            
            double step_cost = Cost(neighbor, current);
            double tentative_cost = current->cost + step_cost;
    
            if (open_set.find(neighbor) == open_set.end()) {
                // New node: Add to open list
                neighbor->cost = tentative_cost;
                open_list.push(neighbor);
                open_set.insert(neighbor);
            } else if (tentative_cost < neighbor->cost) {
                // Update node: Use lazy removal
                neighbor->cost = tentative_cost;
                open_list.push(neighbor); // Insert updated neighbor
            } else {
                // Restore neighbor direction & parent
                neighbor->dir = origin_dir;
                neighbor->parent = origin_parent;
            }

        }

    }

    return false;
}

void GR::TracePath(int id, Gcell* src, Gcell* snk) {
    
    Gcell* current = snk->parent;
    Direction last_dir = snk->dir;
    
    path = new deque<Gcell*>();
    path->push_front(snk);

    while(current != src) {
        while(current->dir == last_dir) {
            current = current->parent;
            if(current == src) {
                path->push_front(src);
                path_list[id] = path;
                return;
            }
        }
        last_dir = current->dir;
        path->push_front(current);
        current = current->parent;
    }
    path->push_front(src);
    path_list[id] = path;
    return;

}

void GR::UpdateCap() {

    for(unsigned int i = 0; i < path->size() - 1; i++) {
        
        Gcell* start = (*path)[i];
        Gcell* end   = (*path)[i + 1];

        // cout << start->x << " " << start->y << " " << end->x << " " << end->y << endl;

        if(start->x < end->x) {
            // trace right
            Gcell* c = start;
            while(c != end) {
                c = c->R;
                c->left_cap--;
            }
        } else if(start->x > end->x) {
            // trace left
            Gcell* c = start;
            while(c != end) {
                c->left_cap--;
                c = c->L;
            }
        } else if(start->y < end->y) {
            // trace up
            Gcell* c = start;
            while(c != end) {
                c = c->U;
                c->down_cap--;
            }
        } else if(start->y > end->y) {
            // trace down
            Gcell* c = start;
            while(c != end) {
                c->down_cap--;
                c = c->D;
            }
        }

    }

}

void GR::DumpOutput(int id, Net n, Gcell* src, Gcell* snk) {
    
    // cout << "== Dump Output" << endl;
    ofstream outfile;
    outfile.open(outfile_name, ios::app);

    int x = n.src_x, y = n.src_y;
    
    outfile << "n" << id << endl;
    for(unsigned int i = 0; i < path->size() - 1; i++) {

        Gcell* start = (*path)[i];
        Gcell* end   = (*path)[i + 1];

        if(i != 0 || (start == src && end->dir == Horizontal)) outfile << "via" << endl;

        if(end->dir == Vertical) {
            outfile << "M1 " << x << " " << y << " ";
            y += (end->y - start->y)*cell_h;
            outfile << x << " " << y << endl;
        } else {
            outfile << "M2 " << x << " " << y << " ";
            x += (end->x - start->x)*cell_w;
            outfile << x << " " << y << endl;
        }

        if(end == snk && end->dir == Horizontal) outfile << "via" << endl;
    }

    outfile << ".end" << endl;
    
}

void GR::DumpImage() {
    cout << "== Dump Image" << endl;
    ofstream outimage;

    outimage.open(outimage_name, ios::app);

    outimage << RA.x << " " << RA.y << " " << RA.w << " " << RA.h << endl;
    outimage << chip1.x << " " << chip1.y << " " << chip1.w << " " << chip1.h << endl;
    outimage << chip2.x << " " << chip2.y << " " << chip2.w << " " << chip2.h << endl;
    
    for(auto& net : Bump_map) {

        int id = net.F;
        Net n = net.S;

        deque<Gcell*>* Path = path_list[id];

        int x = n.src_x, y = n.src_y;

        for(unsigned int i = 0; i < Path->size() - 1; i++) {
            Gcell* start = (*Path)[i];
            Gcell* end   = (*Path)[i + 1];
            
            outimage << x << " " << y << " ";
            y += (end->y - start->y)*cell_h;
            x += (end->x - start->x)*cell_w;
        }
        
        outimage << x << " " << y << endl;

    }
}