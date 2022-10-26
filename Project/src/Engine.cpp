#include "Engine.h"
#include "logger.h"
#include "BitUtil.h"
#include "Evaluator.h"

Engine::Engine(){
    
}

void Engine::init(){
    logger.log("Initializig Engine");
    Generator::load_data();
    
    /*ifstream bbsq(BBOARD_SQUARE_FILE);
    if(bbsq.fail()){
        logger.error("Couldn't load engine data \"" + BBOARD_SQUARE_FILE + "\"");
    }
    int ign, b;
    for(int i=0; i<64; i++){
        bbsq >> ign >> b;
        bboard_square[b] = i;
    }*/    

    logger.success("Engine succesfully initialized");
}

//There is a bug with castling here
bool EngineUtil::is_legal(board b, move_pair m, int c){
    if(c == 1){
        b = Generator::sim_board(b, m, c);
        moveset moves = Generator::get_moves_b(b);

        return ((b.w_pieces[5][0]&moves.attacks)==0);
    }else{
        b = Generator::sim_board(b, m, c);
        moveset moves = Generator::get_moves_w(b);

        return ((b.b_pieces[5][0]&moves.attacks)==0);
    }
}

vector<vector<int>> Engine::getMoves(int cboard[8][8], int color[8][8],  bool white_lc, bool white_rc, bool black_lc, bool black_rc){
    logger.log("Calculating moves for white player");

    board b = Generator::array_to_board(cboard, color, white_lc, white_rc, black_lc, black_rc);

    //if(__builtin_popcount(b.white|b.black) < 10) SEARCH_DEEPNESS = 5;
    //else SEARCH_DEEPNESS = 3;

    logger.log("Getting moves");
    moveset moves = Generator::get_moves_w(b);

    vector<vector<int>> move_list(64);
    
    logger.log("Converting moves");
    for(int i=0; i<moves.moves.size(); i++){
        int piece = bb_sq(b.w_pieces[moves.moves[i].from.first][moves.moves[i].from.second]);
        bitboard move = moves.moves[i].to;
        while(move){
            if(EngineUtil::is_legal(b, make_pair(moves.moves[i].from, (1ULL<<lsb(move))), 1)) move_list[piece].push_back(bb_sq(move));
            clearb(move, lsb(move));
        }
    }

    for(int i=0; i<moves.captures.size(); i++){
        int piece = bb_sq(b.w_pieces[moves.captures[i].from.first][moves.captures[i].from.second]);
        bitboard move = moves.captures[i].to;
        while(move){
            if(EngineUtil::is_legal(b, make_pair(moves.captures[i].from, (1ULL<<lsb(move))), 1)) move_list[piece].push_back(bb_sq(move));
            clearb(move, lsb(move));
        }
    }

   /*
   // Get moves for black
    board b = Generator::array_to_board(cboard, color);
    moveset moves = Generator::get_moves_b(b);

    vector<vector<int>> move_list(64);
    for(int i=0; i<moves.moves.size(); i++){
        int piece = bb_sq(b.b_pieces[moves.moves[i].from.first][moves.moves[i].from.second]);
        bitboard move = moves.moves[i].to;
        while(move){
            move_list[piece].push_back(bb_sq(move));
            clearb(move, lsb(move));
        }
    }

    for(int i=0; i<moves.captures.size(); i++){
        int piece = bb_sq(b.b_pieces[moves.captures[i].from.first][moves.captures[i].from.second]);
        bitboard move = moves.captures[i].to;
        while(move){
            move_list[piece].push_back(bb_sq(move));
            clearb(move, lsb(move));
        }
    }
    */

    return move_list;
}

pair<int, int> Engine::get_engine_move(int cboard[8][8], int color[8][8], bool white_lc, bool white_rc, bool black_lc, bool black_rc){
    logger.log("Calculating engine move");
    
    board b = Generator::array_to_board(cboard, color, white_lc, white_rc, black_lc, black_rc);
    moveset moves = Generator::get_moves_b(b);
    
    bool c = 0;
    int best = 0,v;
    bitboard best_to = 0ULL;
    int move_value = INT_MAX;
    
    int alpha = INT_MIN;
    int beta = INT_MAX;

    bool cutoff = 0;


    for(int i=0; i<moves.captures.size(); i++){
        move_pair move = moves.captures[i];
        while(move.to > 0){
            if(EngineUtil::is_legal(b, make_pair(move.from, (1ULL << lsb(move.to))), 2)){
                v = Search::alphabeta(Generator::sim_board(b, make_pair(move.from, (1ULL << lsb(move.to))), 2), SEARCH_DEEPNESS,alpha, beta, 1);
                beta = min(beta, v);
                if(v < move_value){
                    move_value = v;
                    best =  i; c = 1;
                    best_to = 1ULL<<lsb(move.to);
                }
            }
            clearb(move.to, lsb(move.to));
        }
    }

    for(int i=0; i<moves.moves.size(); i++){
        move_pair move = moves.moves[i];
        while(move.to > 0){
            if(EngineUtil::is_legal(b, make_pair(move.from, (1ULL << lsb(move.to))), 2)){
                v = Search::alphabeta(Generator::sim_board(b, make_pair(move.from, (1ULL << lsb(move.to))), 2), SEARCH_DEEPNESS,INT_MIN, INT_MAX, 1);
                beta = min(beta, v);
                if(v < move_value){
                    move_value = v;
                    best =  i; c = 0;
                    best_to = 1ULL<<lsb(move.to);
                }
            }
            clearb(move.to, lsb(move.to));
        }
    }

    logger.log("Engine move calculated!, expected value: " + to_string(move_value));

    /*logger.log("Engine move: ");
    if(c){
        logger.printbboard(b.b_pieces[moves.captures[best].from.first][moves.captures[best].from.second]);
        logger.printbboard(best_to);
    }else{
        logger.printbboard(b.b_pieces[moves.moves[best].from.first][moves.moves[best].from.second]);
        logger.printbboard(best_to);
    }*/

    if(c) return {bb_sq(b.b_pieces[moves.captures[best].from.first][moves.captures[best].from.second]) ,bb_sq(best_to)};
    else return {bb_sq(b.b_pieces[moves.moves[best].from.first][moves.moves[best].from.second]) ,bb_sq(best_to)}; 
}

