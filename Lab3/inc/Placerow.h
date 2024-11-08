#ifndef PLACEROW_H
#define PLACEROW_H
#include <bits/stdc++.h>
#include "Cell.h"

#define F first 
#define S second

using namespace std;

class LEGALIZER;

class PLACEROW {

private:
    
    
public:
    int row_num;
    int site_num;
    double height;
    double x, y;
    
    vector<vector<CELL*>> placement_row;


    // function
    vector<CELL*>& operator[](size_t index) {
        return placement_row[index];
    }

    void Init(int row, int site, double h, double x, double y);
    void PlaceCell(LEGALIZER& LGR);
    
};

#endif