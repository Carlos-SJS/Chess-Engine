#include <climits> 
#include <string>
#include <fstream>
#include <unordered_map>
#include "Generator.h"

using namespace std;

class Engine{
    private:
        const int SEARCH_DEEPNESS = 5;

        const string BBOARD_SQUARE_FILE = "../data/sq_to_bboard.bitboard";
    public:
        unordered_map<bitboard, unsigned int> bboard_square;

        Engine();
        void init();
        vector<vector<int>> getMoves(int cboard[8][8], int color[8][8]);
        pair<int, int> get_engine_move(int cboard[8][8], int color[8][8]);
};

namespace Search{
    int alphabeta(board, int, int, int, bool);
};