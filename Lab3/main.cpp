#include <bits/stdc++.h>
#include "Legalizer.h"

#define F first 
#define S second

using namespace std;
// using time = std::chrono::time_point<std::chrono::high_resolution_clock>;

int main(int argc, char *argv[]) {

    string input_lg  = argv[1];
    string input_opt = argv[2];
    string output_lg = argv[3];

    TIME clock;

    clock.StartTimer();

    LEGALIZER LGR;
    LGR.parse(input_lg);
    LGR.PlaceCell();
    // LGR.RunOpt(input_opt);
    


    clock.EndTimer();
    cout << "Runtime: " << clock.Duration() << endl;

    LGR.DumpLayout("Layout/Die.txt");

    return 0;

}