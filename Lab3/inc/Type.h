#ifndef TYPE_H
#define TYPE_H
#include <bits/stdc++.h>

#define F first 
#define S second

struct CompareByX {
    bool operator()(const CELL* a, const CELL* b) const {
        return a->LEFT() < b->LEFT();
    }
};

using RowIterator = std::optional<std::set<CELL*, CompareByX>::iterator>;
using Rptr = std::set<CELL*, CompareByX>::iterator;


struct DIE {
    double lowerX, lowerY;
    double upperX, upperY;
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