#include<vector>
#include<fstream>
#include"../src/BitUtil.h"
using namespace std;

bool valid(int y, int x){
    return y>=0 && x>=0 && y<8 && x<8;
}

void write(bitboard c, vector<bitboard> &m, ofstream &f){
    f << m.size() << " " << c;
    for(auto mv: m) f << " " << mv;
    f << '\n';
}

int main(){
    ofstream file("knight_moves.bitboard");
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            bitboard moves = 0LL;
            if(valid(i+2, j-1)) moves |= getbboard(i+2, j-1);
            if(valid(i+2, j+1)) moves |= getbboard(i+2, j+1);
            if(valid(i-2, j-1)) moves |= getbboard(i-2, j-1);
            if(valid(i-2, j+1)) moves |= getbboard(i-2, j+1);

            if(valid(i-1, j+2)) moves |= getbboard(i-1, j+2);
            if(valid(i+1, j+2)) moves |= getbboard(i+1, j+2);
            if(valid(i-1, j-2)) moves |= getbboard(i-1, j-2);
            if(valid(i+1, j-2)) moves |= getbboard(i+1, j-2);

            if(moves != 0LL) file << (i<<3)+j << " " << moves << '\n';    
        }
    }

    file.close();
}