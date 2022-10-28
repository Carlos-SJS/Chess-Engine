#include "main.h"
#include "src/logger.h"

//audio_sample audio_move("../Resources/Sound/game/move.wav", 1);

//Implementing Game Renderer Methodes
void GameRenderer::draw_pieces(SDL_Renderer *rend){
    try{
        SDL_Rect srect;
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if(board[i][j] != 0){
                    srect = {LRBORDER_SIZE + j*CELL_SIZE + (PIZE_OFFSET>>1), TDBORDER_SIZE + i*CELL_SIZE + (PIZE_OFFSET>>1), CELL_SIZE - PIZE_OFFSET, CELL_SIZE - PIZE_OFFSET};
                    SDL_RenderCopy(rend, sprites[((board[i][j]-1)*2)+color[i][j]-1], NULL, &srect);
                }
            }
        }
    }catch(exception &e){
        logger.error("Error rendering pieces sprites: " + string(e.what()));
    }catch(...){
        logger.error("Error ocurred while rendering pieces sprites");
    } 
}

void GameRenderer::draw_moves(SDL_Renderer *rend){ 
    for(auto move: moves[move_to_draw]){
        pair<int, int> c = sq_cd(move);
        //logger.log(to_string(move) + " -> " + to_string(c.cY) + ", " + to_string(c.cX));
        if(board[7-c.cY][c.cX] == 0){
            SDL_Rect rect = {LRBORDER_SIZE + c.cX*CELL_SIZE + (CELL_SIZE-CELL_SIZE/5)/2, TDBORDER_SIZE + (7-c.cY)*CELL_SIZE + (CELL_SIZE-CELL_SIZE/5)/2,CELL_SIZE/5, CELL_SIZE/5};
            SDL_RenderCopy(rend, sprites[12], NULL, &rect);
        }else{
            SDL_Rect rect = {LRBORDER_SIZE + c.cX*CELL_SIZE, TDBORDER_SIZE + (7-c.cY)*CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(rend, sprites[13], NULL, &rect);
        }
    }
}

void GameRenderer::draw_board(SDL_Renderer *rend){
    try{
        SDL_Rect sq;
        SDL_SetRenderDrawColor(rend, 100,100,100, SDL_ALPHA_OPAQUE);
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++){
                if(((i+j)&1)==0){
                    sq.x = LRBORDER_SIZE + j*CELL_SIZE , sq.y = TDBORDER_SIZE + i*CELL_SIZE, sq.h = CELL_SIZE, sq.w = CELL_SIZE;
                    SDL_RenderFillRect(rend, &sq);
                }
            }
        SDL_SetRenderDrawColor(rend, 10,10,10, SDL_ALPHA_OPAQUE);
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++){
                if(((i+j)&1)){
                    sq.x = LRBORDER_SIZE + j*CELL_SIZE, sq.y = TDBORDER_SIZE + i*CELL_SIZE, sq.h = CELL_SIZE, sq.w = CELL_SIZE;
                    SDL_RenderFillRect(rend, &sq);
                }
            }

        if(last_move_f != -1){
            SDL_SetRenderDrawColor(rend, 71, 137, 181, 255);
            pair<int, int> s = sq_cd(last_move_f);
            sq.x = LRBORDER_SIZE + s.cX*CELL_SIZE , sq.y = TDBORDER_SIZE + (7-s.cY)*CELL_SIZE, sq.h = CELL_SIZE, sq.w = CELL_SIZE;
            SDL_RenderDrawRect(rend, &sq);
            SDL_SetRenderDrawColor(rend, 5, 105, 156, 255);
            s = sq_cd(last_move_t);
            sq.x = LRBORDER_SIZE + s.cX*CELL_SIZE , sq.y = TDBORDER_SIZE + (7-s.cY)*CELL_SIZE, sq.h = CELL_SIZE, sq.w = CELL_SIZE;
            SDL_RenderDrawRect(rend, &sq);
        }

        if(draw_check){
            SDL_SetRenderDrawColor(rend, 148, 16, 43, 255);
            sq.x = LRBORDER_SIZE + check.second*CELL_SIZE , sq.y = TDBORDER_SIZE + (check.first)*CELL_SIZE, sq.h = CELL_SIZE, sq.w = CELL_SIZE;
            SDL_RenderDrawRect(rend, &sq);
        }


        this->draw_pieces(rend);

        sq = {100, CELL_SIZE*8 + TDBORDER_SIZE + TDBORDER_SIZE/4, 120, CELL_SIZE/2};
        SDL_RenderCopy(rend, human_text, NULL, &sq);
        sq = {100, TDBORDER_SIZE/4, 140, CELL_SIZE/2};
        SDL_RenderCopy(rend, bot_text, NULL, &sq);

        sq = {10, CELL_SIZE*8 + TDBORDER_SIZE + TDBORDER_SIZE/10, CELL_SIZE*3/4, CELL_SIZE*3/4};
        SDL_RenderCopy(rend, human_pic, NULL, &sq);
        sq = {10, TDBORDER_SIZE/10, CELL_SIZE*3/4, CELL_SIZE*3/4};
        SDL_RenderCopy(rend, bot_pic, NULL, &sq);

        if(white_win){
            sq = {CELL_SIZE*6, TDBORDER_SIZE/4, CELL_SIZE*2, CELL_SIZE/2};
            SDL_RenderCopy(rend, white_win_text, NULL, &sq);
        }

        if(black_win){
            sq = {CELL_SIZE*6, TDBORDER_SIZE/3, CELL_SIZE*2, CELL_SIZE/2};
            SDL_RenderCopy(rend, black_win_text, NULL, &sq);
        }

        if(draw){
            sq = {CELL_SIZE*6, TDBORDER_SIZE/4, 140, CELL_SIZE/2};
            SDL_RenderCopy(rend, draw_text, NULL, &sq);
        }

        if(rend_moves && !need_move_update) draw_moves(rend);

        
    }catch(exception &e){
        logger.error("Error rendering the board: " + string(e.what()));
    }catch(...){
        logger.error("Error ocurred while rendering the game board");
    }
}


