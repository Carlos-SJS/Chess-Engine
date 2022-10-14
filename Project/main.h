#include <iostream>
#include <SDL_IMAGE.h>
#include <SDL_TTF.h>
#include <SDL.h>
#include <map>
#include <vector>
#include <utility>
#include <string>
#include <pthread.h>

#include<string>
#include <windows.h>
#include <Lmcons.h>
#include <locale>
#include <clocale>
#include <cstdlib>
#include <codecvt>

#include "src/Engine.h"
#include "src/BitUtil.h"

using namespace std;

Engine engine;
vector<vector<int>> set_of_moves;
int boardtc[8][8];
int colortc[8][8];
bool calculate_moves_flag = 1;


class GameRenderer{
    private:
        //Constants needed for SDL stuff
        const int CELL_SIZE = 90;
        const int TDBORDER_SIZE = (CELL_SIZE/3)*2;
        const int LRBORDER_SIZE = CELL_SIZE/3;
        const int SCREEN_HEIGHT = CELL_SIZE * 8 + TDBORDER_SIZE * 2;
        const int SCREEN_WIDTH = CELL_SIZE * 8 + LRBORDER_SIZE * 2;

        const int PIZE_OFFSET = CELL_SIZE/8 + ((CELL_SIZE/8)&1);

        TTF_Font* font;
        SDL_Color text_color = {220,220,220};
        SDL_Texture* human_text;
        SDL_Texture* bot_text;
        

        //Creating engine obj
        //Engine engine;


        //Chessboard reprecentaction data 
        //0 -> void, 1 -> pawn, 2 -> knight, 3 -> bishop, 4 -> rook, 5 -> queen, 6 -> king
        int board[8][8] = {{4, 3, 2, 5, 6, 2, 3, 4},
                           {1, 1, 1, 1, 1, 1, 1, 1},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 4, 0, 0, 0, 0, 0, 0},
                           {1, 1, 1, 1, 1, 1, 1, 1},
                           {4, 3, 2, 5, 6, 2, 3, 4}};
        //0 -> void, 1 -> white, 2 -> black
        int color[8][8] = {{2, 2, 2, 2, 2, 2, 2, 2},
                           {2, 2, 2, 2, 2, 2, 2, 2},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 2, 0, 0, 0, 0, 0, 0},
                           {1, 1, 1, 1, 1, 1, 1, 1},
                           {1, 1, 1, 1, 1, 1, 1, 1}};
        
        map<string, pair<int, int>> position {{"A1", {7,0}}, {"A2", {7,1}}, {"A3", {7,2}}, {"A4", {7,3}}, {"A5", {7,4}}, {"A6", {7,5}}, {"A7", {7,6}}, {"A8", {7,7}},
                                              {"B1", {6,0}}, {"B2", {6,1}}, {"B3", {6,2}}, {"B4", {6,3}}, {"B5", {6,4}}, {"B6", {6,5}}, {"B7", {6,6}}, {"B8", {6,7}},
                                              {"C1", {5,0}}, {"C2", {5,1}}, {"C3", {5,2}}, {"C4", {5,3}}, {"C5", {5,4}}, {"C6", {5,5}}, {"C7", {5,6}}, {"C8", {5,7}},  
                                              {"D1", {4,0}}, {"D2", {4,1}}, {"D3", {4,2}}, {"D4", {4,3}}, {"D5", {4,4}}, {"D6", {4,5}}, {"D7", {4,6}}, {"D8", {4,7}},
                                              {"E1", {3,0}}, {"E2", {3,1}}, {"E3", {3,2}}, {"E4", {3,3}}, {"E5", {3,4}}, {"E6", {3,5}}, {"E7", {3,6}}, {"E8", {3,7}},
                                              {"F1", {2,0}}, {"F2", {2,1}}, {"F3", {2,2}}, {"F4", {2,3}}, {"F5", {2,4}}, {"F6", {2,5}}, {"F7", {2,6}}, {"F8", {2,7}},
                                              {"G1", {1,0}}, {"G2", {1,1}}, {"G3", {1,2}}, {"G4", {1,3}}, {"G5", {1,4}}, {"G6", {1,5}}, {"G7", {1,6}}, {"G8", {1,7}},
                                              {"H1", {0,0}}, {"H2", {0,1}}, {"H3", {0,2}}, {"H4", {0,3}}, {"H5", {0,4}}, {"H6", {0,5}}, {"H7", {0,6}}, {"H8", {0,7}}
                                            };
        vector<SDL_Texture *> sprites;
        SDL_Surface* window_icon;

        //Game data
        vector<vector<int>> moves;
        int turn= 0;
        bool need_move_update = 1;

        //Rendering data
        bool rend_moves = 0;
        int move_to_draw = 0;

    public:
        void init();
        void renderer_loop();
        void send_move();
        void set_values(SDL_Renderer *);

        void copy_matrix();

        //Drawing functions
        void draw_board(SDL_Renderer *);
        void draw_pieces(SDL_Renderer *);
        void load_files(SDL_Renderer *);

        void draw_moves(SDL_Renderer *);
};

struct RendererException: public exception {
    const char* what() const throw();
};