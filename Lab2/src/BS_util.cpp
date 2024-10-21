#include <bits/stdc++.h>
#include "BS.h"

#define F first 
#define S second

using namespace std;

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

    NodeList.resize(0);
    Node* Root = new Node(BlockList.begin()->S);
    NodeList.push_back(Root);
    if(Blk_num < 2) return Root;

    queue<Node*> q;
    q.push(Root);

    auto it = BlockList.begin();
    it++;
    while(it != BlockList.end()) { 
        
        Node* n = q.front();
        q.pop();

        if(it != BlockList.end()) {
            n->Left = new Node(it->S);
            // cout << n->Left->blk->name << endl;
            n->Left->Parent = n;
            q.push(n->Left);
            it++;
            NodeList.push_back(n->Left);
        }
        
        if(it != BlockList.end()) {
            n->Right = new Node(it->S);
            // cout << n->Right->blk->name << endl;
            n->Right->Parent = n;
            q.push(n->Right);
            it++;
            NodeList.push_back(n->Right);
        }

    }

    return Root;

}

// Function to calculate the height of the tree
int getHeight(Node* root) {
    if (!root) return 0;
    return 1 + std::max(getHeight(root->Left), getHeight(root->Right));
}

void PrintSubTree(Node* root, int indent) {
    if (root != nullptr) {
        PrintSubTree(root->Right, indent + 4); // Print the right subtree
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        std::cout << root->Name() << "\n"; // Print root node
        PrintSubTree(root->Left, indent + 4); // Print the left subtree
    }
}

void BSTAR::PrintTree() {
    PrintSubTree(root, 0);
}


void BSTAR::GetCoordinate() {
    
    W_fp = 0;
    H_fp = 0;

    stack<Node*> unvisit;
    // cout << "Root: " << root->Name() << endl;
    root->SetX(0); 
    root->SetY(0);
    Contour.resize(0);
    unvisit.push(root);

    while(!unvisit.empty()) {

        Node* node = unvisit.top();
        unvisit.pop();
        // cout << node->Name() << " " << node->X() << " " << node->X() + node->W() << endl;

        int node_y = UpdateContour(node->X(), node->X() + node->W(), node->H());
        node->SetY( node_y );

        if(node->Right) {
            unvisit.push(node->Right);
            node->Right->SetX( node->X() );
            // cout << "Push right: " << node->Right->Name() << endl;
        }
        if(node->Left) {
            unvisit.push(node->Left);
            node->Left->SetX( node->X() + node->W() );
            // cout << "Push left: " << node->Left->Name() << endl;
        }

        if(node->X() + node->W() > W_fp) W_fp = node->X() + node->W();
        if(node->Y() + node->H() > H_fp) H_fp = node->Y() + node->H();

    }


}

int BSTAR::UpdateContour(int start, int end, int h) {

    // root node
    if(Contour.empty()) {
        Seg seg(0, root->W(), root->H());
        Contour.push_back(seg);
        return 0;
    }

    // else node
    int start_seg = -1, end_seg = -1; 

    for(unsigned int i = 0; i < Contour.size(); i++) {
        Seg s = Contour[i];
        if(start >= s.start && start < s.end) start_seg = i;
        if(end > s.start && end <= s.end)     end_seg   = i;
    }
    // cout << "start_seg: " << start_seg << " | end_seg: " << end_seg << endl;
    if(start_seg == -1) { // blk at right-most position
        Seg seg(start, end, h);
        Contour.push_back(seg);
        return 0;
    }
    else if(end_seg == -1) {
        int y = 0;
        for(unsigned int i = start_seg; i < Contour.size(); i++) {
            if(Contour[i].y > y) y = Contour[i].y;
        }
        Seg seg(start, end, y+h);
        Contour.resize(start_seg);
        Contour.push_back(seg);
        return y;
    }
    else {
        int y = 0, contour_end = Contour[end_seg].end, contour_end_y = Contour[end_seg].y;
        end_seg ++; // for erase iterator align purpose
        for(int i = start_seg; i < end_seg; i++) {
            if(Contour[i].y > y) y = Contour[i].y;
        }
        Contour.erase(Contour.begin() + start_seg, Contour.begin() + end_seg);
        Seg seg(start, end, y+h);
        if(!(end == contour_end)) {
            Seg seg_fix(end, contour_end, contour_end_y);
            Contour.insert(Contour.begin() + start_seg, seg_fix);
        }
        Contour.insert(Contour.begin() + start_seg, seg);
        return y;
    }

}

