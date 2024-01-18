/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "Common.h"
#include "AssetManager.h"
#include "TitleScene.h"
#include "LevelScene.h"
#include "LevelCanvas.h"
#include "Math.h"

Timer *g_time = NULL;
TitleRects g_titleRects = { 0 };
LevelRects g_levelRects = { 0 };
MusicSpec g_musics[MUSIC_COUNT] = { 0 }; 

void Game_initRectangles()
{
    int textHeight = 20; // TODO : Adapter la hauteur de texte
    int y;

    //--------------------------------------------------------------------------
    // Settings

    int xMid = LOGICAL_WIDTH / 2;

    SDL_Rect_set(&(g_titleRects.textTitre), (1280/2)-250, 20, 400, textHeight + 40);  //rectangle choix difficulté

    y = LOGICAL_HEIGHT/6;
    //SDL_Rect_set(,position en x largeur, y hauteur , largeur du rectangle, hauteur du rectangle en longueur);
    SDL_Rect_set(&(g_titleRects.textSelectMusic), 20, y, 400, textHeight+40);   // rectangle musique
    y += 3*textHeight;
    SDL_Rect_set(&(g_titleRects.textMusic), 120, y, 350, textHeight+40);  //rectangle choix musique

    y += 80;
    SDL_Rect_set(&(g_titleRects.textSelectNotes), 20, y, 400, textHeight + 40);  //rectangle choix notes
    y += 3*textHeight;
    SDL_Rect_set(&(g_titleRects.textNbNotes), 120, y, 350, textHeight + 40);  //rectangle nombre notes

    y +=80;
    SDL_Rect_set(&(g_titleRects.textSelectDifficulty), 20, y, 400, textHeight + 40);  //rectangle choix difficulté
    y += 3*textHeight;
    SDL_Rect_set(&(g_titleRects.difficulte1), 120, y - 10, 60, 60);  //rectangle difficulté
    SDL_Rect_set(&(g_titleRects.difficulte2), 200, y - 10, 60, 60);  //rectangle difficulté
    SDL_Rect_set(&(g_titleRects.difficulte3), 280, y - 10, 60, 60);  //rectangle difficulté

    y += 80;
    SDL_Rect_set(&(g_titleRects.textSelectBinding), 20, y, 400, textHeight + 40);  //rectangle choix touches jouables
    y += 3 * textHeight;
    SDL_Rect_set(&(g_titleRects.textBinding1), 40, y - 10, 60, 60);  //rectangle difficulté
    SDL_Rect_set(&(g_titleRects.textBinding2), 120, y - 10, 60, 60);  //rectangle difficulté
    SDL_Rect_set(&(g_titleRects.textBinding3), 200, y - 10, 60, 60);  //rectangle difficulté
    SDL_Rect_set(&(g_titleRects.textBinding4), 280, y - 10, 60, 60);  //rectangle difficulté
    SDL_Rect_set(&(g_titleRects.textBinding5), 360, y - 10, 60, 60);  //rectangle difficulté

    y += 80;
    SDL_Rect_set(&(g_titleRects.textMenu), 1280 - 250, 720 - 3*textHeight - 40, 400, textHeight + 40);;  //rectangle choix difficulté
   


    y += 100;
    SDL_Rect_set(&(g_titleRects.textStart1), 1280-250, 720- textHeight - 40, 400, textHeight + 40);  // rectangle commencer

    //--------------------------------------------------------------------------
    // Menu
    y = 100 + LOGICAL_HEIGHT / 4;
    SDL_Rect_set(&(g_titleRects.textSettings), 50, y, 400, textHeight + 40);

    y += 3* textHeight;
    SDL_Rect_set(&(g_titleRects.textStart), 50, y, 400, textHeight+40);  // rectangle commencer

    y += 60;
    SDL_Rect_set(&(g_titleRects.textQuit), 50, y, 400, textHeight + 40);  // rectangle commencer


    //--------------------------------------------------------------------------
    // Niveau

    int trackWidth = 580;     // largeur de la descente des notes
    int trackHeight = 500;    // hauteur de la descnete des notes
    int xTrack = 50;

    //SDL_Rect_set(&(g_levelRects.logo), 0, 0, 360, 211);  // logo
    SDL_Rect_set(&(g_levelRects.trackFill), xMid-290, 0, trackWidth, 720);  // decente de notes avec bloc de récupération
    SDL_Rect_set(&(g_levelRects.trackMask), xMid-290, 0, trackWidth, 100);  // bloc sombre descente des notes

    int barH = 10;//hauteur de la progress bar
    int uiWidth = 300;
    int xUI = LOGICAL_WIDTH - uiWidth - 30;

    y = 0;
    SDL_Rect_set(&(g_levelRects.uiRect), 1280-350, y, 350, 200);  // box score 

    SDL_Rect_set(&(g_levelRects.pointsA), xUI - 140, y + 30, trackWidth, trackHeight);

    y += 67;
    SDL_Rect_set(&(g_levelRects.points), xUI-140, y, trackWidth,trackHeight);  // les points
    SDL_Rect_set(&(g_levelRects.textRecord), 0, y, trackWidth, trackHeight);  // les points
    SDL_Rect_set(&(g_levelRects.recordBar), 25, y+10, uiWidth, barH);  // progress bas

    y += textHeight + 67;
    SDL_Rect_set(&(g_levelRects.progressBar), xUI, y, uiWidth, barH);  // progress bas

    y += 45;
    SDL_Rect_set(&(g_levelRects.comboRect), 1280 - 350, y, 350, 200);  // box score

    SDL_Rect_set(&(g_levelRects.comboA), xUI - 140, y+30, trackWidth, trackHeight);  // combo
    y += 67;
    SDL_Rect_set(&(g_levelRects.combo), xUI - 140, y,trackWidth, trackHeight);  // combo

    y += 67;
    SDL_Rect_set(&(g_levelRects.progressBarCombo), xUI, y, uiWidth, barH);  // progress bas

    SDL_Rect_set(&(g_levelRects.textPerfect), xUI - 140, 720-70, trackWidth, trackHeight);  // perfect

}

