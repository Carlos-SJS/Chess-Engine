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
    ofstream file("king_moves.bitboard");
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            bitboard moves = 0LL;
            int move_count = 0;

            for(int k=-1; k<=1; k++){
                for(int l = -1; l<=1; l++) if((k!=0 || l!=0) && valid(i+k, j+l) ) moves |= getbboard(i+k, j+l), move_count++;
            }

            if(moves != 0LL) file << (i<<3)+j << " " << moves << '\n';    
        }
    }

    file.close();
}