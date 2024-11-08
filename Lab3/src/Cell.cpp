#include <bits/stdc++.h>
#include "Cell.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file
CELL::CELL() { }

CELL::CELL(string cellname, double X, double Y, double width, double height, bool Fix) {
    name = cellname;
    x = X;
    y = Y;
    w = width;
    h = height;
    fix = Fix;
}

CELL::~CELL() { }

void CELL::SetXY(double X, double Y) {
    x = X;
    y = Y;
}

