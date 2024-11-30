#include <bits/stdc++.h>
#include"GR.h"

#define F first 
#define S second

using namespace std;

int main(int argc, char *argv[]) {

    string input_gmp  = argv[1];
    string input_gcl  = argv[2];
    string input_cst  = argv[3];
    string output_lg  = argv[4];

    TIME clock;
    clock.StartTimer();

    GR Global_Router;
    Global_Router.Parse(input_gmp, input_gcl, input_cst);
    Global_Router.SetGrid();
    Global_Router.DirectRoute();

    clock.EndTimer();
    cout << "Runtime: " << clock.Duration() << endl;
    
    return 0;

}