#include <bits/stdc++.h>
#include "Parser.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file
PARSER::PARSER(string& file) {
    infile.open(file);
    if(!infile.is_open()) {
        cout << "Cannot open file: " << file << endl;
    }
}

PARSER::~PARSER() {
    infile.close();
}

void PARSER::parse(LEGALIZER& LGR) {
    LoadParam(LGR);
    LoadDie(LGR);
    LoadCell(LGR);
    // LoadPlacementRow(LGR);
}

void PARSER::LoadParam(LEGALIZER& LGR) {
    string __;
    infile >> __ >> LGR.alpha >> __ >> LGR.beta;
}

void PARSER::LoadDie(LEGALIZER& LGR) {
    string __;
    infile >> __ >> LGR.Die.lowerX >> LGR.Die.lowerY >> LGR.Die.upperX >> LGR.Die.upperY;
}

void PARSER::LoadCell(LEGALIZER& LGR) {
    string cellname;
    streampos currentPos = infile.tellg();
    while(infile >> cellname) {
        if(cellname == "PlacementRows") {
            infile.seekg(currentPos); 
            break;
        }

        double x, y, w, h;
        string fix;

        infile >> x >> y >> w >> h >> fix;

        CELL* cell = new CELL(cellname, x, y, w, h, fix=="FIX");
        LGR.Cellmap[cellname] = cell;
        currentPos = infile.tellg();
    }
}

void PARSER::LoadPlacementRow(LEGALIZER& LGR) {
    cout << "load start" << endl;
    string __;
    CELL* cell = new CELL();
    LGR.PR.Init(50000, 5000000, 10, 5.5, 5.5);
    LGR.PR[0].push_back(cell);
    cout << "load done" << endl;
    // while(infile >> __) {
        
    // }
} 
