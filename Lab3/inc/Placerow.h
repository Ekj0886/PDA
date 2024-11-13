#ifndef PLACEROW_H
#define PLACEROW_H
#include <bits/stdc++.h>
#include "Cell.h"
#include "Type.h"

#define F first 
#define S second

using namespace std;

class PLACEROW {
  
public:
    DIE Die;
    int row_num;
    int site_num;
    double height;
    double xcoor, ycoor;
    
    vector<Set*> placement_row;
    vector<double> space;
    vector<double> Xseg; // start point of segment

    // operator overwrite
    Set& operator[](size_t index) {
        return *placement_row[index];
    }

    // major function 
    void Init(int row, int site, double h, double x, double y);
    void Insert(CELL* cell);
    void Remove(CELL* cell);
    bool FindVacant(CELL*);
    bool SingleVacant(CELL*);
    bool DumbFill(CELL*);
    bool FindSRVacant(CELL*);
    bool Legalize(CELL*);

    // sub function for Legalize (.cpp)
    unordered_map<CELL*, double> SRcellmap;
    vector<deque<CELL*>> SRcell; 
    vector<double> x; // Row x coor indicator

    void LoadSRcell(CELL*);

    // helper function
    Set* RowSet(int row) { return placement_row[row]; }

    int GetRow(double y);
    int GetTrack(CELL*); // get cell height in # of row
    bool Legal(CELL*);
    bool SRLegal(CELL*);
    bool InBound(int);
    double TOP() { return ycoor + height*(row_num-1); }
    double LEFT() { return xcoor; }
    double RIGHT() { return xcoor + site_num - 1; }
    void PrintRow(int row);
    void PrintPR();

    

    

    // WINDOW SetWindow(CELL*);
    // void FindSegment(WINDOW W);

};

#endif