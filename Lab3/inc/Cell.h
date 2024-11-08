#ifndef CELL_H
#define CELL_H
#include <bits/stdc++.h>

#define F first 
#define S second

using namespace std;

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

    // setup function
    void SetXY(double, double);

    // get function
    string GetName() { return name; }
    double GetW() { return w; }
    double GetH() { return h; }
    double GetX() { return x; }
    double GetY() { return y; }
    bool   Fix() { return fix; }
    
};


#endif