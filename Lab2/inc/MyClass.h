#ifndef MYCLASS_H
#define MYCLASS_H
#include <bits/stdc++.h>

#define F first 
#define S second

using namespace std;

class UNIT { // Base class

public:
    string name;
    virtual pair<int, int> GetPoint() const = 0;

};

class BLK : public UNIT {

public:
    int w, h;
    int x, y; // lower left point

    BLK(string Name, int width, int height);

    pair<int, int> GetPoint() const override;
    void Rotate();

};

class TER : public UNIT {

public:
    int x, y; // single point in terminal

    TER(string Name, int X, int Y);

    pair<int, int> GetPoint() const override;

};

class NET {

public:
    int degree;
    vector<UNIT*> list;

    NET() {};
    NET(int Degree) { degree = Degree; }

    int HPWL();
    string GetName(int index);

};


class SequencePair {
public:
    vector<BLK*> X;
    vector<BLK*> Y;

    void Shuffle();
    void Print();

};

#endif