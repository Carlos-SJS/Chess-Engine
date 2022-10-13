#include<fstream>
#include<iostream>
#include "../src/Magics.h"
#include "../src/BitUtil.h"
using namespace std;

void printbboard(bitboard b){
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

bool valid(int x, int y){
    return (x>=0 && x<8 && y>=0 && y<8);
}

bool nbd(int x, int y){
    return (x>0 && x<7 && y>0 && y<7);
}

int main(){
    vector<vector<bitboard>> bishop_moves(64, vector<bitboard>(512));

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            for(u64 b = 0ULL; b<=(1ULL<<(bishop_rellevant_bits[(i<<3)+j])+1); b++){
                u64 c = b;
                bitboard bk = 0ULL;
                bitboard fm = 0ULL;
                bool p  = 1;

                for(int x = 1; valid(i+x, j+x); x++){
                    if(p) fm |= getbboard(i+x,j+x);
                    if(nbd(i+x, j+x) && (c&1)) bk |= getbboard(i+x, j+x), p = 0;
                    if(nbd(i+x, j+x)) c>>=1;
                }
                p = 1;
                for(int x = 1; valid(i+x, j-x); x++){
                    if(p) fm |= getbboard(i+x, j-x);
                    if(nbd(i+x, j-x) && (c&1)) bk |= getbboard(i+x, j-x), p = 0;
                    if(nbd(i+x, j-x)) c>>=1;
                }
                p = 1;
                for(int x = 1; valid(i-x, j+x); x++){
                    if(p) fm |= getbboard(i-x, j+x);
                    if(nbd(i-x, j+x) && (c&1)) bk |= getbboard(i-x, j+x), p = 0;
                    if(nbd(i-x, j+x)) c>>=1;
                }
                p = 1;
                for(int x = 1; valid(i-x, j-x); x++){
                    if(p) fm |= getbboard(i-x, j-x);
                    if(nbd(i-x, j-x) && (c&1)) bk |= getbboard(i-x, j-x), p = 0;
                    if(nbd(i-x, j-x)) c>>=1;
                }
                u64 key = (bk * bishopMagics[(i<<3)+j]) >> (64 - bishop_rellevant_bits[(i<<3)+j]);
                bishop_moves[(i<<3)+j][key] = fm;

                //printbboard(bk);
                //cout << "\n";
                //printbboard(fm);
                //cout << "\n\n";
            }
        }
    }

    ofstream f("bishop_moves.bitboard");
    for(int i=0; i<64; i++)
        for(int j=0; j<512; j++)
            f << bishop_moves[i][j] << '\n';
    f.close();
    cout << "File created\n";
}
