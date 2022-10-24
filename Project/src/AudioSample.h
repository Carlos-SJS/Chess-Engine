/*
    Based on aram's answer at 
    https://stackoverflow.com/questions/50240497/sdl-how-to-play-audio-asynchronously-in-c-without-stopping-code-execution
*/

#pragma once
#include <memory>
#include <string>
#include <SDL_mixer.h>

using namespace std;

class audio_sample{
    private:
        Mix_Chunk *chunk;
        string file;
    public:
        audio_sample(){}
        audio_sample(const string path);
        void set_file(const string path);
        void play();
        void set_volume(int volume);
};

