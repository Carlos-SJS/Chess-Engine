#pragma once

#include <climits> 
#include <stdlib.h>
#include <time.h> 
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include "Generator.h"
#include "BitUtil.h"

#define rnd_e(x) (x[rand() % x.size()]) 
#define absv(x) ((x)<0?-(x):(x))
#define valid(x, y) (x>=0 && y>=0 && x<8 && y <8)

using namespace std;

class Engine{
    private:
        int SEARCH_DEEPNESS = 3;

        const string BBOARD_SQUARE_FILE = "data/sq_to_bboard.bitboard";

        const string OPENNING_BOOK_FILE = "data/opening_book.txt";
        map<string, vector<string>> opening_book;


    public:
        unordered_map<bitboard, unsigned int> bboard_square;

        //engine initlization
        Engine();
        void init();

        //Engine functions
        vector<vector<int>> getMoves(int cboard[8][8], int color[8][8], bool, bool, bool, bool);
        pair<int, int> get_engine_move(int cboard[8][8], int color[8][8], bool, bool, bool, bool);
        pair<int, int> opening_lookup(int cboard[8][8], int color[8][8], string);
        int get_array_eval(int cboard[8][8], int color[8][8]);

        int board_state(int cboard[8][8], int color[8][8], int turn);
        bool in_check(int c, int cboard[8][8], int color[8][8]);
};

namespace Search{
    int alphabeta(board, int, int, int, bool);
};

namespace EngineUtil{
    int move_count(board, int);
    bool is_legal(board, move_pair, int);
};