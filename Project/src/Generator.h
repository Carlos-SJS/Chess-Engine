#include "BitUtil.h"
#include <unordered_map>
#include <fstream>
#include <vector>
using namespace std;

namespace Generator{
    void load_data();
    moveset get_moves_w(board);
    moveset get_moves_b(board);
    moveset get_moves(vector<boardset>, bitboard, bitboard);

    bitboard get_knight_moves(bitboard, bitboard);
    bitboard get_knight_captures(bitboard, bitboard);
    bitboard get_king_moves(bitboard, bitboard);
    bitboard get_king_captures(bitboard, bitboard);
    bitboard get_bishop_moves(bitboard, bitboard);
    bitboard get_bishop_captures(bitboard, bitboard, bitboard);
    bitboard get_rook_moves(bitboard, bitboard);
    bitboard get_rook_captures(bitboard, bitboard, bitboard);
    bitboard get_queen_moves(bitboard, bitboard);
    bitboard get_queen_captures(bitboard, bitboard, bitboard);

    board array_to_board(int cboard[8][8], int color[8][8]);

    board sim_board(board, move_pair, int);

    const string knight_file = "data/knight_moves.bitboard";
    const string king_file = "data/king_moves.bitboard";
    const string white_pawn_file = "data/white_pawn.bitboard";
    const string black_pawn_file = "data/black_pawn.bitboard";
    const string white_pawnc_file = "data/white_pawn_captures.bitboard";
    const string black_pawnc_file = "data/black_pawn_captures.bitboard";

    const string rook_ray_file = "data/rook_rays.bitboard";
    const string rook_move_file = "data/rook_moves.bitboard";
    const string bishop_ray_file = "data/bishop_rays.bitboard";
    const string bishop_move_file = "data/bishop_moves.bitboard";
};