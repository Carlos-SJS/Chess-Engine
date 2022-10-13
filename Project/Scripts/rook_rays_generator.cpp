#include "../src/BitUtil.h"
#include <fstream>
using namespace std;

int main(){
    ofstream f("rook_rays.bitset");
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            bitboard r  = 0ULL;
            for(int x=1; x<i; x++) r |= getbboard(x, j);
            for(int x=i+1; x<7; x++) r |= getbboard(x, j);
            for(int x=1; x<j; x++) r |= getbboard(i, x);
            for(int x=j+1; x<7; x++) r |= getbboard(i, x);
            f << r << '\n';
        }
    }
    f.close();
}