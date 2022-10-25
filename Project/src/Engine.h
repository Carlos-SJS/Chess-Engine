#pragma once

#include <climits> 
#include <string>
#include <fstream>
#include <unordered_map>
#include "Generator.h"
#include "BitUtil.h"

using namespace std;

class Engine{
    private:
        const int SEARCH_DEEPNESS = 3;

        const string BBOARD_SQUARE_FILE = "../data/sq_to_bboard.bitboard";\

        bool is_legal(board, move_pair, int);
        int move_count(board, int);

    public:
        unordered_map<bitboard, unsigned int> bboard_square;

        //engine initlization
        Engine();
        void init();

        //Engine functions
        vector<vector<int>> getMoves(int cboard[8][8], int color[8][8], bool, bool, bool, bool);
        pair<int, int> get_engine_move(int cboard[8][8], int color[8][8], bool, bool, bool, bool);
        int get_array_eval(int cboard[8][8], int color[8][8]);

        int board_state(int cboard[8][8], int color[8][8], int turn);

        bool in_check(int c, int cboard[8][8], int color[8][8]);
};

namespace Search{
    int alphabeta(board, int, int, int, bool);
};