#include <bits/stdc++.h>
#include "SP.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file
// Split the input line into inst segments
void SP_FP::GetLine(ifstream& infile, vector<string>& inst) { 
    inst.resize(0);
    string str;
    getline(infile, str);
    istringstream ss(str);
    string token;
    while (ss >> token) inst.push_back(token);
}

// Skip empty line in infile
void SP_FP::SkipEmpty(ifstream& infile, vector<string>& inst) {
    streampos pos;
    do {
        // std::cout << "empty" << endl;
        pos = infile.tellg();
        GetLine(infile, inst);
    } while(inst.size() == 0);
    infile.seekg(pos);
}

// functions defined in header file

void SP_FP::LoadUnit(string file) {

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

    Blk_area = 0;
    for(int i = 0; i < Blk_num; i++) {
        GetLine(infile, inst);
        BLK* block = new BLK(inst[0], stoi(inst[1]), stoi(inst[2]));
        Blk_area += (stoi(inst[1])*stoi(inst[2]));
        BlockList[inst[0]] = block;
    }

    if(Ter_num == 0) return; 

    SkipEmpty(infile, inst);

    // Load Terminal information
    for(int i = 0; i < Ter_num; i++) {
        GetLine(infile, inst);
        TER* terminal = new TER(inst[0], stoi(inst[2]), stoi(inst[3]));
        TerminalList[inst[0]] = terminal;
    }

    std::cout << "== Load Unit Done" << endl;

}

void SP_FP::LoadNet(string file) {

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

    std::cout << "== Load Netlist Done" << endl;

}


void SP_FP::GetCoordinate() {

    W_fp = 0;
    H_fp = 0;

    VEB Vx(Blk_num + 1);
    vector<int> BUCKLx(Blk_num + 1);

    Vx.Insert(0);
    BUCKLx[0] = 0;

    for(int i = 0; i < Blk_num; i++) {
        BLK* blk = SP.X[i];
        auto ptr = find(SP.Y.begin(), SP.Y.end(), blk);
        int p = distance(SP.Y.begin(), ptr);
        p++;

        Vx.Insert(p);
        blk->x = BUCKLx[Vx.Predecessor(p)];
        
        if(W_fp < blk->x + blk->w) W_fp = blk->x + blk->w;

        BUCKLx[p] = blk->x + blk->w;

        int s = Vx.Successor(p);
        while(s != -1) {
            int s_nxt = Vx.Successor(s);
            if(BUCKLx[s] <= BUCKLx[p]) {
                Vx.Delete(s);
                if(s_nxt == -1) break;
            } 
            s = s_nxt;
        } 
    }



    VEB Vy(Blk_num + 1);
    vector<int> BUCKLy(Blk_num + 1);

    Vy.Insert(0);
    BUCKLy[0] = 0;

    for(int i = Blk_num-1; i >= 0; i--) {
        BLK* blk = SP.X[i];
        auto ptr = find(SP.Y.begin(), SP.Y.end(), blk);
        int p = distance(SP.Y.begin(), ptr);
        p++;

        Vy.Insert(p);
        blk->y = BUCKLy[Vy.Predecessor(p)];

        if(H_fp < blk->y + blk->h) H_fp = blk->y + blk->h;

        BUCKLy[p] = blk->y + blk->h;

        int s = Vy.Successor(p);
        while(s != -1) {
            int s_nxt = Vy.Successor(s);
            if(BUCKLy[s] <= BUCKLy[p]) {
                Vy.Delete(s);
                if(s_nxt == -1) break;
            } 
            s = s_nxt;
        }
    }


}

void SP_FP::DumpRatio() {
    for (auto it = BlockList.begin(); it != BlockList.end(); ++it) {
        BLK* blk = it->S;
        Ratio_mem[blk] = make_pair(blk->w, blk->h);
    }
}

void SP_FP::LoadRatio() {
    for (auto it = BlockList.begin(); it != BlockList.end(); ++it) {
        BLK* blk = it->S;
        pair<int, int> ratio = Ratio_mem[blk];
        blk->w = ratio.F;
        blk->h = ratio.S;
    }
}

float SP_FP::GetTime() {
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    float runtime = duration.count();
    return runtime;
}

void SP_FP::SaveBest() {
    DumpRatio();
    Best_Cost = Cost();
    SP_Best = SP;
}

void SP_FP::LoadBest() { 
    LoadRatio();
    SP = SP_Best;
    GetCoordinate();
    cout << "== Load Best" << endl;
    std::cout << "       Area     : " << (long long)W_fp*H_fp << " ( " << W_fp << ", " << H_fp << " )" << endl;
    std::cout << "       Wire     : " << Wire() << endl;
    std::cout << "       Cost     : " << Cost() << endl;
}

void SP_FP::RotateBlk() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, SP.X.size() - 1);
    BLK* blk = SP.X[distr(gen)];
    // std::cout << blk->name << endl;
    // std::cout << blk->w << " " << blk->h << endl;
    rFlag = blk->name;
    blk->Rotate();

    // std::cout << blk->w << " " << blk->h << endl;

}

void SP_FP::SwapX() {
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, SP.X.size() - 1);
    int index1 = distr(gen);
    int index2 = distr(gen);
    while (index2 == index1) index2 = distr(gen);
    
    swap(SP.X[index1], SP.X[index2]);
    
}

void SP_FP::SwapY() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, SP.Y.size() - 1);
    int index1 = distr(gen);
    int index2 = distr(gen);
    while (index2 == index1) index2 = distr(gen);
    
    swap(SP.Y[index1], SP.Y[index2]);

}

void SP_FP::Swap() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, SP.X.size() - 1);
    int index1 = distr(gen);
    int index2 = distr(gen);
    while (index2 == index1) index2 = distr(gen);
    
    auto ptr = find(SP.Y.begin(), SP.Y.end(), SP.X[index1]);
    int q1 = distance(SP.Y.begin(), ptr);
    ptr = find(SP.Y.begin(), SP.Y.end(), SP.X[index2]);
    int q2 = distance(SP.Y.begin(), ptr);

    swap(SP.X[index1], SP.X[index2]);
    swap(SP.Y[q1], SP.Y[q2]);
}

bool SP_FP::Outside(BLK* blk) {
    if(blk->x + blk->w > W || blk->y + blk->h > H) return true;
    else return false;
}

float SP_FP::DeadSpace() {
    GetCoordinate(); 
    return (float) 5000 * (W_fp*H_fp - Blk_area)/(W_fp*H_fp); 
}

float SP_FP::OutArea() {
    GetCoordinate();
    long long cost = 0;
    for(auto it = BlockList.begin(); it != BlockList.end(); ++it) {
        BLK* blk = it->S;
        if(Outside(blk)) {
            cost += (blk->w*blk->h);
        }
    }
    return (float) 1000 * Blk_num * cost / Blk_area;
}

int SP_FP::Wire() {

    GetCoordinate();
    int wire_length = 0;
    for(auto n : NetList) {
        wire_length += n.HPWL();
    }
    return wire_length;
}


int SP_FP::Cost() {

    GetCoordinate();
    double wire_length = 0;
    for(auto n : NetList) {
        wire_length += n.HPWL();
    }

    int cost = floor( (double)(alpha*W_fp*H_fp  + (1.0 - alpha)*wire_length) ); 

    return cost; 

}