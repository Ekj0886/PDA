#include <bits/stdc++.h>
#include "Legalizer.h"

#define F first 
#define S second

using namespace std;

int main(int argc, char *argv[]) {

    string input_lg  = argv[1];
    string input_opt = argv[2];
    string output_lg = argv[3];

    LEGALIZER LGR;
    LGR.parse(input_lg);
    cout << LGR.alpha << " " << LGR.beta << endl;
    cout << LGR.Die.lowerX << " " << LGR.Die.lowerY << endl;
    cout << fixed << LGR.Die.upperX << " " << LGR.Die.upperY << endl;

    double x = 10;
    double y = 0.3;
    cout << (x / y == (int)(x / y)) << endl;

    LGR.DumpLayout("Layout/Die.txt");
    return 0;

}