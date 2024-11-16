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

    // major function (Placerow.cpp)
    void Init(int row, int site, double h, double x, double y);
    void Insert(CELL* cell);
    void Remove(CELL* cell);
    bool FindVacant(CELL*);
    bool SingleVacant(CELL*);
    bool DumbFill(CELL*);
    bool FindSRVacant(CELL*);
    bool Legalize(CELL*);


public:
    // sub function for Tetris (PR_tetris.cpp)
    CELL* EMPTY = new CELL("Empty", 0, 0, 0, 0, 1);
    double UB, LB;   // upper & lower bound for moving cell range
    int UR, LR;
    vector<double> x; // Row x coor indicator
    vector<CELL*> PTR; // Row's iterator
    unordered_map<CELL*, double> CellMem; // memorize original x of cell and moved cell
    deque<Rptr> Qx;

    bool H_overlap(CELL*, CELL*);
    bool V_overlap(CELL*, CELL*);
    bool isBlk(CELL*);
    void PrintX();
    void PrintPTR();
    void DumpMem();
    void Restore();
    bool CheckPTR();

    // push right function
    bool InitR(CELL*);
    void UpdateRX(CELL*);
    bool FindR(CELL*);
    double CellRX(CELL*); // return maximum x value within cell range
    bool PushRight();

    // push left function
    bool InitL(CELL*);
    void UpdateLX(CELL*);
    bool FindL(CELL*);
    double CellLX(CELL*); // return minimum x value within cell range
    bool PushLeft();



public:
    // helper function (PR_util.cpp)
    Set* RowSet(int row) { return placement_row[row]; }
    int GetRow(double y);
    int GetTrack(CELL*); // get cell height in # of row
    bool Legal(CELL*);   // return true if cell position is legal
    bool SRLegal(CELL*); // return true if cell not overlap with fix cell
    bool InBound(int);   // check row in bound
    double TOP() { return ycoor + height*(row_num-1); }
    double LEFT() { return xcoor; }
    double RIGHT() { return xcoor + site_num - 1; }
    void PrintRow(int row);
    void PrintPR();

    

    

    // WINDOW SetWindow(CELL*);
    // void FindSegment(WINDOW W);

};

#endif