#include <bits/stdc++.h>
#include "MyClass.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

pair<int, int> BLK::GetPoint() const {
    pair<int, int> point = make_pair(x+w/2, y+h/2);
    return point;
}

pair<int, int> BLK::GetPoint() const {
    return pair<int, int>();
}

void BLK::Rotate() {
    int temp = w;
    w = h;
    h = temp;
}

pair<int, int> TER::GetPoint() const {
    pair<int, int> point = make_pair(x, y);
    return point;
}


int NET::HPWL() {

    if(net.size() < 2) return 0;

    pair<int, int> max = net[0]->GetPoint();
    pair<int, int> min = net[0]->GetPoint();

    for(unsigned int i = 1; i < net.size(); i++) {
        pair<int, int> pt = net[i]->GetPoint();
        if(max.F < pt.F) max.F = pt.F;
        if(max.S < pt.S) max.S = pt.S;
        if(min.F < pt.F) min.F = pt.F;
        if(min.S < pt.S) min.S = pt.S;
    }

    return (max.F - min.F) + (max.S - min.S);
    
}