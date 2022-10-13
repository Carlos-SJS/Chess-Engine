#include<fstream>
#include "../src/BitUtil.h"

int main(){
    ofstream f("sq_to_bboard.bitboard");
    for(u64 i=0LL; i<8LL; i++){
        for(u64 j=0LL; j<8LL; j++){
            f << (i<<3)+j << " " << getbboard(i, j) << '\n';
        }
    }
}