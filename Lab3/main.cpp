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
    LGR.PlaceCell();
    LGR.RunOpt(input_opt);
    
    // LGR.DumpLayout("Layout/Die.txt");
    return 0;

}