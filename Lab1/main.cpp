#include <bits/stdc++.h>
#include "CS.h"

#define F first 
#define S second

using namespace std;

void split(const string& str, vector<string>& result) {
    istringstream ss(str);
    string token;
    while (ss >> token) result.push_back(token);
}

int main(int argc, char *argv[]) {


//  === self check io setting ==================
    // cout << "Start " << argv[1] << endl;
    // string output = "output/" + string(argv[2]);
    // ifstream infile (argv[1]);
    // ofstream outfile(argv[2]);
    // ofstream outmap("Maplog.txt");
// =============================================

// === homework setting ========================
    string output = argv[2];
    ifstream infile (argv[1]);
    ofstream outfile(output);
// =============================================

    ofstream outdraw(argv[2]);

    CS cs; // corner-stitching
    
    infile >> cs.w >> cs.h;
    cs.Init_Outline();
    
    string line;
    getline(infile, line); // skip to the next line

    // get each line of instruction
    while(getline(infile, line)) {
        vector<string> inst;
        split(line, inst);

        if(line[0] == 'P') { 
            // Point Finding
            TILE* t = cs.Find_Point(stoi(inst[1]), stoi(inst[2]));
            cs.Point_vec.push_back(t->pt);
        }
        else {
            // Block creation
            cs.Create_Blk(stoi(inst[0]), stoi(inst[1]), stoi(inst[2]), stoi(inst[3]), stoi(inst[4]));
        }
    }
    cs.Dump_Output(output);
    cs.Dump_Layout(argv[2]);

//  === self check io setting ==================
    // cs.Dump_Layout(argv[2]);
    // cout << argv[1] << " Done" << endl << endl;
// =============================================

    return 0;

}