void GameRenderer::renderer_loop(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger.error("error initializing SDL:" + string(SDL_GetError()));
    }
    TTF_Init();
    //Creates SDL window
    SDL_Window* win = SDL_CreateWindow("Chess Crusher",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       SCREEN_WIDTH, SCREEN_HEIGHT, 0);
 
    //Set up SDL stuff
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    //Creates SDL Renderer
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    //Initializing SDL audio mixer for audio handling
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        logger.error("An error ocurred while initializing audio mixer");
    }

    // Amount of channels (Max amount of sounds playing at the same time)
    Mix_AllocateChannels(8);

    bool lpressed = 0, rpressed = 0, mpressed = 0;
    // controls animation loop
    int close = 0;
    
    //Load resources (images & sound) required
    this->load_files(rend);
    this->set_values(rend);

    //Set window icon
    SDL_SetWindowIcon(win, window_icon);

    //Main loop
    logger.log("Main loop starting");
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if(turn == 0 && need_move_update == 0){
                    int mouseX = event.motion.x - LRBORDER_SIZE;
                    int mouseY = event.motion.y - TDBORDER_SIZE;

                    if(mouseX >= 0 && mouseX < CELL_SIZE*8 && mouseY >= 0 && mouseY < CELL_SIZE*8){
                        mouseY = 7-mouseY/CELL_SIZE;
                        mouseX /= CELL_SIZE;
                        int moveid = (mouseY<<3)+mouseX;
                        //logger.log("Mouse down at " + to_string(mouseX) + " " + to_string(mouseY) + " (" + to_string(moveid) + ")");
                        if(moves_set.find(moveid) != moves_set.end()){
                            logger.log("Player move selected");

                            current_move++;

                            last_move_f = move_to_draw;
                            last_move_t = moveid;

                            pair<int, int> m_from = sq_cd(move_to_draw);
                            pair<int, int> m_to = sq_cd(moveid);

                            int ant = board[7 - m_to.cY][m_to.cX];

                            board[7 - m_to.cY][m_to.cX] = board[7 - m_from.cY][m_from.cX];
                            color[7 - m_to.cY][m_to.cX] = color[7 - m_from.cY][m_from.cX];
                            board[7 - m_from.cY][m_from.cX] = 0;
                            color[7 - m_from.cY][m_from.cX] = 0;

                            draw_check = 0;

                            if(engine.in_check(2, board, color)){
                                audio_check.play();
                                set_draw_check(2);
                            }else if(ant != 0) audio_capture.play();
                            else audio_move.play();

                            update_notation(m_from, m_to, 1, ant!=0);

                            //Pawn auto promotes to queen
                            if(m_to.cY == 7 && board[7 - m_to.cY][m_to.cX] == 1) board[7 - m_to.cY][m_to.cX] = 5;

                            if(board[7 - m_to.cY][m_to.cX] == 4){
                                if(m_from.cX == 0) castlingwl = 0;
                                else if(m_from.cX == 7) castlingwr = 0;
                            }

                            //White castling (Right side)
                            if(board[7 - m_to.cY][m_to.cX] == 6 && castlingwr && m_to.cX - m_from.cX == 2){
                                logger.warning("Castling right side (white) " + to_string(m_to.cX));

                                castlingwl = 0;
                                castlingwr = 0;

                                board[7 - m_to.cY][m_to.cX - 1] = board[7 - m_to.cY][m_to.cX + 1];
                                color[7 - m_to.cY][m_to.cX - 1] = color[7 - m_to.cY][m_to.cX + 1];
                                board[7 - m_to.cY][m_to.cX + 1] = 0;
                                color[7 - m_to.cY][m_to.cX + 1] = 0;

                                game_notation += "O-O";
                            }
                            //White castling (Left side)
                            else if(board[7 - m_to.cY][m_to.cX] == 6 && castlingwl && m_from.cX - m_to.cX == 2){
                                logger.warning("Castling left side (white) " + to_string(m_to.cX));

                                castlingwl = 0;
                                castlingwr = 0;

                                board[7 - m_to.cY][m_to.cX + 1] = board[7 - m_to.cY][m_to.cX - 2];
                                color[7 - m_to.cY][m_to.cX + 1] = color[7 - m_to.cY][m_to.cX - 2];
                                board[7 - m_to.cY][m_to.cX - 2] = 0;
                                color[7 - m_to.cY][m_to.cX - 2] = 0;

                                game_notation += "O-O-O";
                            }else if(board[7 - m_to.cY][m_to.cX] == 6) castlingwl = 0, castlingwr = 0, game_notation += "K" + (ant!=0?string("x"):string("")) + string(1, 'a' + m_to.cX) + to_string(m_to.cY+1);

                            moves_set.clear();

                            logger.log(game_notation);

                            request_engine_move();
                            //request_move_update();
                        }else if(moves[moveid].size() && move_to_draw != moveid){
                            moves_set.clear();
                            for(auto move: moves[moveid])
                                moves_set.insert(move);
                                
                            rend_moves = 1, move_to_draw = moveid;
                        }else if(move_to_draw != moveid) rend_moves = 0, move_to_draw = -1, moves_set.clear();

                    }else rend_moves = 0, move_to_draw = -1, moves_set.clear();
                }
                break;
            }
        }

        if(need_move_update && !calculate_moves_flag){
            moves = set_of_moves;
            need_move_update = 0;
        }

        if(turn && !engine_move_flag && waiting_engine_move){
            pair<int, int> f, t;
            f = sq_cd(engine_move.first);
            t = sq_cd(engine_move.second);

            last_move_f = engine_move.from;
            last_move_t = engine_move.to;

            int ant = board[7 - t.first][t.second];

            board[7 - t.first][t.second] = board[7 - f.first][f.second];
            color[7 - t.first][t.second] = color[7 - f.first][f.second];
            board[7 - f.first][f.second] = 0;
            color[7 - f.first][f.second] = 0;

            update_notation(f, t, 2, ant!=0);

            if(engine.in_check(1, board, color)) audio_check.play();
            else if(ant!=0) audio_capture.play();
            else audio_move.play();

            if(t.first == 0 && board[7 - t.first][t.second] == 1) board[7 - t.first][t.second] = 5;

            pair<int, int> m_to = t, m_from = f;

            if(board[7 - m_to.cY][m_to.cX] == 4){
                if(m_from.cX == 0) castlingbl = 0;
                else if(m_from.cX == 7) castlingbr = 0;
            }

            //black castling (Right side)
            if(board[7 - m_to.cY][m_to.cX] == 6 && castlingbr && m_to.cX - m_from.cX == 2){
                logger.warning("Castling right side (black) " + to_string(m_to.cX));

                castlingbl = 0;
                castlingbr = 0;

                board[7 - m_to.cY][m_to.cX - 1] = board[7 - m_to.cY][m_to.cX + 1];
                color[7 - m_to.cY][m_to.cX - 1] = color[7 - m_to.cY][m_to.cX + 1];
                board[7 - m_to.cY][m_to.cX + 1] = 0;
                color[7 - m_to.cY][m_to.cX + 1] = 0;

                game_notation += "O-O";
            }
            //Black castling (Left side)
            else if(board[7 - m_to.cY][m_to.cX] == 6 && castlingbl && m_from.cX - m_to.cX == 2){
                logger.warning("Castling left side (black) " + to_string(m_to.cX));

                castlingbl = 0;
                castlingbr = 0;

                board[7 - m_to.cY][m_to.cX + 1] = board[7 - m_to.cY][m_to.cX - 2];
                color[7 - m_to.cY][m_to.cX + 1] = color[7 - m_to.cY][m_to.cX - 2];
                board[7 - m_to.cY][m_to.cX - 2] = 0;
                color[7 - m_to.cY][m_to.cX - 2] = 0;

                game_notation += "O-O-O";
            }else if(board[7 - m_to.cY][m_to.cX] == 6) castlingbl = 0, castlingbr = 0, game_notation += "K" + (ant!=0?string("x"):string("")) + string(1, 'a' + m_to.cX) + to_string(m_to.cY+1);

            draw_check = 0;

            if(engine.in_check(1, board, color)){
                audio_check.play();
                game_notation += "+";
                set_draw_check(1);
            }
            else if(ant!=0) audio_capture.play();
            else audio_move.play();

            logger.log(game_notation);

            turn = 0;
            waiting_engine_move = 0;
            request_move_update();
        }

        if(handle_white_win){
            handle_white_win = 0;
            rnd_e(audio_win).play();
            //Show some text
        }

        if(handle_black_win){
            handle_black_win = 0;
            rnd_e(audio_lose).play();
            //Show some text
        }

        if(handle_draw){
            handle_draw = 0;
            rnd_e(audio_draw).play();
            //Show some text
        }

        draw_board(rend);

        SDL_RenderPresent(rend);
 
        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
    
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    Mix_CloseAudio();
    	
    TTF_Quit();
    // close SDL
    SDL_Quit();
}

