#ifndef PLACEROW_H
#define PLACEROW_H
#include <bits/stdc++.h>
#include "Cell.h"
#include "Type.h"

#define F first 
#define S second

using namespace std;

class PLACEROW {
  
public: // main placerow
    DIE Die;
    int row_num;
    int site_num;
    double height;
    double xcoor, ycoor;
    
    vector<vector<CELL*>> P_Row;
    vector<double> space;
    vector<double> Xseg; // start point of segment
    
    // operator overwrite
    vector<CELL*>& operator[](size_t index) {
        return P_Row[index];
    }

    // major function 
    void Init(int row, int site, double h, double x, double y);
    void Insert(CELL*);
    void Remove(CELL*);
    bool FindVacant(CELL*);
    bool SingleVacant(CELL*);
    bool DumbFill(CELL*);
    bool FindSRVacant(CELL*);
    bool Legalize(CELL*);


public: // sub member function for Legalize (PR_tetris.cpp)
    map<int, unordered_map<Rptr, CELL*, RptrHash>> RowMem;
    map<int, unordered_map<CELL*, double>> CellMem; // document moved cell original position
    bool Overlap(CELL*, CELL*);
    bool PushRight(CELL*);
    bool PushLeft(CELL*);
    void GoBack(CELL*);


public: // helper function (PR_util.cpp)
    int GetRow(double y);
    BoundPtr BIndex(int, CELL*);
    BoundCell Bcell(int, double);
    int U_index(int, double);
    int L_index(int, double);
    Rptr U_ptr(int, double);
    Rptr L_ptr(int, double);
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