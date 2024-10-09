#include <bits/stdc++.h>
#include "VEB.h"

#define F first 
#define S second

using namespace std;

// // functions defined in header file

VEB::VEB(int size) {
    u = size;
    min = -1;
    max = -1;
    
    if (size > 2) {
        int upperSize = ceil(sqrt(size));
        summary = new VEB(upperSize);
        cluster = new VEB*[upperSize];
        for (int i = 0; i < upperSize; i++) 
            cluster[i] = new VEB(upperSize);
    } 
    else { // top level
        summary = nullptr;
        cluster = nullptr;
    }
}

VEB::~VEB() {
    if(summary) delete summary;
    if(cluster) {
        int upperSize = ceil(sqrt(u));
        for (int i = 0; i < upperSize; i++) 
            delete cluster[i];
        delete [] cluster;
    }
}

void VEB::Insert(int x) {

    if (min == -1) {
        min = max = x;
    } else {
        if (x < min) swap(x, min);
        if (x > max) max = x;
        if (u > 2) {
            if (cluster[high(x)]->min == -1) {
                summary->Insert(high(x));
                cluster[high(x)]->Insert(low(x));
            } else {
                cluster[high(x)]->Insert(low(x));
            }
        }
    }

}

bool VEB::Member(int x) {
    if (x == min || x == max) {
            return true;
        } else if (u == 2) {
            return false;
        } else {
            return cluster[high(x)]->Member(low(x));
        }
}

int VEB::Successor(int x) {

    if (u == 2) {
        if (x == 0 && max == 1) return 1;
        else return -1;
    } 
    else if (min != -1 && x < min) return min;
    else {
        int maxLow = cluster[high(x)]->max;

        if (maxLow != -1 && low(x) < maxLow) {

            int offset = cluster[high(x)]->Successor(low(x));
            return index(high(x), offset);

        } else {

            int sucX = summary->Successor(high(x));
            if (sucX == -1) return -1;
            else {
                int offset = cluster[sucX]->min;
                return index(sucX, offset);
            }
            
        }

    }
}

int VEB::Predecessor(int x) {
    if (u == 2) {
        if (x == 1 && min == 0) return 0;
        else return -1;
    } 
    else if (max != -1 && x > max) return max;
    else {
        int minLow = cluster[high(x)]->min;

        if (minLow != -1 && low(x) > minLow) {
            int offset = cluster[high(x)]->Predecessor(low(x));
            return index(high(x), offset);
        } else {
            int predCluster = summary->Predecessor(high(x));
            if (predCluster == -1) {
                if (min != -1 && x > min) return min;
                else return -1;
            } else {
                int offset = cluster[predCluster]->max;
                return index(predCluster, offset);
            }
        }

    }
}

void VEB::Delete(int x) {

    if (min == max) {
        min = max = -1;
    } else if (u == 2) {
        if (x == 0) min = 1;
        else min = 0;
        max = min;
    } else {
        if (x == min) {
            int firstCluster = summary->min;
            x = index(firstCluster, cluster[firstCluster]->min);
            min = x;
        }

        cluster[high(x)]->Delete(low(x));

        if (cluster[high(x)]->min == -1) {
            summary->Delete(high(x));
            if (x == max) {
                int summaryMax = summary->max;
                if (summaryMax == -1) {
                    max = min;
                } else {
                    max = index(summaryMax, cluster[summaryMax]->max);
                }
            }
        } else if (x == max) {
            max = index(high(x), cluster[high(x)]->max);
        }
    }

}