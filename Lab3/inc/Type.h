#ifndef TYPE_H
#define TYPE_H
#include <bits/stdc++.h>
#include "Cell.h"

#define F first 
#define S second

struct CompareByX {
    bool operator()(const CELL* a, const CELL* b) const {
        return a->LEFT() < b->LEFT();
    }
};

using Set = set<CELL*, CompareByX>;
using RowIterator = std::optional<std::set<CELL*, CompareByX>::iterator>;
using Rptr = std::vector<CELL*>::iterator;

struct DIE {
    double lowerX, lowerY;
    double upperX, upperY;
};

struct BoundCell {
    CELL* Ucell;
    CELL* Lcell;
};

struct BoundPtr {
    Rptr Uptr;
    Rptr Lptr;
};

class WINDOW {

public:
    double xs, xe;
    int    rs, re;

    double GetW() { return xe - xs; }
    int GetTrack() { return re - rs + 1; }
    
};

class TIME {

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

public:
    void StartTimer() { start = std::chrono::high_resolution_clock::now(); }
    void EndTimer() { end = std::chrono::high_resolution_clock::now(); } 
    float Duration() { 
        std::chrono::duration<double> duration = end - start;
        return duration.count();
    }

};

#endif