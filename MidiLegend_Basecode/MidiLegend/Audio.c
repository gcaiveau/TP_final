#include "Settings.h"

void playMainAudio()
{
    Mix_Music* music = Mix_LoadMUS("../Assets/Musics/MusiqueFond.mp3");
    Mix_PlayMusic(music, -1);
}

void playSwitchSound()
{
    Mix_Chunk* music = Mix_LoadWAV("../Assets/Musics/switch.wav");
    Mix_PlayChannel(-1, music, 0);
}