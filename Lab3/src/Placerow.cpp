#include <bits/stdc++.h>
#include "Placerow.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

void PLACEROW::Init(int row, int site, double h, double xcoor, double ycoor) {
    row_num = row;
    site_num = site;
    height = h;
    x = xcoor;
    y = ycoor;

    placement_row.resize(row);
}

void PLACEROW::PlaceCell(LEGALIZER& LGR) {

    

}