void GameRenderer::set_draw_check(int c){
    logger.log("Now drawing check");
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(board[i][j] == 6 && color[i][j] == c){
                draw_check = 1;
                check = {i, j};
                return;
            }
        }
    }
}

void GameRenderer::request_move_update(){
    move_to_draw = -1;
    rend_moves = 0;
    need_move_update = 1;

    for(int i=0; i<8; i++) for(int j=0; j<8; j++) boardtc[i][j] = board[i][j];
    for(int i=0; i<8; i++) for(int j=0; j<8; j++) colortc[i][j] = color[i][j];

    white_left_castle = castlingwl;
    white_right_castle = castlingwr;
    black_left_castle = castlingbl;
    black_right_castle = castlingbr; 

    //logger.log("Casling wl(" + to_string(castlingwl) + "), Castling wr(" + to_string(castlingwr) + ")");

    calculate_moves_flag = 1;
}

void GameRenderer::request_engine_move(){
    rend_moves = 0;
    move_to_draw = -1;
    
    for(int i=0; i<8; i++) for(int j=0; j<8; j++) boardtc[i][j] = board[i][j];
    for(int i=0; i<8; i++) for(int j=0; j<8; j++) colortc[i][j] = color[i][j];

    white_left_castle = castlingwl;
    white_right_castle = castlingwr;
    black_left_castle = castlingbl;
    black_right_castle = castlingbr;

    engine_move_flag = 1;
    waiting_engine_move = 1;
    turn = 1;

    move_count = current_move;
    notation = game_notation;
}

