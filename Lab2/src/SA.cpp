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

    if(Blk_num == 0) return;

    SkipEmpty(infile, inst);
    
    // Load Block information
    for(int i = 0; i < Blk_num; i++) {
        GetLine(infile, inst);
        BLK* block = new BLK(inst[0], stoi(inst[1]), stoi(inst[2]));
        BlockList[inst[0]] = block;
        SP.X.push_back(block);
        SP.Y.push_back(block);
    }

    if(Ter_num == 0) return; 

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

    if(net_num == 0) return;

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



void SA::GetCoordinate() {

    // VEB Vx(Blk_num + 1);
    // vector<int> BUCKLx(Blk_num + 1);

    // Vx.Insert(0);
    // BUCKLx[0] = 0;

    // for(int i = 0; i < Blk_num; i++) {
    //     BLK* blk = SP.X[i];
    //     cout << endl << blk->name << endl;
    //     auto ptr = find(SP.Y.begin(), SP.Y.end(), blk);
    //     int p = distance(SP.Y.begin(), ptr);
    //     p++;
    //     cout << "p: " << p << endl;

    //     Vx.Insert(p);
    //     cerr << "Inserted" << endl;
    //     cout << "Pre: " << Vx.Predecessor(p) << endl;
    //     blk->x = BUCKLx[Vx.Predecessor(p)];
    //     cout << blk->x << endl;
    //     BUCKLx[p] = blk->x + blk->w;
    //     cout << "BUCK p: " << BUCKLx[p] << endl;

    //     int s = Vx.Successor(p);
    //     cout << "s: " << s << endl;
    //     while(s != -1) {
    //         if(BUCKLx[s] <= BUCKLx[p]) {
    //             int s_nxt = Vx.Successor(s);
    //             Vx.Delete(s);
    //             if(s_nxt == -1) break;
    //             s = s_nxt;
    //         } 
    //     } 
    // }

    // VEB Vy(Blk_num + 1);
    // vector<int> BUCKLy(Blk_num + 1);

    // Vy.Insert(0);
    // BUCKLy[0] = 0;

    // for(int i = Blk_num-1; i >= 0; i--) {
    //     BLK* blk = SP.X[i];
    //     auto ptr = find(SP.Y.begin(), SP.Y.end(), blk);
    //     int p = distance(SP.Y.begin(), ptr);

    //     Vy.Insert(p);
    //     blk->y = BUCKLy[Vy.Predecessor(p)];
    //     BUCKLy[p] = blk->y + blk->h;

    //     int s = Vy.Successor(p);
    //     while(s != -1) {
    //         if(BUCKLy[s] <= BUCKLy[p]) {
    //             int s_nxt = Vy.Successor(s);
    //             Vy.Delete(s);
    //             if(s_nxt == -1) break;
    //             s = s_nxt;
    //         } 
    //     } 
    // }

}

void SA::Init() { 

    SP.Shuffle();
    SP.Print();

    // cout << Blk_num + 1 << endl;
    VEB v(10);
    v.Insert(0);
    v.Insert(4);
    v.Insert(2);
    // v.Insert(15);
    cout << v.Predecessor(2) << endl;
    cout << v.Successor(2) << endl;
    cout << v.Predecessor(4) << endl;
    cout << v.Successor(4) << endl;

    // GetCoordinate();

}