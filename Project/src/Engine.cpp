#include "Engine.h"
#include "logger.h"
#include "Evaluator.h"

Engine::Engine(){
    
}

void Engine::init(){
    logger.log("Initializig Engine");
    Generator::load_data();

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
        int piece = bb_sq(moves.moves[i].from);
        bitboard move = moves.moves[i].to;
        while(move){
            move_list[piece].push_back(bb_sq(move));
            clearb(move, lsb(move));
        }
    }

    for(int i=0; i<moves.captures.size(); i++){
        int piece = bb_sq(moves.captures[i].from);
        bitboard move = moves.captures[i].to;
        while(move){
            move_list[piece].push_back(bb_sq(move));
            clearb(move, lsb(move));
        }
    }

    return move_list;
}