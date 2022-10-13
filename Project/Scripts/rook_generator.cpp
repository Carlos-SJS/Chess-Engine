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

int main(){
    vector<vector<bitboard>> rook_moves(64, vector<bitboard>(4096));

    //unsigned long long rook_moves[64][4086];

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            for(u64 b = 0ULL; b<=(1ULL<<(rook_rellevant_bits[(i<<3)+j])+1); b++){
                u64 c = b;
                bitboard bk = 0ULL;
                bitboard fm = 0ULL;
                bool p  = 1;

                for(int x = i-1; x >= 0; x--){
                    if(p) fm |= getbboard(x,j);
                    if(x>0 && (c&1)) bk |= getbboard(x, j), p = 0;
                    if(x>0) c>>=1;
                }
                p = 1;
                for(int x = i+1; x<8; x++){
                    if(p) fm |= getbboard(x,j);
                    if(x<7 && (c&1)) bk |= getbboard(x, j), p = 0;
                    if(x<7) c>>=1;
                }
                p = 1;
                for(int x = j-1; x >= 0; x--){
                    if(p) fm |= getbboard(i,x);
                    if(x > 0 && (c&1)) bk |= getbboard(i, x), p = 0;
                    if(x > 0) c>>=1;
                }
                p = 1;
                for(int x = j+1; x<8; x++){
                    if(p) fm |= getbboard(i,x);
                    if(x<7 && (c&1)) bk |= getbboard(i, x), p = 0;
                    if(x<7) c>>=1;
                }
                u64 key = (bk * rookMagics[(i<<3)+j]) >> (64 - rook_rellevant_bits[(i<<3)+j]);
                rook_moves[(i<<3)+j][key] = fm;

                //printbboard(bk);
                //cout << "\n";
                //printbboard(fm);
                //cout << "\n\n";
            }
        }
    }

    ofstream f("rook_moves.bitboard");
    for(int i=0; i<64; i++)
        for(int j=0; j<4096; j++)
            f << rook_moves[i][j] << '\n';
    f.close();
    cout << "File created\n";
}
