#ifndef LEGALIZER_H
#define LEGALIZER_H
#include <bits/stdc++.h>
#include "Cell.h"
#include "Parser.h"
#include "Placerow.h"
#include "Type.h"

#define F first 
#define S second

using namespace std;

class LEGALIZER {

private:
    int legal_num, illegal_num;

public:
    string in_file, out_file;

    double alpha, beta;
    DIE Die;

    unordered_map<string, CELL*> Cellmap;
    PLACEROW PR;

    // function
    void parse(string&, string&);
    void PlaceCell();
    void RunOpt(string&);
    void AddCell(CELL*);
    bool SpaceSearch(CELL*);
    bool SRTetris(CELL*);
    void DumpLayout(string);
    void DumpSpace(CELL*);
    void DumpMoved(CELL*);
    
};

#endif