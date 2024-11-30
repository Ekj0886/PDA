#ifndef PARSER_H
#define PARSER_H
#include <bits/stdc++.h>
#include "GR.h"

#define F first 
#define S second

using namespace std;

class GR;

class PARSER {

private:
    ifstream file_gmp;
    ifstream file_gcl;
    ifstream file_cst;
    
public:
    PARSER(string&, string&, string&);
    ~PARSER();
    void Parse(GR& gr);

public:
    void Load_GMP(GR& gr);
    void Load_GCL(GR& gr);
    void Load_CST(GR& gr);
    
};

#endif