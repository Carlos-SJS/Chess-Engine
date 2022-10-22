#include "Engine.h"
#include "logger.h"
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

    /*int boardc[8][8] = {{4, 2, 3, 5, 6, 3, 2, 4},
                           {1, 1, 1, 1, 1, 1, 1, 1},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {1, 1, 1, 1, 1, 1, 1, 1},
                           {4, 2, 3, 5, 6, 3, 2, 4}};
    //0 -> void, 1 -> white, 2 -> black
    int color[8][8] = {{2, 2, 2, 2, 2, 2, 2, 2},
                        {2, 2, 2, 2, 2, 2, 2, 2},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1, 1, 1, 1}};

    board bd = Generator::array_to_board(boardc, color);
    moveset m = Generator::get_moves_w(bd);
    move_pair move = m.moves[3];
    board sb = Generator::sim_board(bd, make_pair(move.from, 1ULL << lsb(clearb(move.to, lsb(move.to)))), 1);
    logger.printbboard(sb.white|sb.black);*/
    

    logger.success("Engine succesfully initialized");
}

vector<vector<int>> Engine::getMoves(int cboard[8][8], int color[8][8]){
    logger.log("Calculating moves for white player");

    board b = Generator::array_to_board(cboard, color);
    moveset moves = Generator::get_moves_w(b);

    //logger.log(to_string(bb_sq(moves.moves[3].from)));
    //logger.printbboard(moves.moves[3].from);
    //logger.printbboard(moves.moves[3].to);


    vector<vector<int>> move_list(64);
    for(int i=0; i<moves.moves.size(); i++){
        int piece = bb_sq(b.w_pieces[moves.moves[i].from.first][moves.moves[i].from.second]);
        bitboard move = moves.moves[i].to;
        while(move){
            move_list[piece].push_back(bb_sq(move));
            clearb(move, lsb(move));
        }
    }

    for(int i=0; i<moves.captures.size(); i++){
        int piece = bb_sq(b.w_pieces[moves.captures[i].from.first][moves.captures[i].from.second]);
        bitboard move = moves.captures[i].to;
        while(move){
            move_list[piece].push_back(bb_sq(move));
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

pair<int, int> Engine::get_engine_move(int cboard[8][8], int color[8][8]){
    logger.log("Calculating engine move");

    board b = Generator::array_to_board(cboard, color);
    moveset moves = Generator::get_moves_b(b);
    
    bool c = 0;
    int best = 0,v;
    int move_value = INT_MAX;
    
    for(int i=0; i<moves.captures.size(); i++){
        move_pair move = moves.captures[i];
        while(move.to > 0){
            v = Search::alphabeta(Generator::sim_board(b, make_pair(move.from, 1ULL << lsb(move.to)), 2), SEARCH_DEEPNESS,INT_MIN, INT_MAX, 1);
            if(v < move_value){
                move_value = v;
                best =  i; c = 1;
            }
            clearb(move.to, lsb(move.to));
        }
    }

    for(int i=0; i<moves.moves.size(); i++){
        move_pair move = moves.moves[i];
        while(move.to > 0){
            v = Search::alphabeta(Generator::sim_board(b, make_pair(move.from, 1ULL << lsb(move.to)), 2), SEARCH_DEEPNESS,INT_MIN, INT_MAX, 1);
            if(v < move_value){
                move_value = v;
                best =  i; c = 0;
            }
            clearb(move.to, lsb(move.to));
        }
    }

    logger.log("Engine move calculated!, expected value: " + to_string(move_value));

    if(c) return {bb_sq(b.b_pieces[moves.captures[best].from.first][moves.captures[best].from.second]) ,bb_sq(moves.captures[best].to)};
    else return {bb_sq(b.b_pieces[moves.moves[best].from.first][moves.moves[best].from.second]) ,bb_sq(moves.moves[best].to)}; 
}

int Search::alphabeta(board bd, int depth, int alpha, int beta, bool maximize){
    //logger.log("Alpha beta call");

    if(depth == 0){
        //Return static board eval (quicense search ideally required)
        //logger.log("Evaluating position");
        return Evaluator::get_board_eval(bd, 1) - Evaluator::get_board_eval(bd, 2);
    }

    /*Check if position is in transposition table
    if(0){

    }
    */

    if(maximize){
        //logger.log("Maximizing at depth " + to_string(depth));

        int value = INT_MIN;
        boolean cutoff=0;
        moveset moves = Generator::get_moves_w(bd);
        //Could use move ordering euristcs here
        for(auto it = moves.captures.begin(); it!=moves.captures.end() && !cutoff; it++){
            move_pair move = *it;
            while(move.to > 0){
                value = max(value, alphabeta(Generator::sim_board(bd, make_pair(move.from, 1ULL << lsb(move.to)), 1), depth-1, alpha, beta, 0));
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
                value = max(value, alphabeta(Generator::sim_board(bd, make_pair(move.from, 1ULL << lsb(move.to)), 1), depth-1, alpha, beta, 0));
                if(value >= beta)break;
                alpha = max(alpha, value);
                clearb(move.to, lsb(move.to));
            }
        }

        //logger.log("Finished maximizing at depth " + to_string(depth));
        return value;
    }else{
        //logger.log("Minimizing at depth " + to_string(depth));

        int value = INT_MAX;
        boolean cutoff=0;
        moveset moves = Generator::get_moves_b(bd);
        //Could use move ordering euristcs here
        for(auto it = moves.captures.begin(); it!=moves.captures.end() && !cutoff; it++){
            move_pair move = *it;
            while(move.to > 0){
                value = min(value, alphabeta(Generator::sim_board(bd, make_pair(move.from, 1ULL << lsb(move.to)), 2), depth-1, alpha, beta, 1));
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
                value = min(value, alphabeta(Generator::sim_board(bd, make_pair(move.from, 1ULL << lsb(move.to)), 2), depth-1, alpha, beta, 1));
                if(value <= alpha)break;
                beta = min(beta, value);
                clearb(move.to, lsb(move.to));
            }
        }

        //logger.log("Finished minimizing at depth " + to_string(depth));
        return value;
    }
}

int Engine::get_array_eval(int cboard[8][8], int color[8][8]){
    board b = Generator::array_to_board(cboard, color);
    return Evaluator::get_board_eval(b, 1) - Evaluator::get_board_eval(b, 2);
}
