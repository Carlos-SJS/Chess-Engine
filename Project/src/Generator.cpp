#include "Generator.h"
#include "logger.h"
#include "Magics.h"

namespace Generator{
    //Precalculated move tables
    bitboard knight_move_t[64];
    bitboard king_move_t[64];

    bitboard white_pawn_move_t[64];
    bitboard black_pawn_move_t[64];
    bitboard white_pawn_capture_t[64];
    bitboard black_pawn_capture_t[64];

    bitboard rook_rays[64];
    bitboard bishop_rays[64];

    vector<vector<bitboard>> rook_move_t(64, vector<bitboard>(4096));
    bitboard bishop_move_t[64][512]; 

    
    //Load precalculated data
    void load_data(){
        logger.log("Loading Move Generator data");
        try{
            int key;
            bitboard moves;

            //Loading pawn data
            logger.log("Loading white pawn move table");
            ifstream wpwn(white_pawn_file);
            if(wpwn.fail()){
                logger.error("Couldn't load Knigth data file \"" + white_pawn_file + "\"");
            }

            for(int i=0; i<64; i++)
                wpwn >> white_pawn_move_t[i];
                
            wpwn.close();

            //White pawn captures
            ifstream wpwnc(white_pawnc_file);
            if(wpwnc.fail()){
                logger.error("Couldn't load Pawn data file \"" + white_pawnc_file + "\"");
            }

            for(int i=0; i<64; i++)
                wpwnc >> white_pawn_capture_t[i];
                
            wpwn.close();

            logger.success("White pawn move table loaded");

            logger.log("Loading black pawn move table");
            ifstream bpwn(black_pawn_file);
            if(bpwn.fail()){
                logger.error("Couldn't load black pawn data file \"" + black_pawn_file + "\"");
            }

            for(int i=0; i<64; i++)
                bpwn >> black_pawn_move_t[i];

            bpwn.close();

            //Pawn captures
            ifstream bpwnc(black_pawnc_file);
            if(bpwnc.fail()){
                logger.error("Couldn't load black pawn data file \"" + black_pawnc_file + "\"");
            }

            for(int i=0; i<64; i++)
                bpwnc >> black_pawn_capture_t[i];

            bpwn.close();

            logger.success("Black pawn move table loaded");

            //Loading knight data
            logger.log("Loading knight move table");
            ifstream knightm(knight_file);
            if(knightm.fail()){
                logger.error("Couldn't load Knigth data file \"" + knight_file + "\"");
            }

            while(!knightm.eof()){
                knightm >> key >> moves;
                knight_move_t[key] = moves;
            }
            knightm.close();
            logger.success("Knight move table loaded");

            //Loading king data
            logger.log("Loading king move table");
            ifstream kingm(king_file);
            if(kingm.fail()){
                logger.error("Couldn't load King data file \"" + king_file + "\"");
            }

            while(!kingm.eof()){
                kingm >> key >> moves;
                king_move_t[key] = moves;
            }
            kingm.close();
            logger.success("King move table loaded");

            //Loading rook data
            logger.log("Loading rook ray table");
            ifstream rookr(rook_ray_file);
            if(rookr.fail()){
                logger.error("Couldn't load rook ray data file \"" + rook_ray_file + "\"");
            }

            for(int i=0; i<64; i++)rookr >> rook_rays[i];
            rookr.close();
            logger.success("Rook ray table loaded");

            logger.log("Loading rook move table");
            ifstream rookm(rook_move_file);
            if(rookm.fail()){
                logger.error("Couldn't load rook move data file \"" + rook_move_file + "\"");
            }

            for(int i=0; i<64; i++){
                for(int j=0; j<4096; j++){
                    rookm >> rook_move_t[i][j];
                }
            }
            
            rookm.close();
            logger.success("Rook move table loaded");

            //Loading bishop data
            logger.log("Loading bishop ray table");
            ifstream bhpr(bishop_ray_file);
            if(bhpr.fail()){
                logger.error("Couldn't load bishop ray data file \"" + bishop_ray_file + "\"");
            }

            for(int i=0; i<64; i++)bhpr >> bishop_rays[i];
            bhpr.close();
            logger.success("Bishop ray table loaded");

            logger.log("Loading bishop move table");
            ifstream bhpm(bishop_move_file);
            if(bhpm.fail()){
                logger.error("Couldn't load bishop move data file \"" + bishop_move_file + "\"");
            }

            for(int i=0; i<64; i++){
                for(int j=0; j<512; j++){
                    bhpm >> bishop_move_t[i][j];
                }
            }
            
            bhpm.close();
            logger.success("Bishop move table loaded");           

        }catch(exception e){
            logger.error("Error loading Move Generator files with exception: " + string(e.what()));
        }catch(...){
            logger.error("Error loading Move Generator files");
        }
        
    }


