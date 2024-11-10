#ifndef LEGALIZER_H
#define LEGALIZER_H
#include <bits/stdc++.h>
#include "Cell.h"
#include "Parser.h"
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

class LEGALIZER {

public:
    string file;

    double alpha, beta;
    DIE Die;

    unordered_map<string, CELL*> Cellmap;
    PLACEROW PR;

    // function
    void parse(string&);
    void PlaceCell();
    void RunOpt(string&);
    void Legalize(CELL*);

    void DumpLayout(string);
    
};

#endif