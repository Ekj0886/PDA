#include <bits/stdc++.h>
#include "GR.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

void GR::Parse(string& input_gmp, string& input_gcl, string& input_cst) {
    PARSER Parser(input_gmp, input_gcl, input_cst);
    Parser.Parse(*this);
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

    // checker for grid pointer
    // for(int i = 0; i < grid_h; i++) {
    //     for(int j = 0; j < grid_w; j++) {
    //         Gcell* cur = Grid[i][j];
    //         if(cur->L == nullptr) cout << "L null" << endl;
    //         if(cur->R == nullptr) cout << "R null" << endl;
    //         if(cur->D == nullptr) cout << "D null" << endl;
    //         if(cur->U == nullptr) cout << "U null" << endl;
    //     }
    // }

}

void GR::DirectRoute() {

    int succeed_num = 0;
    int fail_num    = 0;

    for(auto& net : Bump_map) {

        // int id = net.F;
        Net n = net.S;

        Gcell* src = GetCell(n.src_x, n.src_y);
        Gcell* snk = GetCell(n.snk_x, n.snk_y);
        
        // cout << "== Route net " << id << endl;
        // cout << "       src: " << src->x << " " << src->y << endl;
        // cout << "       snk: " << snk->x << " " << snk->y << endl;

        src->cost = 0;
        src->parent= src;
        src->dir = Vertical; // start on m1
        
        open_list.clear();
        close_list.clear();

        if(A_star(src, snk)) succeed_num++;
        else                 fail_num++;

        TracePath(src, snk);

    }

    cout << "Pass num: " << succeed_num << endl;
    cout << "Fail num: " << fail_num << endl;

}

bool GR::A_star(Gcell* src, Gcell* snk) {

    open_list.insert(src);

    while(!open_list.empty()) {
        
        // cout << endl;
        // cout << "openlist: " << open_list.size() << endl; 
        // cout << "closelist: " << close_list.size() << endl; 

        Gcell* cell = *open_list.begin();
        open_list.erase(cell);
        
        // cout << "visit new cell " << cell->x << " " << cell->y << endl;

        deque<Gcell*> cell_que;

        // Load cell_que & set direction
        if(cell->L != pseudo_cell && cell->L != cell->parent) {
            cell_que.push_back(cell->L);
        }
        if(cell->R != pseudo_cell && cell->R != cell->parent) {
            cell_que.push_back(cell->R);
        }
        if(cell->D != pseudo_cell && cell->D != cell->parent) {
            cell_que.push_back(cell->D);
        }
        if(cell->U != pseudo_cell && cell->U != cell->parent) {
            cell_que.push_back(cell->U);
        } 

        while(!cell_que.empty()) {

            Gcell* nxt_cell = cell_que.front();
            Gcell  cell_mem = *nxt_cell;
            
            cell_que.pop_front();

            // cout << "nxt_cell " << nxt_cell->x << " " << nxt_cell->y <<  endl;

            cell_mem = *nxt_cell;
            if(cell->L == nxt_cell || cell->R == nxt_cell) nxt_cell->dir = Horizontal;
            else                                           nxt_cell->dir = Vertical;
            nxt_cell->parent = cell;


            // found sink
            if(nxt_cell == snk) {
                close_list.insert(nxt_cell);
                // cout << "Path Found" << endl;
                return true;
            }
            // successor already in open list
            else if(open_list.find(nxt_cell) != open_list.end()) {
                // cout << "   successor inside open list" << endl;
                if(nxt_cell->cost <= Cost(nxt_cell)) {
                    nxt_cell->dir    = cell_mem.dir;
                    nxt_cell->parent = cell_mem.parent;
                    continue;
                }
                else {
                    open_list.erase(nxt_cell);
                    nxt_cell->cost = Cost(nxt_cell);
                    open_list.insert(nxt_cell);
                    continue;
                }
            }
            // successor already in close list
            else if(close_list.find(nxt_cell) != close_list.end()) {
                // cout << "   successor inside close list" << endl;
                if(nxt_cell->cost <= Cost(nxt_cell)) {
                    nxt_cell->dir    = cell_mem.dir;
                    nxt_cell->parent = cell_mem.parent;
                    continue;
                }
                else {
                    close_list.erase(nxt_cell);
                    nxt_cell->cost = Cost(nxt_cell);
                    open_list.insert(nxt_cell);
                    continue;
                }
            }
            // successor unvisited
            else {
                // cout << "   successor unvisited" << endl;
                nxt_cell->cost = Cost(nxt_cell);
                open_list.insert(nxt_cell);
            }

        }
        // cout << "close cell " << cell->x << " " << cell->y << endl;

        // push cell into close list
        close_list.insert(cell);

    } 
    
    // cout << "Fail" << endl;
    return false;
}

void GR::TracePath(Gcell* src, Gcell* snk) {

}