#include <bits/stdc++.h>
#include "SA.h"

#define F first 
#define S second

using namespace std;

int main(int argc, char *argv[]) {

    // ifstream infile (argv[1]);
    // ofstream outfile(argv[2]);
    
    int alpha = stoi(argv[1]);
    string Unitfile = argv[2];
    string Netfile   = argv[3];
    string output    = argv[4];

    SA Simulated_Annealing(alpha);

    Simulated_Annealing.LoadUnit(Unitfile);
    Simulated_Annealing.LoadNet(Netfile);
    Simulated_Annealing.Init();


    return 0;

}