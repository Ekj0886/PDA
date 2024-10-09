#ifndef VEB_H
#define VEB_H
#include <bits/stdc++.h>

#define F first 
#define S second

using namespace std;


class VEB {

public:
    int u;
    int min, max;
    VEB* summary;
    VEB** cluster;

    VEB(int size);
    ~VEB();

    int high(int x) { return x / (int)sqrt(u); }
    int low(int x) { return x % (int)sqrt(u); }
    int index(int high, int low) { return high * (int)sqrt(u) + low; }

    void Insert(int x);
    bool Member(int x);
    int Successor(int x);
    int Predecessor(int x);
    void Delete(int x);

};



#endif