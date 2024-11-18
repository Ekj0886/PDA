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

struct PT {
    double x, y;
    double dis; 
    PT(double X, double Y) : x(X), y(Y) {}
};

struct Mem {
    CELL* c;
    double xo, yo;
    Mem(CELL* cell, double x, double y) : c(cell), xo(x), yo(y) {}
};

struct CompareByDis {
    bool operator()(const PT a, const PT b) const {
        return a.dis < b.dis;
    }
};


using Set = set<CELL*, CompareByX>;
using Rptr = std::set<CELL*, CompareByX>::iterator;

struct DIE {
    double lowerX, lowerY;
    double upperX, upperY;
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