    //Get moves for white
    moveset get_moves_w(board b){
        // 0 -> pawn, 1 -> knight, 2 -> bishop, 3 -> rook, 4 -> queen, 5 -> king   
        moveset moves = get_moves(b.w_pieces, b.white, b.black);
        moves.color = 1;

        //White Pawns
        for(auto pawn: b.w_pieces[0]){
            if(white_pawn_move_t[bb_sq(pawn)])
                if((white_pawn_move_t[bb_sq(pawn)] & (b.black|b.white)) == 0)
                    moves.moves.push_back({pawn, white_pawn_move_t[bb_sq(pawn)]});

            if(white_pawn_capture_t[bb_sq(pawn)]&b.black)
                moves.captures.push_back({pawn, white_pawn_capture_t[bb_sq(pawn)] & b.black});
        }
        return moves;
    }

    //Get moves for black
    moveset get_moves_b(board b){
        // 0 -> pawn, 1 -> knight, 2 -> bishop, 3 -> rook, 4 -> queen, 5 -> king   
        moveset moves = get_moves(b.w_pieces, b.white, b.black);
        moves.color = 1;

        //Black Pawns
        for(auto pawn: b.b_pieces[0]){
            if(black_pawn_move_t[bb_sq(pawn)])
                if((black_pawn_move_t[bb_sq(pawn)] & (b.black|b.white)) == 0)
                    moves.moves.push_back({pawn, black_pawn_move_t[bb_sq(pawn)]});

            if(black_pawn_capture_t[bb_sq(pawn)]&b.white)
                moves.captures.push_back({pawn, black_pawn_capture_t[bb_sq(pawn)] & b.white});
        }

        return moves;
    }

    //Get general moves (Knight, Bishop, Rook, Queen, King)
    moveset get_moves(vector<boardset> pieces, bitboard own, bitboard other){
        moveset moves;
        bitboard m,c;

        //Knight
        for(auto p: pieces[1]){
            m = get_knight_moves(p, (own|other));
            c = get_knight_captures(p, other);
            if(m) moves.moves.push_back({p, m});
            if(c) moves.captures.push_back({p, c});
        }

        //Bishop
        for(auto p: pieces[2]){
            m = get_bishop_moves(p, (own|other));
            c = get_bishop_captures(p, other, own);
            if(m) moves.moves.push_back({p, m});
            if(c) moves.captures.push_back({p, c});
        }

        //Rook
        for(auto p: pieces[3]){
            m = get_rook_moves(p, (own|other));
            c = get_rook_captures(p, other, own);
            if(m) moves.moves.push_back({p, m});
            if(c) moves.captures.push_back({p, c});
        }

        //Queen
        for(auto p: pieces[4]){
            m = get_queen_moves(p, (own|other));
            c = get_queen_captures(p, other, own);
            if(m) moves.moves.push_back({p, m});
            if(c) moves.captures.push_back({p, c});
        }

        //King
        for(auto p: pieces[5]){
            m = get_king_moves(p, (own|other));
            c = get_king_captures(p, other);
            if(m) moves.moves.push_back({p, m});
            if(c) moves.captures.push_back({p, c});
        }

        return moves;
    }


