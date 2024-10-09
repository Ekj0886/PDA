#include <bits/stdc++.h>
#include"VEB.h"

#define F first 
#define S second

using namespace std;

int main(int argc, char *argv[]) {

    // ifstream infile (argv[1]);
    // ofstream outfile(argv[2]);
    
    VEB v(16);
    v.Insert(0);
    v.Insert(2);
    v.Insert(5);
    cout << v.min << " " << v.max << endl;
    cout << v.summary->min << " " << v.summary->max << endl;

    cout << v.Successor(0) << endl;
    cout << v.Successor(2) << endl;
    cout << v.Predecessor(2) << endl;
    v.Delete(0);
    cout << v.Predecessor(2) << endl;
    
    return 0;

}