#ifndef TYPE_H
#define TYPE_H
#include <bits/stdc++.h>

#define F first 
#define S second

using namespace std;


enum Direction {
    Vertical,
    Horizontal,
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

class RouteArea {

public:
    RouteArea() {};
    int x, y, w, h;
    int LEFT() { return x; }
    int DOWN() { return y; }
    int RIGHT() { return x + w;}
    int TOP() { return y + h; }
    
};

class Chip {

public:
    Chip() {};
    Chip(int X, int Y, int W, int H) {
        x = X;
        y = Y;
        w = W;
        h = H;
    }
    int x, y, w, h;
    int Left () { return x; }
    int Down () { return y; }
    int Right() { return x + w; }
    int Top  () { return y + h; }
    
};

class Gcell {

public:
    Gcell() {};
    double m1_cost, m2_cost;
    int left_cap, down_cap;
    Gcell* L;
    Gcell* R;
    Gcell* D;
    Gcell* U;
    
    double cost;
    Gcell* parent; 
    Direction dir;

    // debud purpose
    int x, y;

};


struct Net {
    int src_x, src_y;
    int snk_x, snk_y;
};

struct CompareByCost {
    bool operator()(const Gcell* a, const Gcell* b) const {
        return a->cost < b->cost;
    }
};

using Set = multiset<Gcell*, CompareByCost>;

#endif