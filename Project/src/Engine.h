#include "Generator.h"

class Engine{
    private:
        
    public:
        Engine();
        void init();
        vector<vector<int>> getMoves(int cboard[8][8], int color[8][8]);
};