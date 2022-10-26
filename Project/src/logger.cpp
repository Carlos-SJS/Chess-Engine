#include "logger.h"

//Implementig Logger Methodes
//Get time methode got from https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
string Logger::get_time(){
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "(%d-%m) %X:%S", &tstruct);
    return buf;
}

/*Logger::Logger(){
    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(handleOut, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
    SetConsoleMode(handleOut, consoleMode);
}*/

void Logger::log(string lg){
    cout << "\x1b[90m" << get_time() << " \x1b[0m:\x1b[46m LOG \x1b[0m: " << lg << '\n';
}

void Logger::error(exception e){
    cout << "\x1b[90m" << get_time() << " \x1b[0m:\x1b[41m ERROR \x1b[0m: \x1b[91m" << e.what() << "\x1b[0m\n";
}

void Logger::error(string lg){
    cout << "\x1b[90m" << get_time() << " \x1b[0m:\x1b[41m ERROR \x1b[0m: \x1b[91m" << lg << "\x1b[0m\n";
}

void Logger::warning(string lg){
    cout << "\x1b[90m" << get_time() << " \x1b[0m:\x1b[103m WARNING \x1b[0m: \x1b[93m" << lg << "\x1b[0m\n";
}

void Logger::success(string lg){
    cout << "\x1b[90m" << get_time() << " \x1b[0m:\x1b[102m SUCCESS \x1b[0m: \x1b[92m" << lg << "\x1b[0m\n";
}

void Logger::init(){
    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(handleOut, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
    SetConsoleMode(handleOut, consoleMode);
}

void Logger::printbboard(bitboard b){
    cout << "\x1b[90m" << get_time() << " \x1b[0m:\x1b[46m LOG (BitBoard) \x1b[0m: " << b << "\n";
    string board[8];
    for(int i=0; i<8; i++){
        board[i] = "";
        for(int j=0; j<8; j++){
            board[i] += (b&1ULL)?"1":".";
            b=(b>>1);
        }
    }
    for(int i=7; i>=0; i--) cout << "\t\t\t" << board[i] << "\n";
}

void Logger::printboard(board b){
    vector<char> w(64, '.');
    vector<char> n(64,'.');

    for(int i = 0; i<b.w_pieces[0].size(); i++) w[bb_sq(b.w_pieces[0][i])] = 'P';
    for(int i = 0; i<b.w_pieces[1].size(); i++) w[bb_sq(b.w_pieces[1][i])] = 'N';
    for(int i = 0; i<b.w_pieces[2].size(); i++) w[bb_sq(b.w_pieces[2][i])] = 'B';
    for(int i = 0; i<b.w_pieces[3].size(); i++) w[bb_sq(b.w_pieces[3][i])] = 'R';
    for(int i = 0; i<b.w_pieces[4].size(); i++) w[bb_sq(b.w_pieces[4][i])] = 'Q';
    for(int i = 0; i<b.w_pieces[5].size(); i++) w[bb_sq(b.w_pieces[5][i])] = 'K';

    for(int i = 0; i<b.b_pieces[0].size(); i++) n[bb_sq(b.b_pieces[0][i])] = 'P';
    for(int i = 0; i<b.b_pieces[1].size(); i++) n[bb_sq(b.b_pieces[1][i])] = 'N';
    for(int i = 0; i<b.b_pieces[2].size(); i++) n[bb_sq(b.b_pieces[2][i])] = 'B';
    for(int i = 0; i<b.b_pieces[3].size(); i++) n[bb_sq(b.b_pieces[3][i])] = 'R';
    for(int i = 0; i<b.b_pieces[4].size(); i++) n[bb_sq(b.b_pieces[4][i])] = 'Q';
    for(int i = 0; i<b.b_pieces[5].size(); i++) n[bb_sq(b.b_pieces[5][i])] = 'K';

    cout << "\x1b[90m" << get_time() << " \x1b[0m:\x1b[100m LOG (Board) \x1b[0m:\n";
    for(int i=7; i>=0; i--){
        cout << "\t\t\t";
        for(int j=0; j<8; j++){
            if(w[i*8 + j] != '.' && n[i*8 + j] != '.') cout << "\x1b[31me\e[0m";
            else if(w[i*8 + j] != '.') cout << "\x1b[97m" << w[i*8 + j] << "\e[0m";
            else if(n[i*8 + j] != '.') cout << "\x1b[90m" << n[i*8 + j] << "\e[0m";
            else cout << "\x1b[90m.\x1b[0m";
        }
        cout << '\n';
    }
}