#include <bits/stdc++.h>
#include "BS.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file
void BSTAR::Walk() {

    for(auto n : NodeList) n->UpdateMem();

    int dnum = 8;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, dnum);

    int opt = distr(gen);
    int seg = dnum / 2;
    
    if(opt == 0)        RotateBlk(); 
    else if(opt <= seg) SwapNode();  
    else                MoveNode();  

}


void BSTAR::ReverseWalk() {
    for(auto n : NodeList) n->Recall();
    if(rFlag.length() > 0) {
        BLK* blk = BlockList[rFlag];
        blk->Rotate();       
    }
    rFlag = "";
    GetCoordinate();

}


void BSTAR::Init() {

    root = BuildTree();

    float cost = OutArea();
    int iteration = 0;
    // PrintTree();

    while( OutofBound() && iteration < 5000 ) {
        iteration++;
        Walk();
        float cost_nxt = OutArea();
        // cout << "cost_nxt: " << cost_nxt << endl;
        if(cost_nxt > cost) ReverseWalk();
        else {
            cost = cost_nxt;
        }
    }
    std::cout <<  "== Init Floorplan" << endl;
    std::cout << "       Area     : " << (long long)W_fp*H_fp << " ( " << W_fp << ", " << H_fp << " )" << endl;
    std::cout << "       Wire     : " << Wire() << endl;
    std::cout << "       Cost     : " << Cost() << endl;
    std::cout << "       iteration: " << iteration << endl;

}


void BSTAR::Stage0(float Temp) { // Place within outline

    if(!OutofBound()) {
        std::cout << "== Stage0 Skipped (Already inside outline)" << endl;
        // std::cout << "       Area     : " << (long long)W_fp*H_fp << " ( " << W_fp << ", " << H_fp << " )" << endl;
        return;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    float T = Temp;
    float rate = 0.998;
    float cost, cost_new, delta_cost;
    float P, prob;
    int iteration = 0;
    int iter_num = Blk_num*450;

    cost = OutArea();

    while( OutofBound() && iteration < iter_num ) {
        iteration ++;
        Walk();
        cost_new = OutArea();
        delta_cost = cost_new - cost;
        if(delta_cost <= 0) {
            cost = cost_new;
        } 
        else {
            P = exp( (float) -1 * delta_cost / T );
            prob = dis(gen);
            if(prob <= P) {
                cost = cost_new;
            }
            else {
                ReverseWalk();
            }
        }
        T *= rate;
    }

    std::cout <<  "== Stage0 Floorplan " << endl;
    std::cout << "       Area     : " << (long long)W_fp*H_fp << " ( " << W_fp << ", " << H_fp << " )" << endl;
    std::cout << "       Wire     : " << Wire() << endl;
    std::cout << "       Cost     : " << Cost() << endl;
    std::cout << "       iteration: " << iteration << endl; 
    
    if(OutofBound()) Stage0(Temp * 0.005);

}


void BSTAR::Stage1(float Temp) { // Reduce DeadSpace

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    float T = Temp;
    float rate = 0.99;
    float cost, cost_new, delta_cost;
    float P, prob;
    int iteration = 0;
    int iter_num = min( (int)6e6/Blk_num, 100000 );

    if(alpha >= 0.5) cost = DeadSpace();
    else             cost = Wire(); 

    while( iteration < iter_num ) {
        iteration ++;
        Walk();

        if(alpha > 0.5) cost_new = DeadSpace();
        else            cost_new = Wire(); 

        delta_cost = cost_new - cost;

        if(OutofBound()) {
            ReverseWalk();
        }
        else if(delta_cost <= 0) {
            cost = cost_new;
        } 
        else {
            P = exp( (float) -0.1 * delta_cost / T );
            prob = dis(gen);
            if(prob <= P) {
                cost = cost_new;
            }
            else {
                ReverseWalk();
            }
        }
        T *= rate;
    }

    std::cout <<  "== Stage1 Floorplan " << endl;
    std::cout << "       Area     : " << (long long)W_fp*H_fp << " ( " << W_fp << ", " << H_fp << " )" << endl;
    std::cout << "       Wire     : " << Wire() << endl;
    std::cout << "       Cost     : " << Cost() << endl;
    std::cout << "       iteration: " << iteration << endl; 

}


void BSTAR::Stage2(float Temp) { // Reduce DeadSpace

    if(alpha == 0) {
        std::cout << "== Stage2 Skipped (Alpha is 0)" << endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    float T = Temp;
    float rate = 0.99;
    float cost, cost_new, delta_cost;
    float P, prob;
    int iteration = 0;
    int iter_num = min( (int)6e6/Blk_num, 100000 );
    float wire = 0;
    
    GetCoordinate();
    wire = Wire();
    cost = (float) (alpha * (W_fp * H_fp) + (1.0 - alpha) * wire);

    while( iteration < iter_num ) {
        iteration ++;
        Walk();

        GetCoordinate();
        wire = Wire();
        cost_new = (float) (alpha * (W_fp * H_fp) + (1 - alpha) * wire);
        delta_cost = 0.0001*(cost_new - cost);

        if(OutofBound()) {
            ReverseWalk();
        }
        else if(delta_cost <= 0) {
            cost = cost_new;
        } 
        else {
            // P = exp( (float) -1 * delta_cost / T );
            P = 0;
            prob = dis(gen);
            if(prob <= P) {
                cost = cost_new;
            }
            else {
                ReverseWalk();
            }
        }
        T *= rate;
    }

    std::cout <<  "== Stage2 Floorplan " << endl;
    std::cout << "       Area     : " << (long long)W_fp*H_fp << " ( " << W_fp << ", " << H_fp << " )" << endl;
    std::cout << "       Wire     : " << Wire() << endl;
    std::cout << "       Cost     : " << Cost() << endl;
    std::cout << "       iteration: " << iteration << endl; 

}


void BSTAR::DumpFloorPlan(string file) {
    
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


void BSTAR::DumpOutput(string file, float runtime) {
    
    ofstream outfile(file);
    
    outfile << Cost() << endl;
    outfile << Wire() << endl;
    outfile << W_fp*H_fp << endl;
    outfile << W_fp << " " << H_fp << endl;
    outfile << runtime << endl;

    for (auto it = BlockList.begin(); it != BlockList.end(); ++it) {
        BLK* blk = it->S;
        outfile << blk->name << " " << blk->x << " " << blk->y << " ";
        outfile << blk->x + blk->w << " " << blk->y + blk->h << endl;
    }

    std::cout << "== Dump BSTAR Output" << endl; 

}

