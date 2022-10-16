#include "AudioSample.h"
#include "logger.h" 

audio_sample::audio_sample(const string path){
    chunk = Mix_LoadWAV(path.c_str());
}

// -1 here means we let SDL_mixer pick the first channel that is free
// If no channel is free it'll return an err code.
void audio_sample::play() {
    logger.log("Playing sound...");
    Mix_PlayChannel(-1, chunk, 0);
}

void audio_sample::set_volume(int volume) {
    Mix_VolumeChunk(chunk, volume);
}

void audio_sample::set_file(const string path){
    chunk = Mix_LoadWAV(path.c_str());
}