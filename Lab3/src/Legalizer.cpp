#include <bits/stdc++.h>
#include "Legalizer.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file
void LEGALIZER::parse(string& infile_name, string& outfile_name) {
    this->in_file = infile_name;
    this->out_file = outfile_name;
    // outfile.open(outfile_name);
    ofstream outfile(outfile_name);
    PARSER Parser(in_file);
    Parser.parse(*this);
    cout << "== # of Cell: " << Cellmap.size() << endl;
    cout << "== # of PRow: " << PR.row_num << " " << PR.site_num << endl; 
    PR.alpha = alpha;
    PR.beta  = beta;
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

    legal_num = 0;
    illegal_num = 0;

    int moved_num = 0;
    int space_num = 0;

    while(opt >> __) {
    // for(int q = 0; q < 1632; q++) { opt >> __;

        string cell_name;
        double x, y, w, h;

        while(opt >> cell_name) {
            if(cell_name == "-->") break;
            CELL* cell = Cellmap[cell_name];
            PR.Remove(cell);
            delete cell;
            Cellmap.erase(cell_name);
        }
        opt >> cell_name >> x >> y >> w >> h;
        CELL* merge_cell = new CELL(cell_name, x, y, w, h, 0);
        merge_cell->merge = true;
        
        if(!PR.Legal(merge_cell)) {
            if(SRTetris(merge_cell)) {
                DumpMoved(merge_cell);
                moved_num++;
                // cout << "pass " << legal_num << endl;
            }
            else if(SpaceSearch(merge_cell)) {  
                DumpSpace(merge_cell);
                space_num++;
                // cout << "pass " << legal_num << endl;
            }
            else {
                // cout << "Fail " << illegal_num << ": " << merge_cell->GetName() << endl;
                illegal_num ++;
            }
        
        } 
        else {
            // cout << "pass " << legal_num << endl;
            AddCell(merge_cell);
            DumpSpace(merge_cell);
            space_num++;
        } 

        
    }

    cout << "== Final Cell Num " << Cellmap.size() << endl;
    cout << "== Legal number  : " << legal_num << endl;
    cout << "== ILLegal number: " << illegal_num << endl;
    cout << "== Moved number  : " << moved_num << endl;
    cout << "== Space number: " << space_num << endl;

}   

bool LEGALIZER::SpaceSearch(CELL* cell) {
    // cout << "space ";
    PR.origin_x = cell->LEFT();
    PR.origin_y = cell->DOWN();
    if(PR.FindVacant(cell)) {
        // cout << "vacant" << endl;
        AddCell(cell);
        return true;
    }
    else if(PR.DumbFill(cell)) {
        // cout << "dumb" << endl;
        AddCell(cell);
        return true;
    }
    else {
        return false;
    }
}

bool LEGALIZER::SRTetris(CELL* cell) {
    PR.distance = 0;
    PR.origin_x = cell->LEFT();
    PR.origin_y = cell->DOWN();
    if(PR.FindSRVacant(cell)) {
        if(PR.Legalize(cell)) {
            Cellmap[cell->GetName()] = cell;
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

void LEGALIZER::AddCell(CELL* cell) {
    PR.Insert(cell);
    Cellmap[cell->GetName()] = cell;
}

void LEGALIZER::DumpSpace(CELL* cell) {

    legal_num++;
    ofstream outfile(out_file, ios::app);

    if (!outfile) {
        cerr << "Error: Could not open file " << out_file << endl;
        return;
    }
    
    outfile << fixed << cell->LEFT() << " " << cell->DOWN() << endl;
    outfile << "0" << endl;

}

void LEGALIZER::DumpMoved(CELL* merge_cell) {
    
    legal_num++;
    ofstream outfile(out_file, ios::app);

    if (!outfile) {
        cerr << "Error: Could not open file " << out_file << endl;
        return;
    }

    outfile << fixed << merge_cell->LEFT() << " " << merge_cell->DOWN() << endl;
    
    outfile << PR.CellMem.size() << endl;

    // for (auto& c : PR.CellMem) {
    //     CELL* cell = c.F;
    //     outfile << cell->GetName() << " " << cell->LEFT() << " " << cell->DOWN() << endl;
    // }
    if(PR.CellMem.size() > 0) {
        for(auto mem : PR.CellMem) {
            CELL* cell = mem.c;
            outfile << cell->GetName() << " " << cell->LEFT() << " " << cell->DOWN() << endl;
        }
    }

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