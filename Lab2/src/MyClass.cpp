#include <bits/stdc++.h>
#include "MyClass.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

// BLOCK =======================================
BLK::BLK(string Name, int width, int height) {
    name = Name;
    w = width;
    h = height;
}

pair<int, int> BLK::GetPoint() const {
    pair<int, int> point = make_pair(x+w/2, y+h/2);
    return point;
}

void BLK::Rotate() {
    int temp = w;
    w = h;
    h = temp;
}
// =============================================


// TERMINAL ====================================
TER::TER(string Name, int X, int Y) {
    name = Name;
    x = X;
    y = Y;
}

pair<int, int> TER::GetPoint() const {
    pair<int, int> point = make_pair(x, y);
    return point;
}
// =============================================


// NET =========================================
int NET::HPWL() {

    if(list.size() < 2) return 0;

    pair<int, int> max = list[0]->GetPoint();
    pair<int, int> min = list[0]->GetPoint();

    for(unsigned int i = 1; i < list.size(); i++) {
        pair<int, int> pt = list[i]->GetPoint();
        if(max.F < pt.F) max.F = pt.F;
        if(max.S < pt.S) max.S = pt.S;
        if(min.F < pt.F) min.F = pt.F;
        if(min.S < pt.S) min.S = pt.S;
    }

    return (max.F - min.F) + (max.S - min.S);
    
}

string NET::GetName(int index) {
    return list[index]->name;
}
// =============================================


// SequencePair ================================
void SequencePair::Shuffle() {
    random_device rd;
    mt19937 g(rd());
    shuffle(X.begin(), X.end(), g);
    shuffle(Y.begin(), Y.end(), g);
}

void SequencePair::Print() {
    cout << endl;
    cout << "X: ";
    for(unsigned int i = 0; i < X.size(); i++) {
        cout << X[i]->name << " ";
    }cout << endl;
    cout << "Y: ";
    for(unsigned int i = 0; i < Y.size(); i++) {
        cout << Y[i]->name << " ";
    }cout << endl << endl;
}


// =============================================