void BSTAR::PrintContour() {

    for(unsigned int i = 0; i < Contour.size(); i++) {
        cout << Contour[i].start << " " << Contour[i].end << " " << Contour[i].y << endl;
    }
    cout << endl;

}

Node* BSTAR::GetRandNode() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, Blk_num-1);

    return NodeList[distr(gen)];
}

Node* BSTAR::GetLeaf() {
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, 1);


    Node* n = root;
    while(n->Right || n->Left) {
        if(n->Right && n->Left) {
            int r = distr(gen);
            if(r) n = n->Right;
            else   n = n->Left;
        }
        if(n->Right) n = n->Right;
        if(n->Left)  n = n->Left;
    }
    return n;
} 

void BSTAR::RotateBlk() {
    BLK* blk = GetRandNode()->blk;
    rFlag = blk->name;
    blk->Rotate();
}

void BSTAR::SwapNode() {

    Node* n1 = GetRandNode();
    Node* n2 = GetRandNode();
    while(n1 == n2) n2 = GetRandNode();

    BLK* temp = n1->blk;
    n1->blk = n2->blk;
    n2->blk = temp;

}

void BSTAR::MoveNode() {

    Node* n = GetRandNode();
    while(n->Left && n->Right) n = GetRandNode(); // get node with one/zero child

    // Update n's parent and child pointer
    if(n->Left) {
        if(n == root) root = n->Left;
        if(!n->Parent) {
            n->Left->Parent = nullptr;
            root = n->Left;
        }   
        else if(n->Parent->Left == n) {
            n->Parent->Left = n->Left;
            n->Left->Parent = n->Parent;
        }
        else {
            n->Parent->Right = n->Left;
            n->Left->Parent = n->Parent;
        }
    }
    else if(n->Right) {
        if(n == root) root = n->Right;
        if(!n->Parent) {
            n->Right->Parent = nullptr;
            root = n->Right;
        } 
        else if(n->Parent->Left == n) {
            n->Parent->Left = n->Right;
            n->Right->Parent = n->Parent;
        }
        else {
            n->Parent->Right = n->Right;
            n->Right->Parent = n->Parent;
        }
    }
    else {
        if(n->Parent->Left == n) n->Parent->Left = nullptr;
        else                     n->Parent->Right = nullptr;
    }

    // Insert node
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, 1);

    int r = distr(gen);
    
    Node* np = GetLeaf();
    while(np == n) np = GetLeaf();

    n->Left   = nullptr;
    n->Right  = nullptr;
    n->Parent = np;

    if(r) np->Right = n;
    else  np->Left = n;
 
}

bool BSTAR::Outside(BLK* blk) {
    if(blk->x + blk->w > W || blk->y + blk->h > H) return true;
    else return false;
}

float BSTAR::DeadSpace() {
    GetCoordinate(); 
    return (float) 5000 * (W_fp*H_fp - Blk_area)/(W_fp*H_fp); 
}

float BSTAR::OutArea() {
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

int BSTAR::Wire() {

    GetCoordinate();
    float wire_length = 0;
    for(auto n : NetList) {
        wire_length += n.HPWL();
    }
    return wire_length;
}


int BSTAR::Cost() {

    GetCoordinate();
    float wire_length = 0;
    for(auto n : NetList) {
        wire_length += n.HPWL();
    }

    int cost = floor( (float)(alpha*W_fp*H_fp  + (1.0 - alpha)*wire_length) ); 

    return cost; 

}