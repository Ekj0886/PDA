#include <bits/stdc++.h>
#include "SA.h"

#define F first 
#define S second

using namespace std;

// Split the input line into inst segments
void GetLine(ifstream& infile, vector<string>& inst) { 
    inst.resize(0);
    string str;
    getline(infile, str);
    istringstream ss(str);
    string token;
    while (ss >> token) inst.push_back(token);
}

// Skip empty line in infile
void SkipEmpty(ifstream& infile, vector<string>& inst) {
    streampos pos;
    do {
        // cout << "empty" << endl;
        pos = infile.tellg();
        GetLine(infile, inst);
    } while(inst.size() == 0);
    infile.seekg(pos);
}

// functions defined in header file

void SA::LoadUnit(string file) {

    ifstream infile(file);
    vector<string> inst;

    // get outline
    GetLine(infile, inst); 
    W = stoi(inst[1]);
    H = stoi(inst[2]);

    // get block number
    GetLine(infile, inst);
    Blk_num = stoi(inst[1]);

    // get terminal number
    GetLine(infile, inst);
    Ter_num = stoi(inst[1]);

    SkipEmpty(infile, inst);
    
    // Load Block information
    for(int i = 0; i < Blk_num; i++) {
        GetLine(infile, inst);
        BLK* block = new BLK(inst[0], stoi(inst[1]), stoi(inst[2]));
        BlockList[inst[0]] = block;
    }

    SkipEmpty(infile, inst);

    // Load Terminal information
    for(int i = 0; i < Ter_num; i++) {
        GetLine(infile, inst);
        TER* terminal = new TER(inst[0], stoi(inst[2]), stoi(inst[3]));
        TerminalList[inst[0]] = terminal;
    }

    cout << "== Load Unit Done" << endl;

}

void SA::LoadNet(string file) {

    ifstream infile(file);
    vector<string> inst;

    GetLine(infile, inst);
    int net_num = stoi(inst[1]);

    for(int i = 0; i < net_num; i++) {

        GetLine(infile, inst);
        NET net(stoi(inst[1]));

        for(int q = 0; q < net.degree; q++) {
            GetLine(infile, inst);
            string unit = inst[0];
            
            UNIT* uptr;
            if(BlockList.find(unit) != BlockList.end())       uptr = BlockList[unit];
            if(TerminalList.find(unit) != TerminalList.end()) uptr = TerminalList[unit];
            net.list.push_back(uptr);
        }
        
        NetList.push_back(net);

    } 

    cout << "== Load Netlist Done" << endl;

}