void GameRenderer::set_values(SDL_Renderer *rend){
    human_text = SDL_CreateTextureFromSurface(rend, TTF_RenderText_Solid(font, "Human", text_color));
    bot_text = SDL_CreateTextureFromSurface(rend, TTF_RenderText_Solid(font, "Computer", text_color));

    white_win_text = SDL_CreateTextureFromSurface(rend, TTF_RenderText_Solid(font, "Human wins!", {55, 204, 95}));
    black_win_text = SDL_CreateTextureFromSurface(rend, TTF_RenderText_Solid(font, "Computer has won!", {204, 55, 67}));
    draw_text = SDL_CreateTextureFromSurface(rend, TTF_RenderText_Solid(font, "Draw .-.", {116, 140, 143}));

    audio_move.set_file("Resources/Sound/game/move.wav");
    audio_capture.set_file("Resources/Sound/game/capture.wav");
    audio_check.set_file("Resources/Sound/game/check.wav");

    for(int i=1; i<=4; i++) audio_win.push_back(audio_sample("Resources/Sound/friendly_lines/win" + to_string(i) + ".wav"));
    for(int i=1; i<=3; i++) audio_lose.push_back(audio_sample("Resources/Sound/friendly_lines/lose" + to_string(i) + ".wav"));
    for(int i=1; i<=3; i++) audio_draw.push_back(audio_sample("Resources/Sound/friendly_lines/tie" + to_string(i) + ".wav"));

}

