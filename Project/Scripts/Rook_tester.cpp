#include "../src/Magics.h"
#include "../src/BitUtil.h"
#include <fstream>
#include <iostream>

using namespace std;

void printbboard(bitboard b){
    cout << b << '\n';
    string board[8];
    for(int i=0; i<8; i++){
        board[i] = "";
        for(int j=0; j<8; j++){
            board[i] += (b&1ULL)?"1":".";
            b=(b>>1);
        }
    }
    for(int i=7; i>=0; i--) cout << board[i] << "\n";
}

int main(){
    bitboard rook_rays[64];
    vector<vector<bitboard>> rook_moves(64, vector<bitboard>(4096));
    ifstream f("rook_moves.bitboard");
    for(int i=0; i<64; i++)
        for(int j=0; j<4096; j++) f >> rook_moves[i][j];
    f.close();

    ifstream rr("rook_rays.bitset");
    for(int i=0; i<64; i++) rr >> rook_rays[i];
    rr.close();

    /*int blockers[]{
        1,0,1,0,1,1,1,0,
        1,1,1,0,1,1,1,0,
        1,0,1,1,1,1,1,0,
        0,0,1,1,1,0,1,0,
        0,0,1,0,1,1,0,0,
        1,0,0,0,1,1,0,0,
        1,1,1,0,1,1,1,0,
        0,0,1,0,1,1,1,1
    };*/

    int blockers[]{
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1
    };

    bitboard bk = 0ULL;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(blockers[((i)<<3)+j])bk|=getbboard(7-i,j);
        }
    }

    cout << "Blockers:\n";
    printbboard(bk);

    int a,b;
    while(true){
        cin >> a >> b;
        bitboard rook = getbboard(b,a);
        bitboard moves = rook_moves[(b<<3)+a][rookMagics[(b<<3)+a]*(bk&rook_rays[(b<<3)+a])>>(64-rook_rellevant_bits[(b<<3)+a])];
        printbboard(rook_rays[(b<<3)+a]);
        cout << "\n";
        printbboard(bk&rook_rays[(b<<3)+a]);
        cout << "\n";
        printbboard(moves);
        cout << "\n";
    }

}