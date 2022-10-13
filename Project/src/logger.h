#include<iostream>
#include<time.h>
#include<windows.h>
using namespace std;

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008

class Logger{
    public:
        //Logger();
        static void log(string);
        static void error(exception);
        static void error(string);
        static void warning(string);
        static void success(string);
        static string get_time();
        static void init();
};

extern Logger logger;