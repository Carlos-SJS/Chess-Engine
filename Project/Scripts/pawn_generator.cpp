#include "../src/BitUtil.h"
#include <fstream>

int main(){
    ofstream wp("white_pawn.bitset");
    for(int i=0; i<8; i++) wp << "0\n";
    for(int i=0; i<8; i++) wp << (getbboard(2, i)|getbboard(3, i)) << "\n";
    for(int i=2; i<7; i++)
        for(int j=0; j<8; j++) wp << (getbboard(i+1, j)) << "\n";
    for(int i=0; i<8; i++) wp << "0\n";   
    wp.close();

    ofstream bp("black_pawn.bitset");
    for(int i=0; i<8; i++) bp << "0\n";
    for(int i=1; i<6; i++)
        for(int j=0; j<8; j++) bp << (getbboard(i-1, j)) << "\n";
    for(int i=0; i<8; i++) bp << (getbboard(5, i)|getbboard(4, i)) << "\n";
    for(int i=0; i<8; i++) bp << "0\n";
    bp.close();
}