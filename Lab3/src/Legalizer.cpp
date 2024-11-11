#include <bits/stdc++.h>
#include "Legalizer.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file
void LEGALIZER::parse(string& file_name) {
    this->file = file_name;
    PARSER Parser(file);
    Parser.parse(*this);
    cout << "== # of Cell: " << Cellmap.size() << endl;
    cout << "== # of PRow: " << PR.row_num << " " << PR.site_num << endl; 
}

void LEGALIZER::PlaceCell() {
    for (auto& cptr : Cellmap) {
        CELL* cell = cptr.S;
        PR.Insert(cell);
    }
}

void LEGALIZER::RunOpt(string& opt_file) {

    ifstream opt(opt_file); 
    string __;

    // PR.PrintPR();
    // cout << endl;

    int num = 0;
    while(opt >> __) {
    // opt >> __;

        string cell_name;
        double x, y, w, h;

        while(opt >> cell_name) {
            if(cell_name == "-->") break;
            CELL* cell = Cellmap[cell_name];
            PR.Remove(cell);
            delete cell;
            Cellmap.erase(cell_name);
        }

        // PR.PrintPR();
        // cout << endl;

        opt >> cell_name >> x >> y >> w >> h;
        CELL* merge_cell = new CELL(cell_name, x, y, w, h, 0);
        merge_cell->merge = true;
        
        if(!PR.Legal(merge_cell)) {
            Legalize(merge_cell);
            // cout << "illegal" << endl;
        } else {
            // cout << "Legal" << endl;
            num++;
            PR.Insert(merge_cell);
            Cellmap[cell_name] = merge_cell;
        } 

        // Cellmap[cell_name] = merge_cell;
        
    }

    cout << "== Final Cell Num " << Cellmap.size() << endl;
    cout << "== Legal number: " << num << endl;
    // PR.test(); 

    // PR.PrintPR();
    // cout << endl;

}   


void LEGALIZER::Legalize(CELL* cell) {
    // int row_num = cell->GetH() / PR.height;
    // cout << row_num << endl;
    // cout << cell->GetName() << endl;
}

void LEGALIZER::DumpLayout(string file) {
    ofstream outfile(file);

    outfile << fixed << Die.lowerX << " " << Die.lowerY << " " << Die.upperX << " " << Die.upperY << endl;
    
    for (auto& cptr : Cellmap) {
        CELL* cell = cptr.S;
        outfile << fixed << cell->LEFT() << " " << cell->DOWN() << " " << cell->GetW() << " " << cell->GetH() << " " << cell->Fix() << " " << cell->merge << endl;
    }
    cout << "== Dump Layout" << endl;
}