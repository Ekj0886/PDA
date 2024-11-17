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
    while(opt >> __) {
    // for(int q = 0; q < 10; q++) { opt >> __;

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
                cout << "push pass " << legal_num << endl;
                DumpMoved(merge_cell);
            }
            else if(SpaceSearch(merge_cell)) {
                cout << "space pass " << legal_num << endl;
                DumpSpace(merge_cell);
            }
            else {
                cout << "fail " << illegal_num << endl;
                illegal_num ++;
            }
        
        } 
        else {
            legal_num++;
            AddCell(merge_cell);
            DumpSpace(merge_cell);
        } 

        
    }

    cout << "== Final Cell Num " << Cellmap.size() << endl;
    cout << "== Legal number  : " << legal_num << endl;
    cout << "== ILLegal number: " << illegal_num << endl;

}   

bool LEGALIZER::SpaceSearch(CELL* cell) {
    // cout << "Start Space search" << endl;
    if(PR.FindVacant(cell)) {
        legal_num++;
        AddCell(cell);
        return true;
    }
    else if(PR.DumbFill(cell)) {
        AddCell(cell);
        legal_num++;
        return true;
    }
    else {
        illegal_num++;
        return false;
    }
}

bool LEGALIZER::SRTetris(CELL* cell) {
    if(PR.FindSRVacant(cell)) {
        if(PR.Legalize(cell)) {
            legal_num++;
            Cellmap[cell->GetName()] = cell;
            return true;
        }
    }
    return false;
}

void LEGALIZER::AddCell(CELL* cell) {
    PR.Insert(cell);
    Cellmap[cell->GetName()] = cell;
}

void LEGALIZER::DumpSpace(CELL* cell) {

    ofstream outfile(out_file, ios::app);

    if (!outfile) {
        cerr << "Error: Could not open file " << out_file << endl;
        return;
    }
    
    outfile << fixed << cell->LEFT() << " " << cell->DOWN() << endl;
    outfile << "0" << endl;

}

void LEGALIZER::DumpMoved(CELL* merge_cell) {
    
    ofstream outfile(out_file, ios::app);

    if (!outfile) {
        cerr << "Error: Could not open file " << out_file << endl;
        return;
    }

    outfile << fixed << merge_cell->LEFT() << " " << merge_cell->DOWN() << endl;

    outfile << PR.CellMem.size() << endl;

    for(auto cptr : PR.CellMem) {
        CELL* cell = cptr.F;
        outfile << cell->GetName() << " " << cell->LEFT() << " " << cell->DOWN() << endl;
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