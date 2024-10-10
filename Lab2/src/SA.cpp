#include <bits/stdc++.h>
#include <random>
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
        // std::cout << "empty" << endl;
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

    Blk_area = 0;
    for(int i = 0; i < Blk_num; i++) {
        GetLine(infile, inst);
        BLK* block = new BLK(inst[0], stoi(inst[1]), stoi(inst[2]));
        Blk_area += (stoi(inst[1])*stoi(inst[2]));
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

    std::cout << "== Load Unit Done" << endl;

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

    std::cout << "== Load Netlist Done" << endl;

}


void SA::GetCoordinate() {

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

void SA::DumpFloorPlan(string file) {
    
    ofstream outfile("floorplan/" + file + ".txt");
    ofstream outdraw("draw", ios::app);

    outfile << Blk_num << endl;
    outfile << W << " " << H << endl;
    for (auto it = BlockList.begin(); it != BlockList.end(); ++it) {
        BLK* blk = it->S;
        outfile << blk->name << " " << blk->x << " " << blk->y << " " << blk->w << " " << blk->h << endl;
    }

    outdraw << "python3 draw.py floorplan/" << file << ".txt " << file << endl;

}

void SA::RotateBlk() {

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

void SA::SwapX() {
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, SP.X.size() - 1);
    int index1 = distr(gen);
    int index2 = distr(gen);
    while (index2 == index1) index2 = distr(gen);
    
    swap(SP.X[index1], SP.X[index2]);
    
}

void SA::SwapY() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, SP.Y.size() - 1);
    int index1 = distr(gen);
    int index2 = distr(gen);
    while (index2 == index1) index2 = distr(gen);
    
    swap(SP.Y[index1], SP.Y[index2]);

}

void SA::Swap() {
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

bool SA::Outside(BLK* blk) {
    if(blk->x + blk->w > W || blk->y + blk->h > H) return true;
    else return false;
}

float SA::OutArea() {
    GetCoordinate();
    long long cost = 0;
    for (auto it = BlockList.begin(); it != BlockList.end(); ++it) {
        BLK* blk = it->S;
        if(Outside(blk)) {
            cost += (blk->w*blk->h);
        }
    }
    return (float) 1000 * Blk_num * cost / Blk_area;
}

void SA::Init() {
    SP.Shuffle();
    float cost = OutArea();
    int i = 0;
    while( OutofBound() && i < 5000 ) {
        i++;
        Walk();
        float cost_nxt = OutArea();
        if(cost_nxt > cost) ReverseWalk();
        else {
            cost = cost_nxt;
        }
    }
    cout <<  "== Init Floorplan Done " << "cost: " << cost << endl;
}

void SA::Stage0(float T) { // Place within outline
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    // float T = 20000;
    float rate = 0.9995;
    float cost, cost_new, delta_cost;
    float P, prob;
    int iteration = 0;
    int Uphill = 0;
    int Downhill = 0;

    cost = OutArea();

    while( OutofBound() && iteration < 100000 ) {
        iteration ++;
        Walk();
        cost_new = OutArea();
        delta_cost = cost_new - cost;
        if(delta_cost <= 0) {
            Downhill ++;
            cost = cost_new;
        } 
        else {
            P = exp( (float) -1 * delta_cost / T );
            prob = dis(gen);
            if(prob <= P) {
                Uphill ++;
                cost = cost_new;
            }
            else {
                ReverseWalk();
            }
        }
        T *= rate;
    }

    cout <<  "== Stage0 Floorplan " << endl;
    cout << "       Cost     : " << cost << endl;
    cout << "       Temper   : " << T << endl;
    cout << "       iteration: " << iteration << endl;
    cout << "       Uphill   : " << Uphill << endl; 
    cout << "       Downhill : " << Downhill << endl; 
    
    if(OutofBound()) {
        // cout << "   Redo" << endl;
        Stage0(T * 0.9);
    }

}

void SA::Walk() {

    SP_mem = SP;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, 6);
    
    int opt = distr(gen);

    switch(opt) {
        case 0: RotateBlk();
        case 1: SwapX();
        case 2: SwapY();
        case 3: Swap();
        case 4: SwapX();
        case 5: SwapY();
        default: Swap();
    }

}

void SA::ReverseWalk() {
    SP = SP_mem;
    if(rFlag.length() > 0) {
        BLK* blk = BlockList[rFlag];
        blk->Rotate();       
    }
    rFlag = "";
    GetCoordinate();
}