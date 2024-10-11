#include <bits/stdc++.h>
#include "SA.h"

#define F first 
#define S second

using namespace std;

int main(int argc, char *argv[]) {

     auto start = std::chrono::high_resolution_clock::now();

    // ifstream infile (argv[1]);
    // ofstream outfile(argv[2]);
    
    ofstream outdraw("draw");
    outdraw << "python3 plot.py" << endl;
    
    int alpha = stoi(argv[1]);
    string Unitfile = argv[2];
    string Netfile   = argv[3];
    string output    = argv[4];

    SA Simulated_Annealing(alpha);

    Simulated_Annealing.LoadUnit(Unitfile);
    Simulated_Annealing.LoadNet(Netfile);
    Simulated_Annealing.Init();
    Simulated_Annealing.DumpFloorPlan("Init");
    Simulated_Annealing.Stage0(1e6);
    Simulated_Annealing.DumpFloorPlan("Stage0");


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    return 0;

}