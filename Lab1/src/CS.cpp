#include <bits/stdc++.h>
#include "CS.h"

#define F first 
#define S second

using namespace std;

// functions defined in header file

void CS::Dump_Layout(string file_name) {
    
    ofstream outfile("layout/" + file_name, ios::app);
    ofstream outdraw;

    outdraw.open("draw", ios::app);

    int tile_num = Block.size() + Space.size();
    outfile << tile_num << '\n' << w << " " << h << '\n';

    for(auto& block : Block) {
        TILE* tile = block.S;
        outfile << tile->ID << " " << tile->pt.x << " " << tile->pt.y << " ";
        outfile << tile->WIDTH() << " " << tile->HEIGHT() << '\n';
    }

    for(auto& space : Space) {
        TILE* tile = space.S;
        outfile << tile->ID << " " << tile->pt.x << " " << tile->pt.y << " ";
        outfile << tile->WIDTH() << " " << tile->HEIGHT() << '\n';
    }

    outdraw << "python3 draw_block_layout.py layout/" + file_name + " drawing/" + file_name + ".png" << endl;

}

void CS::Init_Outline() {
    
    space_index = -1;
    Point_vec.resize(0);

    TILE *outline_tile = new TILE;

    // set up pseudo tiles
    R_TILE = new TILE;
    T_TILE = new TILE;
    B_TILE = new TILE;
    L_TILE = new TILE;
 
    R_TILE->ID = 0;
    R_TILE->tr = nullptr;
    R_TILE->rt = nullptr;
    R_TILE->lb = nullptr;
    R_TILE->bl = outline_tile;
    R_TILE->pt = POINT(w, 0);

    T_TILE->ID = 0;
    T_TILE->tr = nullptr;
    T_TILE->rt = nullptr;
    T_TILE->lb = outline_tile;
    T_TILE->bl = nullptr;
    T_TILE->pt = POINT(0, h);

    B_TILE->ID = 0;
    B_TILE->tr = nullptr;
    B_TILE->rt = outline_tile;
    B_TILE->lb = nullptr;
    B_TILE->bl = nullptr;
    B_TILE->pt = POINT(0, -1);

    L_TILE->ID = 0;
    L_TILE->tr = outline_tile;
    L_TILE->rt = nullptr;
    L_TILE->lb = nullptr;
    L_TILE->bl = nullptr;
    L_TILE->pt = POINT(-1, 0);

    // setup outline tile
    outline_tile->ID = space_index;
    outline_tile->tr = R_TILE;
    outline_tile->rt = T_TILE;
    outline_tile->lb = B_TILE;
    outline_tile->bl = L_TILE;
    outline_tile->pt = POINT(0, 0);

    space_index --;
    Space[outline_tile->ID] = outline_tile;
}

void CS::Init_Case0() {
    space_index = -1;
    TILE *B1, *B2, *B3, *B4, *B5;
    TILE *V1, *V2, *V3, *V4, *V5, *V6, *V7, *V8;
    B1 = new TILE(space_index--, POINT(35, 35));
    B2 = new TILE(2, POINT(35, 65));
    B3 = new TILE(3, POINT(65, 5));
    B4 = new TILE(4, POINT(5, 15));
    B5 = new TILE(5, POINT(15, 35));

    V1 = new TILE(space_index--, POINT(0, 0));
    V2 = new TILE(space_index--, POINT(0, 5));
    V3 = new TILE(space_index--, POINT(0, 15));
    V4 = new TILE(space_index--, POINT(0, 35));
    V5 = new TILE(space_index--, POINT(0, 95));
    V6 = new TILE(space_index--, POINT(35, 85));
    V7 = new TILE(space_index--, POINT(95, 65));
    V8 = new TILE(space_index--, POINT(85, 5));

    B1->Set_Neighbor(B3, B2, B4, B5);
    B2->Set_Neighbor(V7, V6, B1, B5);
    B3->Set_Neighbor(V8, B2, V1, V2);
    B4->Set_Neighbor(B3, B1, V2, V3);
    B5->Set_Neighbor(V6, V5, B4, V4);

    V1->Set_Neighbor( R_TILE, V8, B_TILE, L_TILE);
    V2->Set_Neighbor( B3, B4, V1, L_TILE);
    V3->Set_Neighbor( B4, V4, V2, L_TILE);
    V4->Set_Neighbor( B5, V5, V3, L_TILE);
    V5->Set_Neighbor( R_TILE, T_TILE, V4, L_TILE);
    V6->Set_Neighbor( R_TILE, V5, B2, B5);
    V7->Set_Neighbor( R_TILE, V6, V8, B2);
    V8->Set_Neighbor( R_TILE, V7, V1, B3);

    Block[1] = B1;
    Block[2] = B2;
    Block[3] = B3;
    Block[4] = B4;
    Block[5] = B5;

    Space[-1] = V1;
    Space[-2] = V2;
    Space[-3] = V3;
    Space[-4] = V4;
    Space[-5] = V5;
    Space[-6] = V6;
    Space[-7] = V7;
    Space[-8] = V8;
}

