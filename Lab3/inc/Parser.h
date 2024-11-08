#ifndef PARSER_H
#define PARSER_H
#include <bits/stdc++.h>
#include "Legalizer.h"

#define F first 
#define S second

using namespace std;

class LEGALIZER;

class PARSER {

private:
    ifstream infile;
    
public:
    PARSER(string&);
    ~PARSER();
    void parse(LEGALIZER& LGR);

private:
    void LoadParam(LEGALIZER& LGR);
    void LoadDie(LEGALIZER& LGR);
    void LoadCell(LEGALIZER& LGR);
    void LoadPlacementRow(LEGALIZER& LGR);

};

#endif