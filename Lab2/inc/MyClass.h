#ifndef MYCLASS_H
#define MYCLASS_H
#include <bits/stdc++.h>

#define F first 
#define S second

using namespace std;

class UNIT { // Base class

public:
    virtual pair<int, int> GetPoint() const = 0;

};

class BLK : public UNIT {

public:
    string name;
    int w, h;
    int x, y; // lower left

    pair<int, int> GetPoint() const override;
    void Rotate();

};

class TER : public UNIT {

public:
    string name;
    int x, y;

    pair<int, int> GetPoint() const override;

};

class NET {

public:
    vector<UNIT*> net;
    int HPWL();

};


typedef vector<BLK*> SequencePair;

#endif