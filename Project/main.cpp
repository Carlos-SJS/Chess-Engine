#include "main.h"
#include "src/logger.h"

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

        this->draw_pieces(rend);

        sq = {10, CELL_SIZE*8 + TDBORDER_SIZE + CELL_SIZE/10, 120, CELL_SIZE/2};
        SDL_RenderCopy(rend, human_text, NULL, &sq);
        sq = {10, CELL_SIZE/10, 140, CELL_SIZE/2};
        SDL_RenderCopy(rend, bot_text, NULL, &sq);

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
        //if(turn == 0 && need_move_update) moves = engine.getMoves(board, color), need_move_update = 0;
 
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
                        logger.log("Mouse down at " + to_string(mouseX) + " " + to_string(mouseY));

                        if(moves[(mouseY<<3)+mouseX].size()) rend_moves = 1, move_to_draw = (mouseY<<3)+mouseX;
                        else rend_moves = 0;

                        /*for(int i=0; i<64; i++){
                            cout << i << ": ";
                            for(int j=0; j<moves[i].size(); j++){
                                cout << moves[i][j] << " ";
                            }
                            cout << '\n';
                        }*/

                        //Check if the click is in a move is also needed

                    }else rend_moves = 0;
                }
                break;
            }
        }

        if(need_move_update && !calculate_moves_flag){
            moves = set_of_moves;
            need_move_update = 0;
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
     
    
    	
    TTF_Quit();
    // close SDL
    SDL_Quit();
}

void GameRenderer::set_values(SDL_Renderer *rend){
    /* Stuff to get username
    char username[1024];
    DWORD username_len = 1024;
    GetUserNameA(username,&username_len);
    

    int length = mbstowcs(nullptr, username, 0);

    // Allocate a temporary string
    wchar_t* tmpstr = new wchar_t[length + 1];

    // Do the actual conversion
    mbstowcs(tmpstr, username, length + 1);

    wstring w_username = tmpstr;


    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    string user_name = converter.to_bytes(w_username);
    */

    human_text = SDL_CreateTextureFromSurface(rend, TTF_RenderText_Solid(font, "Human", text_color));
    bot_text = SDL_CreateTextureFromSurface(rend, TTF_RenderText_Solid(font, "Computer", text_color));
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

int main(int argc, char *argv[]){
    logger.init();
    logger.log("Main function running");

    engine.init();

    pthread_t renderer_thread;
    int thread_response = 0;
    thread_response = pthread_create(&renderer_thread, NULL, &run_renderer, NULL);
    if(thread_response != 0) logger.error("Couldn't initialize the renderer thread");

    /*GameRenderer grend;
    logger.log("Initializing Renderer thread");
    grend.init();
    logger.error("Renderer closed");*/
    SDL_Delay(100);
    while(true){

        //Update moves for renderer
        if(calculate_moves_flag){
            logger.log("Calculating moves for renderer");
            set_of_moves = engine.getMoves(boardtc, colortc);
            calculate_moves_flag = 0;
        }

        SDL_Delay(1000/120);
    }

    return 0;
}