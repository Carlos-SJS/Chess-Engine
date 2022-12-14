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
        return get_moves_w(b, 1);
    }

    moveset get_moves_w(board b, bool castle){
        // 0 -> pawn, 1 -> knight, 2 -> bishop, 3 -> rook, 4 -> queen, 5 -> king   
        moveset moves = get_moves(b.w_pieces, b.white, b.black);
        moves.color = 1;

        //White Pawns
        for(int i=0; i<b.w_pieces[0].size(); i++){
            if(b.w_pieces[0][i] & b.white){
                auto pawn = b.w_pieces[0][i];
                if(white_pawn_move_t[bb_sq(pawn)])
                    if((white_pawn_move_t[bb_sq(pawn)] & (b.black|b.white)) > pawn<<8 || (white_pawn_move_t[bb_sq(pawn)] & (b.black|b.white)) == 0)
                        moves.moves.push_back({{0, i}, white_pawn_move_t[bb_sq(pawn)] & ~(b.black|b.white)});

                if(white_pawn_capture_t[bb_sq(pawn)]&b.black){
                    moves.captures.push_back({{0, i}, white_pawn_capture_t[bb_sq(pawn)] & b.black});
                    moves.attacks |= white_pawn_capture_t[bb_sq(pawn)] & b.black;
                }
            }
        }

        //Castling
        if(castle && b.w_pieces[5].size() > 0){
            board ob = b;
            ob.white |= leftcst(b.w_pieces[5][0])|rightcst(b.w_pieces[5][0]);
            if(b.left_white_castle && (b.white&(b.w_pieces[5][0]<<4)) && (leftcstall(b.w_pieces[5][0])&(b.black|b.white)) == 0 && ((leftcst(b.w_pieces[5][0])|b.w_pieces[5][0])&get_moves_b(ob, 0).attacks) == 0){
                moves.moves.push_back({{5,0}, b.w_pieces[5][0]>>2});
            }

            if(b.right_white_castle && (b.white&(b.w_pieces[5][0]<<3)) && (rightcst(b.w_pieces[5][0])&(b.black|b.white)) == 0 && ((rightcst(b.w_pieces[5][0])|b.w_pieces[5][0])&get_moves_b(ob, 0).attacks) == 0){
                moves.moves.push_back({{5,0}, b.w_pieces[5][0]<<2});
            }
        }

        return moves;
    }

    //Get moves for black
    moveset get_moves_b(board b){
        return get_moves_b(b, 1);
    }

    moveset get_moves_b(board b, bool castle){
        // 0 -> pawn, 1 -> knight, 2 -> bishop, 3 -> rook, 4 -> queen, 5 -> king   
        moveset moves = get_moves(b.b_pieces, b.black, b.white);
        moves.color = 2;

        //Black Pawns
        for(int i=0; i<b.b_pieces[0].size(); i++){
            if(b.b_pieces[0][i] & b.black){
                auto pawn = b.b_pieces[0][i];
                if(black_pawn_move_t[bb_sq(pawn)])
                    if((black_pawn_move_t[bb_sq(pawn)] & (b.black|b.white)) < pawn>>8 || (black_pawn_move_t[bb_sq(pawn)] & (b.black|b.white)) == 0)
                        moves.moves.push_back({{0, i}, black_pawn_move_t[bb_sq(pawn)] & ~(b.black|b.white)});

                if(black_pawn_capture_t[bb_sq(pawn)]&b.white){
                    moves.captures.push_back({{0, i}, black_pawn_capture_t[bb_sq(pawn)] & b.white});

                    moves.attacks |= black_pawn_capture_t[bb_sq(pawn)] & b.white;
                }
            }
        }

        if(castle && b.b_pieces[5].size()){
            board ob = b;
            ob.black |= leftcst(b.b_pieces[5][0])|rightcst(b.b_pieces[5][0]);
            if(b.left_black_castle && (b.white&(b.w_pieces[5][0]<<4)) && (leftcst(b.b_pieces[5][0])&(b.black|b.white)) == 0 && ((leftcst(b.b_pieces[5][0])|b.b_pieces[5][0])&get_moves_w(ob, 0).attacks) == 0){
                moves.moves.push_back({{5,0}, b.b_pieces[5][0]>>2});
            }

            if(b.right_black_castle && (b.white&(b.w_pieces[5][0]>>3)) && (rightcst(b.b_pieces[5][0])&(b.black|b.white)) == 0 && ((rightcst(b.b_pieces[5][0])|b.b_pieces[5][0])&get_moves_w(ob, 0).attacks) == 0){
                moves.moves.push_back({{5,0}, b.b_pieces[5][0]<<2});
            }
        }

        return moves;
    }

    //Get general moves (Knight, Bishop, Rook, Queen, King)
    moveset get_moves(vector<boardset> pieces, bitboard own, bitboard other){
        moveset moves;
        bitboard m,c;

        moves.attacks = 0ULL;

        //Knight
        for(int i=0; i<pieces[1].size(); i++){
            if(pieces[1][i] & own){
                auto p = pieces[1][i];
                m = get_knight_moves(p, (own|other));
                c = get_knight_captures(p, other);
                if(m) moves.moves.push_back({{1, i}, m});
                if(c) moves.captures.push_back({{1, i}, c});

                moves.attacks |= c;
            }
        }

        //Bishop
        for(int i=0; i<pieces[2].size(); i++){
            if(pieces[2][i] & own){
                auto p = pieces[2][i];
                m = get_bishop_moves(p, (own|other));
                c = get_bishop_captures(p, other, own);
                if(m) moves.moves.push_back({{2,i}, m});
                if(c) moves.captures.push_back({{2,i}, c});

                moves.attacks |= c;
            }
        }

        //Rook
        for(int i=0; i<pieces[3].size(); i++){
            if(pieces[3][i] & own){
                auto p = pieces[3][i];
                m = get_rook_moves(p, (own|other));
                c = get_rook_captures(p, other, own);
                if(m) moves.moves.push_back({{3,i}, m});
                if(c) moves.captures.push_back({{3,i}, c});

                moves.attacks |= c;
            }
        }

        //Queen
        for(int i=0; i<pieces[4].size(); i++){
            if(pieces[4][i] & own){
                auto p = pieces[4][i];
                m = get_queen_moves(p, (own|other));
                c = get_queen_captures(p, other, own);
                if(m) moves.moves.push_back({{4,i}, m});
                if(c) moves.captures.push_back({{4,i}, c});

                moves.attacks |= c;
            }
        }

        //King
        for(int i=0; i<pieces[5].size(); i++){
            if(pieces[5][i] & own){
                auto p = pieces[5][i];
                m = get_king_moves(p, (own|other));
                c = get_king_captures(p, other);

                if(m) moves.moves.push_back({{5,i}, m});
                if(c) moves.captures.push_back({{5,i}, c});

                moves.attacks |= c;
            }
        }

        return moves;
    }


    //Get moves for Knight (Precalculated)
    bitboard get_knight_moves(bitboard knight, bitboard other){
        bitboard moves = (knight_move_t[bb_sq(knight)] & (~other));
        return moves;
    }

    //Get capture moves for Knight (Precalculated)
    bitboard get_knight_captures(bitboard knight, bitboard other){
        bitboard moves = knight_move_t[bb_sq(knight)] & other;
        return moves;
    }

    //Get moves for King (Precalculated)
    bitboard get_king_moves(bitboard king, bitboard other){
        bitboard moves = king_move_t[bb_sq(king)] & ~other;
        return moves;
    }

    //Get capture moves for King (Precalculated)
    bitboard get_king_captures(bitboard king, bitboard other){
        bitboard moves = king_move_t[bb_sq(king)] & other;
        return moves;
    }

    bitboard get_bishop_moves(bitboard bishop, bitboard other){
        int square = bb_sq(bishop);
        bitboard block_mask = bishop_rays[square]&other;
        
        u64 key = block_mask * bishopMagics[square];
        key >>= 64 - bishop_rellevant_bits[square];
        
        bitboard moves = bishop_move_t[bb_sq(bishop)][key];
        moves &= ~other;

        return moves;
    }

    bitboard get_bishop_captures(bitboard bishop, bitboard other, bitboard self){
        int square = bb_sq(bishop);
        bitboard block_mask = bishop_rays[square]&(other|self);
        
        u64 key = block_mask * bishopMagics[square];
        key >>= 64 - bishop_rellevant_bits[square];
        
        bitboard moves = bishop_move_t[bb_sq(bishop)][key];
        moves &= other;

        return moves;
    }

    bitboard get_rook_moves(bitboard rook, bitboard other){
        int square = bb_sq(rook);
        bitboard block_mask = rook_rays[square]&other;

        u64 key = block_mask * rookMagics[square];
        key >>= 64 - rook_rellevant_bits[square];
        
        bitboard moves = rook_move_t[square][key];
        moves &= ~other;

        return moves;
    }

    bitboard get_rook_captures(bitboard rook, bitboard other, bitboard self){
        int square = bb_sq(rook);
        bitboard block_mask = rook_rays[square]&(other|self);
        
        u64 key = block_mask * rookMagics[square];
        key >>= 64 - rook_rellevant_bits[square];
        
        bitboard moves = rook_move_t[bb_sq(rook)][key];
        moves &= other;

        return moves;
    }

    bitboard get_queen_moves(bitboard queen, bitboard other){
        return get_bishop_moves(queen, other) | get_rook_moves(queen, other);
    }

    bitboard get_queen_captures(bitboard queen, bitboard other, bitboard self){
        return get_bishop_captures(queen, other, self) | get_rook_captures(queen, other, self);
    }

    board array_to_board(int cboard[8][8], int color[8][8], bool white_lc, bool white_rc, bool black_lc, bool black_rc){
        board bd;
        bd.white = 0ULL;
        bd.black = 0ULL;

        bd.left_white_castle = white_lc;
        bd.right_white_castle = white_rc;
        bd.left_black_castle = black_lc;
        bd.right_black_castle = black_rc;

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

    board sim_board(board b, move_pair m, int color){
        if((m.to&(~(1ULL<<lsb(m.to)))) != 0){
            logger.warning("Move with more than one position!");
            logger.printbboard(m.to);
        }

        if(color == 1){ //White
            b.white &= ~b.w_pieces[m.from.first][m.from.second];
            b.black &= ~m.to;
            b.white |= m.to;

            if(m.from.first == 0 && b.w_pieces[m.from.first][m.from.second] > (1ULL<<48)){ //Queen promotion
                //logger.warning("Queening stuff");
                b.w_pieces[4].push_back(m.to);
                swap(b.w_pieces[0][m.from.second], b.w_pieces[0][b.w_pieces[0].size()-1]);
                b.w_pieces[0].pop_back();
            }else if(m.from.first == 5 && (b.w_pieces[5][m.from.second]>>2) == m.to){
                //logger.error("FUCK THIS FUCKING SHIT (left castling)");
                //logger.printbboard(b.white|b.black);

                b.w_pieces[m.from.first][m.from.second] = m.to;
                
                for(int i=0; i<b.w_pieces[3].size(); i++) if(b.w_pieces[3][i]==(m.to>>2)){b.w_pieces[3][i]<<=3;break;}
                b.white &= ~(m.to>>2);
                b.white |=  m.to<<1;

                b.left_white_castle = 0;
                b.right_white_castle = 0;

                //logger.printbboard(b.white|b.black);
            }else if(m.from.first == 5 && (b.w_pieces[5][m.from.second]<<2) == m.to){
                //logger.error("FUCK THIS FUCKING SHIT (right castling)");
                //logger.printbboard(b.white|b.black);

                b.w_pieces[m.from.first][m.from.second] = m.to;
                
                for(int i=0; i<b.w_pieces[3].size(); i++)
                    if(b.w_pieces[3][i]==(m.to<<1)){
                        b.white &= ~b.w_pieces[3][i];
                        b.w_pieces[3][i]>>=2;
                        b.white|=b.w_pieces[3][i]; 
                        break;
                    }

                b.left_white_castle = 0;
                b.right_white_castle = 0;

                //logger.printbboard(b.white|b.black);
            }else b.w_pieces[m.from.first][m.from.second] = m.to;

            if(b.w_pieces[5].size() == 0) b.left_white_castle = 0, b.right_white_castle = 0;
            else if(m.from.first == 5) b.left_white_castle = 0, b.right_white_castle = 0;
            else if(m.from.first == 3){
                if(b.left_white_castle && b.w_pieces[m.from.first][m.from.second] < b.w_pieces[5][0]) b.left_white_castle = 0;
                else if(b.right_white_castle && b.w_pieces[m.from.first][m.from.second] > b.w_pieces[5][0]) b.right_white_castle = 0;
            }

        }else{ //Black
            b.black &= ~b.b_pieces[m.from.first][m.from.second];
            b.white &= ~m.to;
            b.black |= m.to;

            if(m.from.first == 0 && b.b_pieces[m.from.first][m.from.second] < (1ULL<<16)){ //Queen promotion
                //logger.warning("Queening stuff");
                b.b_pieces[4].push_back(m.to);
                swap(b.b_pieces[0][m.from.second], b.b_pieces[0][b.b_pieces[0].size()-1]);
                b.b_pieces[0].pop_back();
            }else if(m.from.first == 5 && b.b_pieces[5][m.from.second]>>2 == m.to){
                b.b_pieces[m.from.first][m.from.second] = m.to;
                
                for(int i=0; i<b.b_pieces[3].size(); i++) if(b.b_pieces[3][i]&(m.to>>2)){b.b_pieces[3][i]>>=3;break;}
                b.black &= ~m.to>>2;
                b.black |=  m.to<<1;

                b.left_black_castle = 0;
                b.right_black_castle = 0;
            }else if(m.from.first == 5 && b.b_pieces[5][m.from.second]<<2 == m.to){
                b.b_pieces[m.from.first][m.from.second] = m.to;

                for(int i=0; i<b.b_pieces[3].size(); i++) if(b.b_pieces[3][i]&(m.to<<1)){b.b_pieces[3][i]<<=2; break;}
                b.black &= ~m.to<<1;
                b.black &=  m.to>>1;

                b.left_black_castle = 0;
                b.right_black_castle = 0;
            }else b.b_pieces[m.from.first][m.from.second] = m.to;

            if(b.b_pieces[5].size() == 0) b.left_black_castle = 0, b.right_black_castle = 0;
            else if(m.from.first == 5) b.left_black_castle = 0, b.right_black_castle = 0;
            else if(m.from.first == 3){
                if(b.left_black_castle && b.b_pieces[m.from.first][m.from.second] < b.b_pieces[5][0]) b.left_black_castle = 0;
                else if(b.right_black_castle && b.b_pieces[m.from.first][m.from.second] > b.b_pieces[5][0]) b.right_black_castle = 0;
            }
        }

        //Eliminating ghost white pieces (Can be optimized)
        for(int i=0; i<b.w_pieces.size(); i++){
            for(int j=0; j < b.w_pieces[i].size(); j++){
                if((b.white & b.w_pieces[i][j]) == 0){
                    swap(b.w_pieces[i][b.w_pieces[i].size()-1], b.w_pieces[i][j]);
                    b.w_pieces[i].pop_back();
                    j--;
                }
            }
        }

        //Eliminating ghost black pieces (Can be optimized)
        for(int i=0; i<b.b_pieces.size(); i++){
            for(int j=0; j < b.b_pieces[i].size(); j++){
                if((b.black & b.b_pieces[i][j]) == 0){
                    swap(b.b_pieces[i][b.b_pieces[i].size()-1], b.b_pieces[i][j]);
                    b.b_pieces[i].pop_back();
                    j--;
                }
            }
        }
        
        return b;
    }

}