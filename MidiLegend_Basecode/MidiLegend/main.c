/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "Settings.h"
#include "Common.h"
#include "Timer.h"
#include "LevelScene.h"
#include "TitleScene.h"

int main(int argc, char *argv[])
{
    //--------------------------------------------------------------------------
    // Initialisation

    // Initialise la SDL
    Game_init(SDL_INIT_VIDEO, IMG_INIT_PNG);

    // Cr�e la fen�tre
    int sdlFlags = 0;
#ifdef FULLSCREEN
    sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
    SDL_Window *window = SDL_CreateWindow(
        u8"MidiLegend", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, sdlFlags
    );

    if (!window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Cr�e le moteur de rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false); abort();
    }

    //--------------------------------------------------------------------------
    // Boucle de jeu

    TitleScene *titleScene = NULL;
    LevelScene *levelScene = NULL;
    while (true)
    {
        //......................................................................
        // Menu principal

        titleScene = TitleScene_create(renderer);

        // Boucle de rendu du menu principal
        while (true)
        {
            // Met � jour le temps
            Timer_update(g_time);

            // Met � jour la sc�ne
            bool quitLoop = TitleScene_update(titleScene);
            if (quitLoop)
                break;

            // Efface le rendu pr�c�dent
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Dessine la sc�ne
            TitleScene_render(titleScene);

            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

        if (titleScene->input->quitPressed)
            break;

        LevelConfig config = *TitleScene_getLevelConfig(titleScene);

        // D�truit la sc�ne du menu principal
        TitleScene_destroy(titleScene);
        titleScene = NULL;

        //......................................................................
        // Niveau

        levelScene = LevelScene_create(
            renderer, config
        );

        // Boucle de rendu du niveau
        LevelScene_start(levelScene);
        while (true)
        {
            // Met � jour le temps
            Timer_update(g_time);

            // Met � jour la sc�ne
            bool quitLoop = LevelScene_update(levelScene);
            if (quitLoop)
                break;

            // Efface le rendu pr�c�dent
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Dessine la sc�ne
            LevelScene_render(levelScene);

            // Affiche le nouveau rendu
            SDL_RenderPresent(renderer);
        }

        if (levelScene->input->quitPressed)
            break;

        // D�truit la sc�ne du niveau
        LevelScene_destroy(levelScene);
        levelScene = NULL;
    }


    //--------------------------------------------------------------------------
    // Lib�ration de la m�moire

    TitleScene_destroy(titleScene);
    titleScene = NULL;

    LevelScene_destroy(levelScene);
    levelScene = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    Game_quit();

    return EXIT_SUCCESS;
}