TILE* CS::Find_Point(int x, int y) {
    
    TILE* tile; // initial tile 
    if(Space.empty()) tile = Block.begin()->S;
    else              tile = Space.begin()->S;
    if(y == h) y--;
    if(x == w) x--;
    while(!tile->Inside(x, y)) {
        if(tile->ID == 0) break;
        while( y >= tile->TOP()   ) tile = tile->rt;
        while( y < tile->BOTTOM() ) tile = tile->lb;
        while( x >= tile->RIGHT() ) tile = tile->tr;
        while( x < tile->LEFT()   ) tile = tile->bl;
    }
    return tile;

}

void CS::SplitY(TILE* tile, int y) {
    
    // if y overlap with tile's edge, skip step
    if(y == tile->TOP() || y == tile->BOTTOM() || tile->ID > 0)  return;

    // Set new tile
    TILE* new_tile = new TILE(space_index--, POINT(tile->LEFT(), y));
    new_tile->Set_Neighbor(tile->tr, tile->rt, tile, nullptr);

    // fix top neighbor
    for(TILE* t = tile->rt; t->lb == tile; t=t->bl) {
        if(t->ID == 0) break;
        t->lb = new_tile;
    }

    // fix right neighbor
    for(TILE* t = tile->tr; t->bl == tile; t=t->lb) {
        if(t->ID == 0) break;
        if(t->BOTTOM() >= y) t->bl = new_tile;
    }

    
    // fix left neighbor
    for(TILE* t = tile->bl; t->tr == tile; t=t->rt) {
        if(t->ID == 0) break;
        if(t->TOP() > y) t->tr = new_tile;
    }

    // fix tile->tr
    TILE* t = tile->tr;
    while(t->ID != 0 && !(t->TOP() >= y && t->BOTTOM() < y)) t = t->lb;
    tile->tr = t;

    // fix new_tile->bl
    t = tile->bl;
    while(t->ID != 0 && !(t->TOP() > y && t->BOTTOM() <= y)) t = t->rt;
    new_tile->bl = t;

    tile->rt = new_tile;

    // insert split tile
    Space[new_tile->ID] = new_tile;

}

bool CS::SplitX(TILE* tile, int x) {

    // if x overlap with tile's edge, skip step
    if(x == tile->LEFT() || x == tile->RIGHT() || tile->ID > 0) return false;

    TILE* new_tile = new TILE(space_index--, POINT(x, tile->BOTTOM()));
    new_tile->Set_Neighbor(tile->tr, tile->rt, nullptr, tile);

    // fix top neighbor
    for(TILE* t = tile->rt; t->lb == tile; t=t->bl) {
        if(t->ID == 0) break;
        if(t->LEFT() >= x) t->lb = new_tile;
    }

    // fix right neighbor
    for(TILE* t = tile->tr; t->bl == tile; t=t->lb) {
        if(t->ID == 0) break;
        t->bl = new_tile;
    }
  
    // fix bottom neighbor
    for(TILE* t = tile->lb; t->rt == tile; t=t->tr) {
        if(t->ID == 0) break;
        if(t->RIGHT() > x) t->rt = new_tile;
    }

    // fix tile->rt
    TILE* t = tile->rt;
    while(t->ID != 0 && !(t->RIGHT() >= x && t->LEFT() < x)) t = t->bl;
    tile->rt = t;

    // fix new_tile->lb
    t = tile->lb;
    while(t->ID != 0 && !(t->RIGHT() > x && t->LEFT() <= x)) t = t->tr;
    new_tile->lb = t;

    tile->tr = new_tile;

    // insert split tile
    Space[new_tile->ID] = new_tile;
    return true;

}

