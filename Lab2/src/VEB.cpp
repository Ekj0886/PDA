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
        cout << "Summary: " << upperSize << endl;
        cluster = new VEB*[upperSize];
        cout << "Cluster: " << upperSize << endl;
        for (int i = 0; i < upperSize; i++) {
            cout << "sub cluster " << upperSize << endl;
            cluster[i] = new VEB(upperSize);
        }
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
    // Base case: if universe size is 2
    if (this->u == 2) {
        if (x == 0) {
            this->min = 0;
        } else {
            this->max = 1;
        }
        return;
    }

    // If the tree is empty, set min and max
    if (this->min == -1) {
        this->min = this->max = x;
        return;
    }

    // If x is smaller than the current minimum, swap min and x
    if (x < this->min) {
        std::swap(x, this->min);
    }

    // If x is greater than the current maximum, update the maximum
    if (x > this->max) {
        this->max = x;
    }

    // If the tree has more than two elements, proceed recursively
    if (this->u > 2) {
        int highX = high(x);
        int lowX = low(x);

        // If the cluster is empty, insert into the summary and cluster
        if (cluster[highX]->min == -1) {
            summary->Insert(highX);
            cluster[highX]->min = cluster[highX]->max = lowX;
        } else {
            cluster[highX]->Insert(lowX);
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

// int VEB::Successor(int x) {
//     if (u == 2) {
//         if (x == 0 && max == 1) return 1;
//         else return -1;
//     } 
//     else if (min != -1 && x < min) return min;
//     else {
//         int maxLow = cluster[high(x)]->max;

//         if (maxLow != -1 && low(x) < maxLow) {

//             int offset = cluster[high(x)]->Successor(low(x));
//             return index(high(x), offset);

//         } else {

//             int sucX = summary->Successor(high(x));
//             if (sucX == -1) return -1;
//             else {
//                 int offset = cluster[sucX]->min;
//                 return index(sucX, offset);
//             }
            
//         }

//     }
// }

int VEB::Successor(int x) {
    // Base case: universe size is 2
    if (this->u == 2) {
        if (x == 0 && this->max == 1)
            return 1;
        else
            return -1;  // No successor
    }

    // If x is greater than or equal to the maximum element, return -1 (no successor)
    if (x >= this->max)
        return -1;

    // If x is less than the minimum, the min is the successor
    if (x < this->min)
        return this->min;

    // Get high and low parts of x
    int highX = high(x);
    int lowX = low(x);

    // Search for a successor within the same cluster
    if (cluster[highX] != nullptr && lowX < cluster[highX]->max) {
        int lowSuccessor = cluster[highX]->Successor(lowX);
        return index(highX, lowSuccessor);  // Combine high and low
    } else {
        // Find the smallest element in the summary larger than highX
        int highSuccessor = summary->Successor(highX);
        if (highSuccessor == -1) {
            return -1;  // No successor in the summary
        } else {
            int minLow = cluster[highSuccessor]->min;
            return index(highSuccessor, minLow);  // Combine high and low parts
        }
    }
}


// int VEB::Predecessor(int x) {
//     if (u == 2) {
//         if (x == 1 && min == 0) return 0;
//         else return -1;
//     } 
//     else if (max != -1 && x > max) return max;
//     else {
//         int minLow = cluster[high(x)]->min;

//         if (minLow != -1 && low(x) > minLow) {
//             int offset = cluster[high(x)]->Predecessor(low(x));
//             return index(high(x), offset);
//         } else {
//             int predCluster = summary->Predecessor(high(x));
//             if (predCluster == -1) {
//                 if (min != -1 && x > min) return min;
//                 else return -1;
//             } else {
//                 int offset = cluster[predCluster]->max;
//                 return index(predCluster, offset);
//             }
//         }

//     }
// }

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