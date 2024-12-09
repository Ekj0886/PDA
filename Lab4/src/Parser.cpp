#include <bits/stdc++.h>
#include "Parser.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file
PARSER::PARSER(string& input_gmp, string& input_gcl, string& input_cst) {
    file_gmp.open(input_gmp);
    if(!file_gmp.is_open()) cout << "Cannot open file: " << input_gmp << endl;

    file_gcl.open(input_gcl);
    if(!file_gcl.is_open()) cout << "Cannot open file: " << input_gcl << endl;

    file_cst.open(input_cst);
    if(!file_cst.is_open()) cout << "Cannot open file: " << input_cst << endl;
}

PARSER::~PARSER() {
    file_gmp.close();
    file_gcl.close();
    file_cst.close();
}

void PARSER::Parse(GR& gr) {
    Load_GMP(gr);
    Load_GCL(gr);
    Load_CST(gr);
}

void PARSER::Load_GMP(GR& gr) {

    string __;
    
    // Routing area inf
    file_gmp >> __ >> gr.RA.x >> gr.RA.y >> gr.RA.w >> gr.RA.h;
    
    // Grid cell w, h
    file_gmp >> __ >> gr.cell_w >> gr.cell_h;
    
    // Chip1 parsing
    string line;
    int index; 
    file_gmp >> __ >> gr.chip1.x >> gr.chip1.y >> gr.chip1.w >> gr.chip1.h;
    file_gmp >> __;

    gr.chip1.x += gr.RA.x; 
    gr.chip1.y += gr.RA.y;

    getline(file_gmp, line);
    while(getline(file_gmp, line)) {
        if(line.empty()) break;
        istringstream iss(line);
        Net n;
        iss >> index >> n.src_x >> n.src_y;
        
        n.src_x += gr.chip1.x;
        n.src_y += gr.chip1.y;
        
        gr.Bump_map[index] = n;
    }

    getline(file_gmp, line);

    // Chip2 parsing
    file_gmp >> gr.chip2.x >> gr.chip2.y >> gr.chip2.w >> gr.chip2.h;
    file_gmp >> __;

    gr.chip2.x += gr.RA.x; 
    gr.chip2.y += gr.RA.y;

    getline(file_gmp, line);
    while(getline(file_gmp, line)) {
        if(line.empty()) break;
        istringstream iss(line);
        iss >> index;
        Net n = gr.Bump_map[index];
        iss >> n.snk_x >> n.snk_y;

        n.snk_x += gr.chip2.x;
        n.snk_y += gr.chip2.y;

        gr.Bump_map[index] = n;
    }

}

void PARSER::Load_GCL(GR& gr) {
    
    // init Grid size
    gr.grid_h = gr.RA.h / gr.cell_h;
    gr.grid_w = gr.RA.w / gr.cell_w;

    gr.Grid.resize(gr.grid_h, std::vector<Gcell*>(gr.grid_w, nullptr));
    for(int i = 0; i < gr.grid_h; i++) {
        for(int j = 0; j < gr.grid_w; j++) {
            gr.Grid[i][j] = new Gcell();
        }
    }

    // parse gcell edge capacity
    string __;
    file_gcl >> __;
    for(int i = 0; i < gr.grid_h; i++) {
        for(int j = 0; j < gr.grid_w; j++) {
            file_gcl >> gr.Grid[i][j]->left_cap >> gr.Grid[i][j]->down_cap;
        }
    }

}

void PARSER::Load_CST(GR& gr) {

    string __, line;
    file_cst >> __ >> gr.alpha;
    file_cst >> __ >> gr.beta;
    file_cst >> __ >> gr.gamma;
    file_cst >> __ >> gr.delta;

    file_cst >> __ >> gr.via_cost;
    
    //layer 1 cost
    file_cst >> __;
    for(int i = 0; i < gr.grid_h; i++) {
        for(int j = 0; j < gr.grid_w; j++) {
            double cost;
            file_cst >> cost;
            gr.Grid[i][j]->m1_cost = cost;;
            if(gr.Half_max_cost < cost) gr.Half_max_cost = cost;
        }
    }
    
    // layer 2 cost
    file_cst >> __;
    for(int i = 0; i < gr.grid_h; i++) {
        for(int j = 0; j < gr.grid_w; j++) {
            double cost;
            file_cst >> cost;
            gr.Grid[i][j]->m2_cost = cost;
            if(gr.Half_max_cost < cost) gr.Half_max_cost = cost;
        }
    } 

    gr.Half_max_cost /= 2.0;

}