void CS::Merge(TILE* U_tile, TILE* D_tile) {

    if(U_tile == nullptr || D_tile == nullptr) return;

    // check two tiles mergable?
    if(U_tile->ID == 0 || D_tile->ID == 0) return;
    bool diff_span = ( U_tile->LEFT() != D_tile->LEFT() || U_tile->RIGHT() != D_tile->RIGHT() );
    bool not_neighbor = ( U_tile->lb != D_tile || D_tile->rt != U_tile );
    bool not_same_type = !(U_tile->ID < 0 && D_tile->ID < 0);


    if(diff_span || not_neighbor || not_same_type) return;

    // create new tile
    int new_ID = max(U_tile->ID, D_tile->ID);

    TILE* new_tile = new TILE(new_ID, D_tile->pt);

    new_tile->Set_Neighbor(U_tile->tr, U_tile->rt, D_tile->lb, D_tile->bl);

    // fix top neighbor
    for(TILE* t = U_tile->rt; t->lb == U_tile; t=t->bl) {
        if(t->ID == 0) break;
        t->lb = new_tile;
    }

    // fix right neighbor
    for(TILE* t = U_tile->tr; t->bl == U_tile || t->bl == D_tile; t=t->lb) {
        if(t->ID == 0) break;
        t->bl = new_tile;
    }

    // fix left neighbor
    for(TILE* t = D_tile->bl; t->tr == U_tile || t->tr == D_tile; t=t->rt) {
        if(t->ID == 0) break;
        t->tr = new_tile;
    }

    // fix bottom neighbor
    for(TILE* t = D_tile->lb; t->rt == D_tile; t=t->tr) {
        if(t->ID == 0) break;
        t->rt = new_tile;
    }

    Space.erase(U_tile->ID);
    Space.erase(D_tile->ID);
    delete U_tile;
    delete D_tile;
    Space[new_ID] = new_tile;
}

TILE* CS::MergeBLK(TILE* U_tile, TILE* D_tile, int y) {

    // check two tiles mergable?
    bool is_block = (U_tile->ID >= 0 || D_tile->ID >= 0);
    if(is_block) return U_tile;

    bool out_bound = D_tile->TOP() <= y;
    bool diff_span = ( (U_tile->LEFT() != D_tile->LEFT()) || (U_tile->RIGHT() != D_tile->RIGHT()) );
    bool not_neighbor = ( U_tile->lb != D_tile || D_tile->rt != U_tile );

    if(diff_span || not_neighbor || out_bound) return U_tile;

    // create new tile
    int new_ID = max(U_tile->ID, D_tile->ID);

    TILE* new_tile = new TILE(new_ID, D_tile->pt);

    new_tile->Set_Neighbor(U_tile->tr, U_tile->rt, D_tile->lb, D_tile->bl);

    // fix top neighbor
    for(TILE* t = U_tile->rt; t->lb == U_tile; t=t->bl) {
        if(t->ID == 0) break;
        t->lb = new_tile;
    }

    // fix right neighbor
    for(TILE* t = U_tile->tr; t->bl == U_tile || t->bl == D_tile; t=t->lb) {
        if(t->ID == 0) break;
        t->bl = new_tile;
    }

    // fix left neighbor
    for(TILE* t = D_tile->bl; t->tr == U_tile || t->tr == D_tile; t=t->rt) {
        if(t->ID == 0) break;
        t->tr = new_tile;
    }

    // fix bottom neighbor
    for(TILE* t = D_tile->lb; t->rt == D_tile; t=t->tr) {
        if(t->ID == 0) break;
        t->rt = new_tile;
    }
 
    Space.erase(U_tile->ID);
    Space.erase(D_tile->ID);
    delete U_tile;
    delete D_tile;
    Space[new_ID] = new_tile;

    return new_tile;

}

