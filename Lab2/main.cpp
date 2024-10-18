#include <bits/stdc++.h>
#include "SP.h"
#include "BS.h"

#define F first 
#define S second

using namespace std;

std::string GetStringAfterSlash(const std::string& input) {
    size_t pos = input.find_last_of('/');
    if (pos != std::string::npos) return input.substr(pos + 1);
    return input;
}

int main(int argc, char *argv[]) {

    auto start = std::chrono::high_resolution_clock::now();
    
    ofstream outdraw("draw");
    outdraw << "python3 plot.py" << endl;

    float alpha = stof(argv[1]);
    string Unitfile = argv[2];
    string Netfile   = argv[3];
    string output    = argv[4];
    string output_name = GetStringAfterSlash(output);

    ofstream outcheck("check", ios::app);
    outcheck << "./verifier " << alpha << " " << Unitfile << " " << Netfile << " " << output << endl;
    outcheck << "rm " << output << ".HPWL" << endl;

    cout << "FloorPlan on case " << output_name << endl;

    // SP_FP SP_FloorPlan(alpha);
    
    // SP_FloorPlan.LoadUnit(Unitfile);
    // SP_FloorPlan.LoadNet(Netfile);
    
    // SP_FloorPlan.Init();
    // SP_FloorPlan.DumpFloorPlan("Init_" + output_name);
    
    // SP_FloorPlan.Stage0(1e6);
    // SP_FloorPlan.DumpFloorPlan("Stage0_" + output_name);

    // SP_FloorPlan.Stage1(35000);
    // SP_FloorPlan.DumpFloorPlan("Stage1_" + output_name);
    
    // SP_FloorPlan.Stage2(35000);
    // SP_FloorPlan.DumpFloorPlan("Stage2_" + output_name);


    BSTAR BStar_FloorPlan;

    BStar_FloorPlan.LoadUnit(Unitfile);
    BStar_FloorPlan.LoadNet(Netfile);

    BStar_FloorPlan.Init();



    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    float runtime = duration.count();

    // SP_FloorPlan.DumpOutput(output, runtime);
    std::cout << "Execution time: " << runtime << " seconds" << std::endl << std::endl;

    return 0;

}