#include "../src/BitUtil.h"
#include <fstream>
using namespace std;

bool valid(int x, int y){
    return (x>0 && x<7 && y>0 && y<7);
}

int main(){
    ofstream f("bishop_rays.bitset");
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            bitboard r  = 0ULL;
            for(int x=1; valid(i+x, j+x); x++) r |= getbboard(i+x, j+x);
            for(int x=1; valid(i+x, j-x); x++) r |= getbboard(i+x, j-x);
            for(int x=1; valid(i-x, j+x); x++) r |= getbboard(i-x, j+x);
            for(int x=1; valid(i-x, j-x); x++) r |= getbboard(i-x, j-x);
            f << r << '\n';
        }
    }
    f.close();
    return 0;
}