void CS::Create_Blk(int id, int x, int y, int W_blk, int H_blk) {

    ofstream outdraw("draw");

    // split by top / down edge
    SplitY(Find_Point(x, y + H_blk), y + H_blk);
    SplitY(Find_Point(x, y), y);
    
  
    // trace covered tile
    int xt = x, yt = y;
    TILE* tile = Find_Point(xt, yt);

    while(tile->BOTTOM() < y + H_blk) {

        bool right = SplitX(tile, x + W_blk);
        bool left  = SplitX(tile, x);

        tile = Find_Point(x, yt);

        if(right) { // split right
            Merge(tile->tr, tile->tr->lb);
            Merge(tile->tr->rt, tile->tr);
        }

        if(left) { // split left
            Merge(tile->bl, tile->bl->lb);
            Merge(tile->bl->rt, tile->bl);
        }

        // self merge
        tile = MergeBLK(tile, tile->lb, y);

        if(tile->rt->ID >= 0) break;
        yt = tile->pt.y + tile->HEIGHT();
        tile = Find_Point(x, yt);

    }

    // create new block
    TILE* new_blk = Find_Point(x, y);
    Space.erase(new_blk->ID);
    new_blk->ID = id;
    Block[id] = new_blk;

    // PrintMap();
    // Dump_Layout("layout");

}

void CS::Dump_Output(string file) {
    
    ofstream outfile(file);
    int tile_num = Block.size() + Space.size();
    outfile << tile_num << endl;

    for(auto& block : Block) {
        
        TILE* tile = block.S;
        int block_num = 0, space_num = 0;
        
        // top
        for(TILE* t = tile->rt; t->ID != 0 && t->RIGHT() > tile->LEFT(); t=t->bl) {
            if(t->ID > 0) block_num++;
            if(t->ID < 0) space_num++;
        }

        // bottom
        for(TILE* t = tile->lb; t->ID != 0 && t->LEFT() < tile->RIGHT(); t=t->tr) {
            if(t->ID > 0) block_num++;
            if(t->ID < 0) space_num++;
        }

        // right
        for(TILE* t = tile->tr; t->ID != 0 && t->TOP() > tile->BOTTOM(); t=t->lb) {
            if(t->ID > 0) block_num++;
            if(t->ID < 0) space_num++;
        }

        // left
        for(TILE* t = tile->bl; t->ID != 0 && t->BOTTOM() < tile->TOP(); t=t->rt) {
            if(t->ID > 0) block_num++;
            if(t->ID < 0) space_num++;
        }

        outfile << tile->ID << " " << block_num << " " << space_num << endl;

    }

    for(auto p : Point_vec) outfile << p.x << " " << p.y << endl;

}

void CS::PrintMap() {

    // ofstream outfile("Maplog.txt", ios::app);
    ofstream outfile("Maplog.txt");

    outfile << endl << "Block Map" << endl; 
    for(auto& block : Block) {
        TILE* t = block.S;
        outfile << "   Blk" << t->ID << endl;
        outfile << "       tr: " << t->tr->ID << endl;
        outfile << "       rt: " << t->rt->ID << endl;
        outfile << "       lb: " << t->lb->ID << endl;
        outfile << "       bl: " << t->bl->ID << endl;
        outfile << endl;
    }

    outfile << "Space Map" << endl; 
    for(auto& block : Space) {
        TILE* t = block.S;
        outfile << "   Spc" << t->ID << endl;
        outfile << "       tr: " << t->tr->ID << endl;
        outfile << "       rt: " << t->rt->ID << endl;
        outfile << "       lb: " << t->lb->ID << endl;
        outfile << "       bl: " << t->bl->ID << endl;
        outfile << endl;
    }
}