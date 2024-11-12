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

    int legal_num = 0, illegal_num = 0;
    while(opt >> __) {

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
            // PR.FindVacant(merge_cell);
            if(PR.FindVacant(merge_cell)) {
                legal_num++;
                AddCell(merge_cell);
            }
            else {
                // illegal_num++;
                if(PR.DumbFill(merge_cell)) {
                    AddCell(merge_cell);
                    legal_num++;
                }
                else {
                    illegal_num++;
                }
            }
        } else {
            legal_num++;
            AddCell(merge_cell);
        } 

        // Cellmap[cell_name] = merge_cell;
        
    }

    cout << "== Final Cell Num " << Cellmap.size() << endl;
    cout << "== Legal number  : " << legal_num << endl;
    cout << "== ILLegal number: " << illegal_num << endl;

}   

void LEGALIZER::AddCell(CELL* cell) {
    PR.Insert(cell);
    Cellmap[cell->GetName()] = cell;
}

void LEGALIZER::Legalize(CELL* cell) {
    // int row_num = cell->GetH() / PR.height;
    // if(PR.FindVacant()) {

    // }
    
}

// bool LEGALIZER::FindVacant(CELL* cell) {
//     BOUND bound = PR.GetBound(cell);
//     CELL* Ucell = bound.Ucell;
//     CELL* Lcell = bound.Lcell;

//     while(!Ucell->pseudo || !Lcell->pseudo) {
//         if(!Ucell->pseudo) {
//             cell->SetXY(Ucell->RIGHT(), cell->DOWN());
//             if(PR.Legal(cell)) return true;
//             else Ucell++;
//         }
//         if(!Lcell->pseudo) {
//             cell->SetXY(Ucell->LEFT(), cell->DOWN());
//             if(PR.Legal(cell)) return true;
//             else Lcell = prev(Lcell);
//         }
//     }
// }

void LEGALIZER::DumpLayout(string file) {
    ofstream outfile(file);

    outfile << fixed << Die.lowerX << " " << Die.lowerY << " " << Die.upperX << " " << Die.upperY << endl;
    
    for (auto& cptr : Cellmap) {
        CELL* cell = cptr.S;
        outfile << fixed << cell->LEFT() << " " << cell->DOWN() << " " << cell->GetW() << " " << cell->GetH() << " " << cell->Fix() << " " << cell->merge << endl;
    }
    cout << "== Dump Layout" << endl;
}