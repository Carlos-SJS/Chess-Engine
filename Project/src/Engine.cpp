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

    vector<vector<int>> move_list(64);
    for(auto piece: moves.moves){
        bitboard move = piece.to;
        while(move){
            move_list[piece.from].push_back(bb_sq(lsb(move)));
            clearb(move, lsb(move));
        }
    }

    return move_list;
}