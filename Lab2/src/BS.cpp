#include <bits/stdc++.h>
#include "BS.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

// Split the input line into inst segments
void BSTAR::GetLine(ifstream& infile, vector<string>& inst) { 
    inst.resize(0);
    string str;
    getline(infile, str);
    istringstream ss(str);
    string token;
    while (ss >> token) inst.push_back(token);
}

// Skip empty line in infile
void BSTAR::SkipEmpty(ifstream& infile, vector<string>& inst) {
    streampos pos;
    do {
        // std::cout << "empty" << endl;
        pos = infile.tellg();
        GetLine(infile, inst);
    } while(inst.size() == 0);
    infile.seekg(pos);
}

// functions defined in header file

void BSTAR::LoadUnit(string file) {

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

void BSTAR::LoadNet(string file) {

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

Node* BSTAR::BuildTree() {

    Node* Root = new Node(BlockList.begin()->S);
    if(Blk_num < 2) return Root;

    queue<Node*> q;
    q.push(Root);

    auto it = BlockList.begin();
    it++;
    while(it != BlockList.end() && !q.empty()) { 
        
        Node* n = q.front();
        q.pop();

        if(it != BlockList.end()) {
            n->Left = new Node(it->S);
            // cout << n->Left->blk->name << endl;
            n->Left->Parent = n;
            q.push(n->Left);
            it++;
        }
        
        if(it != BlockList.end()) {
            n->Right = new Node(it->S);
            // cout << n->Right->blk->name << endl;
            n->Right->Parent = n;
            q.push(n->Right);
            it++;
        }

    }


    return Root;

}

// Function to calculate the height of the tree
int getHeight(Node* root) {
    if (!root) return 0;
    return 1 + std::max(getHeight(root->Left), getHeight(root->Right));
}


void BSTAR::PrintTree(Node* root, int indent = 0) {
    if (root != nullptr) {
        PrintTree(root->Right, indent + 4); // Print the right subtree
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        std::cout << root->Name() << "\n"; // Print root node
        PrintTree(root->Left, indent + 4); // Print the left subtree
    }
}


void BSTAR::Init() {
    cout << "Build" << endl;
    root = BuildTree();
    cout << "Print" << endl;
    PrintTree(root, 0);
}

void BSTAR::GetCoordinate() {
    
    W_fp = 0;
    H_fp = 0;

    

}