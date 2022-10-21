#include "Evaluator.h"
#include "Generator.h"

namespace Evaluator{
    int get_board_eval(board b, int color){
        int board_value = 0;

        if(color == 1){
            vector<boardset> pieces = b.w_pieces;
            //Pawn Values
            for(auto p: pieces[0]){
                if(p&b.white == 0) continue;
                board_value += PAWN_VALUE_TABLE[bb_isq(p)];
                board_value += PAWN_VALUE;
            }

            //Knight/Rook Values
            for(auto p: pieces[1]){
                if(p&b.white == 0) continue;
                board_value += KNIGHT_VALUE_TABLE[bb_isq(p)];
                board_value += KNIGHT_VALUE;
            }

            //Bishop Values
            for(auto p: pieces[2]){
                if(p&b.white == 0) continue;
                board_value += BISHOP_VALUE_TABLE[bb_isq(p)];
                board_value += BISHOP_VALUE;
            }

            // Values
            board_value += pieces[3].size()*ROOK_VALUE;
            for(auto p: pieces[3]){
                if(p&b.white == 0) continue;
                board_value += ROOK_VALUE_TABLE[bb_isq(p)];

            }
            //Queen Values
            for(auto p: pieces[4]){
                if(p&b.white == 0) continue;
                board_value += QUEEN_VALUE_TABLE[bb_isq(p)];
                board_value += QUEEN_VALUE;
            }

            //Pawn Values
            if(pieces[5].size() && (pieces[5][0]&b.white) != 0){
                board_value += KING_VALUE;
                if(pieces[4].size() || (pieces[2].size()+pieces[3].size()+pieces[1].size())) board_value += KING_VALUE_TABLE[bb_isq(pieces[5][0])];
                else board_value += KING_VALUE_TABLE_END[bb_isq(pieces[5][0])];
            }
        }else{
            vector<boardset> pieces = b.b_pieces;
            //Pawn Values
            for(auto p: pieces[0]){
                if(p&b.black == 0) continue;
                board_value += PAWN_VALUE_TABLE[bb_sq(p)];
                board_value = PAWN_VALUE;
            }
            //Knight Values
            for(auto p: pieces[1]){
                if(p&b.black == 0) continue;
                board_value += KNIGHT_VALUE_TABLE[bb_sq(p)];
                board_value += KNIGHT_VALUE;
            }
            //Bishop Values
            for(auto p: pieces[2]){
                if(p&b.black == 0) continue;
                board_value += BISHOP_VALUE_TABLE[bb_sq(p)];
                board_value += BISHOP_VALUE;
            }
            //Rook Values
            for(auto p: pieces[3]){
                if(p&b.black == 0) continue;
                board_value += ROOK_VALUE_TABLE[bb_sq(p)];
                board_value += ROOK_VALUE;
            }

            //Queen Values
            for(auto p: pieces[4]){
                if(p&b.black == 0) continue;
                board_value += QUEEN_VALUE_TABLE[bb_sq(p)];
                board_value += QUEEN_VALUE;
            }

            //Pawn Values
            if(pieces[5].size() && (pieces[5][0]&b.black) != 0){
                board_value += pieces[5].size()*KING_VALUE;
                if(pieces[4].size() || (pieces[2].size()+pieces[3].size()+pieces[1].size())) board_value += KING_VALUE_TABLE[bb_sq(pieces[5][0])];
                else board_value += KING_VALUE_TABLE_END[bb_sq(pieces[5][0])];
            }
        }

        return board_value;
    }

};
