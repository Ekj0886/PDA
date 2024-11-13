#include <bits/stdc++.h>
//#include "HEADER_FILE_NAME"

#define F first 
#define S second

using namespace std;

// functions defined in header file

bool PLACEROW::PushRight(CELL* cell) {

    for(int row = GetRow(cell->DOWN()); row < min(GetRow(cell->TOP()), row_num-1); row++) {
        
        Rptr cptr = U_ptr(row, cell);

        CELL* ucell = *cptr;
    
    }

}