int Search::alphabeta(board bd, int depth, int alpha, int beta, bool maximize){

    if(depth == 0){
        return Evaluator::get_board_eval(bd, 1) - Evaluator::get_board_eval(bd, 2);
    }
    
    //Check mate (White)
    if(bd.b_pieces[5].size() == 0) return INT_MAX;
    //Check mate (black)
    if(bd.w_pieces[5].size() == 0) return INT_MIN;


    if(maximize){
        int value = INT_MIN;
        boolean cutoff=0;

        moveset moves = Generator::get_moves_w(bd);
        if(EngineUtil::move_count(bd, 1) == 0) value = 0;

        //Could use move ordering euristcs here
        for(auto it = moves.captures.begin(); it!=moves.captures.end() && !cutoff; it++){
            move_pair move = *it;
            while(move.to > 0){
                value = max(value, alphabeta(Generator::sim_board(bd, make_pair(move.from, (1ULL << lsb(move.to))), 1), depth-1, alpha, beta, 0));
                if(value >= beta){
                    cutoff = 1;
                    break;
                }
                alpha = max(alpha, value);
                clearb(move.to, lsb(move.to));
            }
        }
        if(!cutoff)for(auto it = moves.moves.begin(); it!=moves.moves.end() && !cutoff; it++){
            move_pair move = *it;
            while(move.to > 0){
                value = max(value, alphabeta(Generator::sim_board(bd, make_pair(move.from,( 1ULL << lsb(move.to))), 1), depth-1, alpha, beta, 0));
                if(value >= beta)break;
                alpha = max(alpha, value);
                clearb(move.to, lsb(move.to));
            }
        }
       
        return value;
    }else{
        int value = INT_MAX;
        boolean cutoff=0;
        
        moveset moves = Generator::get_moves_b(bd);
        if(EngineUtil::move_count(bd, 2) == 0) value = 0;


        //Could use move ordering euristcs here
        for(auto it = moves.captures.begin(); it!=moves.captures.end() && !cutoff; it++){
            move_pair move = *it;
            while(move.to > 0){
                value = min(value, alphabeta(Generator::sim_board(bd, make_pair(move.from, (1ULL << lsb(move.to))), 2), depth-1, alpha, beta, 1));
                if(value <= alpha){
                    cutoff = 1;
                    break;
                }
                beta = min(beta, value);
                clearb(move.to, lsb(move.to));
            }
        }
        if(!cutoff)for(auto it = moves.moves.begin(); it!=moves.moves.end() && !cutoff; it++){
            move_pair move = *it;
            while(move.to > 0){
                value = min(value, alphabeta(Generator::sim_board(bd, make_pair(move.from, (1ULL << lsb(move.to))), 2), depth-1, alpha, beta, 1));
                if(value <= alpha)break;
                beta = min(beta, value);
                clearb(move.to, lsb(move.to));
            }
        }
        return value;
    }
}

int Engine::get_array_eval(int cboard[8][8], int color[8][8]){
    board b = Generator::array_to_board(cboard, color, 0, 0, 0, 0);
    return Evaluator::get_board_eval(b, 1) - Evaluator::get_board_eval(b, 2);
}

int EngineUtil::move_count(board b, int c){
    moveset moves;
    if(c == 1) moves = Generator::get_moves_w(b);
    else moves = Generator::get_moves_b(b);

    int cnt = 0;

    for(auto m : moves.moves){
        while(m.to){
            if(is_legal(b, make_pair(m.from, (1ULL<<lsb(m.to))), c)) cnt++;
            clearb(m.to, lsb(m.to));
        }
    }

    for(auto m : moves.captures){
        while(m.to){
            if(is_legal(b, make_pair(m.from, (1ULL<<lsb(m.to))), c)) cnt++;
            clearb(m.to, lsb(m.to));
        }
    }

    return cnt;
}

//0 normal, 1 white won, 2 black won, 3 draw
int Engine::board_state(int cboard[8][8], int color[8][8], int turn){
    board b =  Generator::array_to_board(cboard, color, 0, 0, 0, 0);
    
    int wmoves = EngineUtil::move_count(b, 1);
    int bmoves = EngineUtil::move_count(b, 2);

    if(turn == 1){
        if(wmoves == 0 && bmoves == 0){
            return 3;
        }if(wmoves == 0){
            moveset bm = Generator::get_moves_b(b);
            if((bm.attacks&b.w_pieces[5][0]) != 0) return 2;
            else return 3;
        }
    }else{
        if(wmoves == 0 && bmoves == 0) return 3;
        if(bmoves == 0){
            moveset wm = Generator::get_moves_w(b);
            if((wm.attacks&b.b_pieces[5][0]) != 0) return 1;
            else return 3;
        }
    }


    return 0;
}

bool Engine::in_check(int c, int cboard[8][8], int color[8][8]){
    board b  = Generator::array_to_board(cboard, color, 0, 0, 0, 0);
    
    moveset m;
    if(c == 1) m = Generator::get_moves_b(b, 0);
    else m = Generator::get_moves_w(b, 0);

    if(c == 1) return (b.w_pieces[5][0]&m.attacks) != 0;
    else return (b.b_pieces[5][0]&m.attacks) != 0;

}