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
    
    if (u <= 2) {
        summary = nullptr;
        cluster = nullptr;
    } else {
        int subsize = ceil(sqrt(u));  // Get the size of subproblems
        summary = new VEB(subsize);
        cluster = new VEB*[subsize];
        for (int i = 0; i < subsize; ++i) {
            cluster[i] = new VEB(subsize);  // Initialize cluster
        }
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
        min = x;
        max = x;
    }
    else {
        if (x < min) {
            swap(min, x);
        }
 
        // Not base case then...
        if (u > 2) {
            if (cluster[high(x)]->min == -1) {
                summary->Insert(high(x));
                cluster[high(x)]->min = low(x);
                cluster[high(x)]->max = low(x);
            }
            else {
                cluster[high(x)]->Insert(low(x));
            }
        }
 
        if (x > max) {
            max = x;
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


int VEB::Successor(int key) {
    if (u == 2) {
        if (key == 0 && max == 1) 
            return 1;
        else
            return -1;
    } else if (min != -1 && key < min) {
        return min;
    } else {
        int subsize = ceil(sqrt(u));
        int high = key / subsize, low = key % subsize; 
        int maxincluster = cluster[high]->max;
        
        int offset = 0, succcluster = 0;
        if (maxincluster != -1 && low < maxincluster) {
            offset = cluster[high]->Successor(low);
            return this->index(high, offset);
        } else {
            succcluster = summary->Successor(high);
            if (succcluster == -1) {
                return -1;
            } else {
                offset = cluster[succcluster]->min;
                return this->index(succcluster, offset);
            }
        }
    }
}

int VEB::Predecessor(int x) {
    // Base case: universe size is 2
    if (this->u == 2) {
        if (x == 1 && this->min == 0)
            return 0;
        else
            return -1;  // No predecessor
    }

    // If x is less than the minimum element, return -1 (no predecessor)
    if (x <= this->min)
        return -1;

    // If x is greater than or equal to the maximum, the max is the predecessor
    if (x > this->max)
        return this->max;

    // Get high and low parts of x
    int highX = high(x);
    int lowX = low(x);

    // Find the maximum element in the cluster `highX`
    if (cluster[highX] != nullptr && lowX > cluster[highX]->min) {
        int lowPredecessor = cluster[highX]->Predecessor(lowX);
        return index(highX, lowPredecessor);  // Combine high and low
    } else {
        // Look for the largest element smaller than highX in the summary
        int highPredecessor = summary->Predecessor(highX);
        if (highPredecessor == -1) {
            return this->min;  // No predecessor in the summary, return the minimum element
        } else {
            int maxLow = cluster[highPredecessor]->max;
            return index(highPredecessor, maxLow);  // Combine high and low parts
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