void Game_init(int sdlFlags, int imgFlags)
{
    // Initialise la SDL2
    if (SDL_Init(sdlFlags) < 0)
    {
        printf("ERROR - SDL_Init %s\n", SDL_GetError());
        assert(false);
        abort();
    }

    // Initialise la SDL2 image
    if (IMG_Init(imgFlags) != imgFlags)
    {
        printf("ERROR - IMG_Init %s\n", IMG_GetError());
        assert(false);
        abort();
    }

    // Initialise la SDL2 TTF
    if (TTF_Init() == -1)
    {
        printf("ERROR - TTF_Init %s\n", TTF_GetError());
        assert(false);
        abort();
    }

    // Initialise la SDL2 Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        printf("ERROR - Mix_OpenAudio %s\n", Mix_GetError());
        assert(false);
        abort();
    }
    Mix_AllocateChannels(8);

    // Initialise les rectangles fixes du jeu utilisés pour le rendu
    Game_initRectangles();

    int musicID = 0;
    g_musics[musicID].textPath = "../Assets/Musics/IslandVillage.txt";
    g_musics[musicID].musicPath = "../Assets/Musics/IslandVillage.mp3";
    g_musics[musicID].titleName = "< Island Village >";
    g_musics[musicID].fullName = "Island Village";
    g_musics[musicID].composer = "Heatley Bros - Heatley Bros V";

    musicID++;
    g_musics[musicID].textPath = "../Assets/Musics/NewChallenger.txt";
    g_musics[musicID].musicPath = "../Assets/Musics/NewChallenger.mp3";
    g_musics[musicID].titleName = "< New Challenger >";
    g_musics[musicID].fullName = "New Challenger";
    g_musics[musicID].composer = "Heatley Bros - Heatley Bros V";

    musicID++;
    g_musics[musicID].textPath = "../Assets/Musics/FightOrFlight.txt";
    g_musics[musicID].musicPath = "../Assets/Musics/FightOrFlight.mp3";
    g_musics[musicID].titleName = "< Fight Or Flight >";
    g_musics[musicID].fullName = "Fight Or Flight";
    g_musics[musicID].composer = "Heatley Bros - Heatley Bros VI";

    // Crée le temps global du jeu
    g_time = Timer_create();
    AssertNew(g_time);
}

void Game_quit()
{
    Timer_destroy(g_time);
    g_time = NULL;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
