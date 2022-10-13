/*
    Utilities, structs and datatypes for bitboard handling
*/

#ifndef bitutil_h
#define bitutil_h

#include <vector>
using namespace std;

#define setb(a, i) ((a)|=(1ULL << i))
#define getb(a, i) ((a)&(1ULL << i))
#define clearb(a, i) ((a)&=~(1ULL << i))
#define lsb(a) (__builtin_ctzll(a))
#define bb_sq(a) (__builtin_clz(a))
#define sq_cd(a) (make_pair((a>>3), a-(a>>3)))

#define getbboard(y, x) (1ULL << ((y<<3)+x))

#define from first
#define to second

#define cY first
#define cX second

typedef unsigned long long u64;
typedef u64 bitboard;
typedef vector<bitboard> boardset;

struct moveset{
    int color;
    vector<pair<bitboard, bitboard>> moves;
    vector<pair<bitboard, bitboard>> captures;
};

struct board{
    bitboard white;
    bitboard black;
    vector<boardset> w_pieces;
    vector<boardset> b_pieces;
    board(){
        w_pieces.resize(6);
        b_pieces.resize(6);
    }
};

#endif
