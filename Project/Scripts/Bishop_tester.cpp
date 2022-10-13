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
    bitboard bishop_rays[64];
    vector<vector<bitboard>> bishop_moves(64, vector<bitboard>(512));
    ifstream f("bishop_moves.bitboard");
    for(int i=0; i<64; i++)
        for(int j=0; j<512; j++) f >> bishop_moves[i][j];
    f.close();

    ifstream rr("bishop_rays.bitset");
    for(int i=0; i<64; i++) rr >> bishop_rays[i];
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
        0,0,1,0,1,1,1,0,
        0,1,1,0,1,1,1,0,
        0,0,1,1,1,1,1,0,
        0,0,1,1,1,0,1,0,
        0,0,1,0,1,1,0,0,
        0,0,0,0,1,1,0,0,
        0,0,1,0,1,1,1,0,
        0,0,0,0,0,0,0,0
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
        bitboard moves = bishop_moves[(b<<3)+a][bishopMagics[(b<<3)+a]*(bk&bishop_rays[(b<<3)+a])>>(64-bishop_rellevant_bits[(b<<3)+a])];
        printbboard(bishop_rays[(b<<3)+a]);
        cout << "\n";
        printbboard(bk&bishop_rays[(b<<3)+a]);
        cout << "\n";
        printbboard(moves);
        cout << "\n";
    }

}