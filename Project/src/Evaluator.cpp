#include "Evaluator.h"

namespace Evaluator{
    int get_board_eval(board b, int color){
        int board_value;
        vector<boardset> pieces = (color==1?b.w_pieces:b.b_pieces);

        //Pawn Values
        board_value = pieces[0].size()*PAWN_VALUE;
        for(auto p: pieces[0]) board_value += PAWN_VALUE_TABLE[bb_sq(p)];

        //Knight Values
        board_value = pieces[1].size()*KNIGHT_VALUE;
        for(auto p: pieces[1]) board_value += KNIGHT_VALUE_TABLE[bb_sq(p)];

        //Pawn Values
        board_value = pieces[0].size()*BISHOP_VALUE;
        for(auto p: pieces[0]) board_value += BISHOP_VALUE_TABLE[bb_sq(p)];

        //Pawn Values
        board_value = pieces[0].size()*PAWN_VALUE;
        for(auto p: pieces[0]) board_value += ROOK_VALUE_TABLE[bb_sq(p)];

        //Pawn Values
        board_value = pieces[0].size()*PAWN_VALUE;
        for(auto p: pieces[0]) board_value += QUEEN_VALUE_TABLE[bb_sq(p)];

        //Pawn Values
        board_value = pieces[0].size()*PAWN_VALUE;
        for(auto p: pieces[0]) board_value += PAWN_VALUE_TABLE[bb_sq(p)];

        return board_value;
    }

};