#ifndef CELL_H
#define CELL_H
#include <bits/stdc++.h>

#define F first 
#define S second

using namespace std;

struct DIE {
    double lowerX, lowerY;
    double upperX, upperY;
};

class CELL {

private:
    string name;
    double w, h;
    double x, y;
    bool   fix;  // 0 for not fix, 1 for fix
    
public:
    CELL();
    CELL(string, double, double, double, double, bool);
    ~CELL();

    bool merge, pseudo;

    // setup function
    void SetXY(double, double);

    // get function
    string GetName() const { return name; }
    double GetW() const { return w; }
    double GetH() const { return h; }
    double LEFT() const { return x; }
    double DOWN() const { return y; }
    double RIGHT() const { return x + GetW(); }
    double TOP() const { return y + GetH(); }
    bool   Fix() const { return fix; }
};


#endif