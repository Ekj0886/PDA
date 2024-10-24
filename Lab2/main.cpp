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
    
    ofstream outdraw("draw");

    double alpha = stof(argv[1]);
    string Unitfile = argv[2];
    string Netfile   = argv[3];
    string output    = argv[4];
    string output_name = GetStringAfterSlash(output);

    ofstream outcheck("check", ios::app);
    outcheck << "./verifier " << alpha << " " << Unitfile << " " << Netfile << " " << output << endl;
    outcheck << "rm " << output << ".HPWL" << endl;

    cout << endl << "<< FloorPlan on case " << output_name << " >>" << endl;

    cout << endl << "< SP start >" << endl;
    SP_FP SP_FloorPlan(alpha);
    
    SP_FloorPlan.LoadUnit(Unitfile);
    SP_FloorPlan.LoadNet(Netfile);
    
    SP_FloorPlan.Init();
    SP_FloorPlan.DumpFloorPlan("SP_Init_" + output_name);
    
    SP_FloorPlan.Stage0(1e6);
    SP_FloorPlan.DumpFloorPlan("SP_Stage0_" + output_name);

    SP_FloorPlan.Stage1(35000);
    SP_FloorPlan.DumpFloorPlan("SP_Stage1_" + output_name);
    
    SP_FloorPlan.Stage2(35000);
    SP_FloorPlan.LoadBest();
    SP_FloorPlan.DumpFloorPlan("SP_Stage2_" + output_name);

    float runtime = SP_FloorPlan.GetTime();

    SP_FloorPlan.DumpOutput(output, runtime);

    std::cout << "Execution time: " << runtime << " seconds" << std::endl << std::endl;

    return 0;

}