void GameRenderer::update_notation(pair<int, int> m_from, pair<int, int> m_to, int c, bool cap){
    //Notation managment
    
    if(c == 1) game_notation += (current_move>1?string(" "):string("")) + to_string(current_move) + ". ";
    else game_notation += " ";

    if(board[7 - m_to.cY][m_to.cX] == 1){ //Pawns
        if(!cap) game_notation += string(1, 'a' + m_to.cX) + to_string(m_to.cY+1);
        else game_notation += string(1, 'a' + m_from.cX) + "x" + string(1, 'a' + m_to.cX) + to_string(m_to.cY+1);

    }else if(board[7 - m_to.cY][m_to.cX] == 2){ //Knights
        game_notation += "N";
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if(color[i][j] == c && board[i][j] == 2 && (i!=7-m_to.cY || j!=m_to.cX)) if((absv((7 - m_to.cY) - i) == 1 && absv(m_to.cX - j) == 2) || (absv((7 - m_to.cY) - i) == 2 && absv(m_to.cX - j) == 1)){
                    game_notation += string(1, 'a' + m_from.cX);
                    if(i == m_from.cX) game_notation += to_string(7 - m_from.cY);
                }
            }
        }

        if(cap) game_notation += "x";

        game_notation += string(1, 'a' + m_to.cX) + to_string(m_to.cY+1);
    }else if(board[7 - m_to.cY][m_to.cX] == 3){ //Bishops :D
        game_notation += "B";
        if(cap) game_notation += "x";
        game_notation += string(1, 'a' + m_to.cX) + to_string(m_to.cY+1);
    }else if(board[7 - m_to.cY][m_to.cX] == 4){ //Rooks :c
        game_notation += "R";

        for(int i=0; i<7; i++){
            if(i != 7-m_to.cY && color[i][m_from.cX] == c && board[i][m_from.cX] == 4) game_notation += string(1, 'a' + m_from.cX) + to_string(7 - m_from.cY);
        }

        for(int j=0; j<7; j++){
            if(j != m_to.cX && color[7 - m_from.cY][j] == c && board[7 - m_from.cY][j] == 4){
                game_notation += string(1, 'a' + m_from.cX);
            }
        }

        if(cap) game_notation += "x";

        game_notation += string(1, 'a' + m_to.cX) + to_string(m_to.cY+1);

    }else if(board[7 - m_to.cY][m_to.cX] == 5){ //Queens D:
        game_notation += "Q";

        for(int i=0; i<7; i++){
            if(i != 7-m_to.cY && color[i][m_from.cX] == c && board[i][m_from.cX] == 5) game_notation += string(1, 'a' + m_to.cX) + to_string(7 - m_from.cY);
        }

        for(int j=0; j<7; j++){
            if(j != m_to.cX && color[7 - m_from.cY][j] == c && board[7 - m_from.cY][j] == 5) game_notation += string(1, 'a' + m_from.cX);
        }

        //Missing diagonals :p

        if(cap) game_notation += "x";

        game_notation += string(1, 'a' + m_to.cX) + to_string(m_to.cY+1);
    }else if(board[7 - m_to.cY][m_to.cX] == 6){//Lord Farquad (King)
        //Handle in castling part of things maybe? .-.
    }
}