    //Get moves for Knight (Precalculated)
    bitboard get_knight_moves(bitboard knight, bitboard other){
        bitboard moves = knight_move_t[bb_sq(knight)] & ~other;
        //Is needed to check if the moves are leagal (if the kniwght is not pinned to the king)
        //Could be avoided if we don't check if the move is legal and take a king captura as the best move in search 
        //(Could be bad for performance but probably not that much since we have to check it anyway)
        return moves;
    }

    //Get capture moves for Knight (Precalculated)
    bitboard get_knight_captures(bitboard knight, bitboard other){
        bitboard moves = knight_move_t[bb_sq(knight)] & other;
        //Is needed to check if the moves are leagal (if the kniwght is not pinned to the king)
        //Could be avoided if we don't check if the move is legal and take a king captura as the best move in search 
        //(Could be bad for performance but probably not that much since we have to check it anyway)
        return moves;
    }

    //Get moves for King (Precalculated)
    bitboard get_king_moves(bitboard king, bitboard other){
        bitboard moves = king_move_t[bb_sq(king)] & ~other;
        //Keep in mid is not being checked if it is a legal move
        return moves;
    }

    //Get capture moves for King (Precalculated)
    bitboard get_king_captures(bitboard king, bitboard other){
        bitboard moves = king_move_t[bb_sq(king)] & other;
        //Keep in mid is not being checked if it is a legal move
        return moves;
    }

    bitboard get_bishop_moves(bitboard bishop, bitboard other){
        int square = bb_sq(bishop);
        bitboard block_mask = bishop_rays[square]&other;
        
        u64 key = block_mask * bishopMagics[square];
        key >>= 64 - bishop_rellevant_bits[square];
        
        bitboard moves = bishop_move_t[bb_sq(bishop)][key];
        moves &= ~other;
        //We are not considering legality of the move!
        return moves;
    }

    bitboard get_bishop_captures(bitboard bishop, bitboard other, bitboard self){
        int square = bb_sq(bishop);
        bitboard block_mask = bishop_rays[square]&(other|self);
        
        u64 key = block_mask * bishopMagics[square];
        key >>= 64 - bishop_rellevant_bits[square];
        
        bitboard moves = bishop_move_t[bb_sq(bishop)][key];
        moves &= other;
        //We are not considering legality of the move!
        return moves;
    }

    bitboard get_rook_moves(bitboard rook, bitboard other){
        int square = bb_sq(rook);
        bitboard block_mask = rook_rays[square]&other;

        u64 key = block_mask * rookMagics[square];
        key >>= 64 - rook_rellevant_bits[square];
        
        bitboard moves = rook_move_t[square][key];
        moves &= ~other;
        //We are not considering legality of the move!
        return moves;
    }

    bitboard get_rook_captures(bitboard rook, bitboard other, bitboard self){
        int square = bb_sq(rook);
        bitboard block_mask = rook_rays[square]&(other|self);
        
        u64 key = block_mask * rookMagics[square];
        key >>= 64 - rook_rellevant_bits[square];
        
        bitboard moves = rook_move_t[bb_sq(rook)][key];
        moves &= other;
        //We are not considering legality of the move!
        return moves;
    }

    bitboard get_queen_moves(bitboard queen, bitboard other){
        return get_bishop_moves(queen, other) | get_rook_moves(queen, other);
    }

    bitboard get_queen_captures(bitboard queen, bitboard other, bitboard self){
        return get_bishop_captures(queen, other, self) | get_rook_captures(queen, other, self);
    }

    board array_to_board(int cboard[8][8], int color[8][8]){
        board bd;
        bd.white = 0ULL;
        bd.black = 0ULL;
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                if(color[i][j] == 1){
                    bd.white|=getbboard(7-i,j);
                    bd.w_pieces[cboard[i][j]-1].push_back(getbboard(7-i,j));
                }
                else if(color[i][j] == 2){
                    bd.black|=getbboard(7-i,j);
                    bd.b_pieces[cboard[i][j]-1].push_back(getbboard(7-i,j));
                } 

        return bd;
    }

}