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
#define bb_sq(a) (__builtin_ctzll(a))
#define bb_isq(a) (63-bb_sq(a))
#define sq_cd(a) (make_pair((a>>3), a%8))

#define getbboard(y, x) (1ULL << ((y<<3)+x))

#define from first
#define to second

#define cY first
#define cX second

typedef unsigned long long u64;
typedef u64 bitboard;
typedef vector<bitboard> boardset;
typedef pair<pair<int, int>, bitboard> move_pair;

struct moveset{
    int color;
    vector<pair<pair<int, int>, bitboard>> moves;
    vector<pair<pair<int, int>, bitboard>> captures;

    bitboard attacks;
};

struct board{
    bitboard white;
    bitboard black;

    bool left_white_castle;
    bool right_white_castle;

    bool left_black_castle;
    bool right_black_castle;

    vector<boardset> w_pieces;
    vector<boardset> b_pieces;
    board(){
        w_pieces.resize(6);
        b_pieces.resize(6);
    }
};


#endif
