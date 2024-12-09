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
    xo = X;
    yo = Y;
    w = width;
    h = height;
    fix = Fix;
    merge = false;
    pseudo = false;
}

CELL::~CELL() { }

void CELL::SetXY(double X, double Y) {
    x = X;
    y = Y;
}

double CELL::DIFF() {
    return abs(x - xo) + abs(y - yo);
}