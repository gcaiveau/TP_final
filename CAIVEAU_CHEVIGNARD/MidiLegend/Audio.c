#include "Settings.h"

/*
* Permet de mettre la musique de fond ainsi que les bruits pour les changements d'items sélectionné
*/
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