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