void GameRenderer::load_files(SDL_Renderer *rend){
    logger.log("Loading renderer files...");
    try{
        //Loading Pieces Sprites
        sprites.clear();
        
        SDL_Surface * image;
        for(int i=1; i<=6; i++){
            image = IMG_Load(("Resources/" + to_string(i) + "_1.png").c_str());
            sprites.push_back(SDL_CreateTextureFromSurface(rend, image));
            image = IMG_Load(("Resources/" + to_string(i) + "_2.png").c_str());
            sprites.push_back(SDL_CreateTextureFromSurface(rend, image));
        }

        image = IMG_Load(BOT_PIC.c_str());
        bot_pic = SDL_CreateTextureFromSurface(rend, image);
        image = IMG_Load(HUMAN_PIC.c_str());
        human_pic = SDL_CreateTextureFromSurface(rend, image);

        image = IMG_Load("Resources/dot.png");
        sprites.push_back(SDL_CreateTextureFromSurface(rend, image));
        image = IMG_Load("Resources/dot2.png");
        sprites.push_back(SDL_CreateTextureFromSurface(rend, image));

        //Loading window icon
        window_icon = IMG_Load("Resources/icon.png");

        //font = TTF_OpenFont(".\\Resources\\Fonts\\Peralta-Regular.ttf",18);
        font = TTF_OpenFont(".\\Resources\\Fonts\\arial.ttf", 18);

        logger.success("Renderer files loaded");
    }catch(exception e){
        logger.error("Error while loading files: " + string(e.what()));
    }catch(...){
        logger.error("Error while loading files :(");
    }
}

void GameRenderer::copy_matrix(){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            boardtc[i][j] = board[i][j];
            colortc[i][j] = color[i][j];
        }
    }
}

void GameRenderer::init(){
    try{
        srand(time(NULL));

        copy_matrix();

        turn = 0;
        renderer_loop();
    }catch(exception e){
        logger.error("Renderer Crashed with error: " + string(e.what()));
    }
}

void* run_renderer(void* arg){
    GameRenderer grend;
    logger.log("Initializing Renderer thread");
    grend.init();
    logger.error("Renderer closed");
}

void handle_gstate(int s){
    if(s == 1){
        logger.success("White has won");
        handle_white_win = 1;
        white_win = 1;
    }
    else if(s == 2){
        logger.success("Black has won");
        handle_black_win = 1;
        black_win = 1;
    }
    else if(s==3){
        logger.success("Draw :o");
        handle_draw = 1;
        draw = 1;
    }else{
        logger.error("Something went wrong :c");
        return;
    }
    while(true);
}

int main(int argc, char *argv[]){
    logger.init();
    logger.log("Main function running");

    engine.init();

    pthread_t renderer_thread;
    int thread_response = 0;
    thread_response = pthread_create(&renderer_thread, NULL, &run_renderer, NULL);
    if(thread_response != 0) logger.error("Couldn't initialize the renderer thread");

    SDL_Delay(100);
    while(true){

        //Update moves for renderer
        if(calculate_moves_flag){
            int state =  engine.board_state(boardtc, colortc, 1);

            if(state != 0){
                handle_gstate(state);
            }else{
                logger.log("Calculating moves for renderer");
                set_of_moves = engine.getMoves(boardtc, colortc, white_left_castle, white_right_castle, black_left_castle, black_right_castle);
                calculate_moves_flag = 0;
            
                logger.log("Board eval (white): " + to_string(engine.get_array_eval(boardtc, colortc)));
            }
        }

        if(engine_move_flag){
            int state =  engine.board_state(boardtc, colortc, 2);

            if(state != 0){
                handle_gstate(state);
            }else{
                //Try to consult the opening book if we are in the first 5 moves of the game
                if(move_count <= 5){
                    pair<int, int> opening_move = engine.opening_lookup(boardtc, colortc, notation);
                    logger.log(to_string(opening_move.from) + ", " + to_string(opening_move.to));
                    if(opening_move.from != -1){
                        logger.log("Position found in opening book");

                        engine_move = opening_move;
                        engine_move_flag  = 0;
                        continue;
                    }
                }

                engine_move = engine.get_engine_move(boardtc, colortc, white_left_castle, white_right_castle, black_left_castle, black_right_castle);
                engine_move_flag = 0;
            }
        }

        SDL_Delay(1000/120);
    }

    return 0;
}