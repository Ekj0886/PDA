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

    cout << Cellmap.size() << endl;

}


void LEGALIZER::DumpLayout(string file) {

    ofstream outfile(file);

    outfile << fixed << Die.lowerX << " " << Die.lowerY << " " << Die.upperX << " " << Die.upperY << endl;

    for (auto& cptr : Cellmap) {
        CELL* cell = cptr.S;
        outfile << fixed << cell->GetX() << " " << cell->GetY() << " " << cell->GetW() << " " << cell->GetH() << " " << cell->Fix() << endl;
    }

}