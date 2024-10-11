#include <bits/stdc++.h>
#include <random>
#include "SA.h"

#define F first 
#define S second

using namespace std;

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

void SA::Stage0(float Temp) { // Place within outline

    ofstream Pfile("floorplan/Prob.txt");
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    float T = Temp;
    float rate = 0.9995;
    float cost, cost_new, delta_cost;
    float P, prob;
    int iteration = 0;
    int Uphill = 0;
    int Downhill = 0;
    int last_Uphill = 0;

    cost = OutArea();

    while( OutofBound() && iteration < 50000 ) {
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
                Pfile << P << endl;
                Uphill ++;
                last_Uphill = iteration;
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
    cout << "       Temper   : " << Temp << endl;
    cout << "       iteration: " << iteration << endl;
    cout << "       Last Up  : " << last_Uphill << endl;
    cout << "       Uphill   : " << Uphill << endl; 
    cout << "       Downhill : " << Downhill << endl; 
    
    if(OutofBound()) Stage0(Temp * 0.005);

}

void SA::Walk() {

    SP_mem = SP;

    int dnum = 12;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, dnum);
    
    int opt = distr(gen);
    int seg = dnum / 3;

    if(opt == 0)          RotateBlk();
    else if(opt <= seg)   SwapX();
    else if(opt <= 2*seg) SwapY();
    else                  Swap();  

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