/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "TitleScene.h"

TitleScene *TitleScene_create(SDL_Renderer *renderer, bool game_statue)
{
    TitleScene *self = (TitleScene *)calloc(1, sizeof(TitleScene));
    AssertNew(self);
<<<<<<< Updated upstream
    //le boleen du main nous permet de savoir si l'utilasateur à jouer ou non et ainsi lui afficher une page de fin 
    // (obligatoire à cause de la remise à zéro de la variable pageID a la fin du jeu)
=======
    
>>>>>>> Stashed changes
    if (game_statue == true)
    {
        self->config.pageID = 2;
    }

    self->renderer = renderer;

    self->assets = AssetManager_create(renderer);
    self->input = Input_create(true);
    self->canvas = TitleCanvas_create(self, game_statue);


    self->config.keyCount = 3;
    self->config.leveldifficulty.difficultyLevel = 1;
    self->config.bindselected = 0;
    self->config.musicID = 0;
    self->config.piano = true;


    return self;
}

void TitleScene_destroy(TitleScene *self)
{
    //détruit tout les éléments de la scène pour en afficher de nouveau, détruit également le canva.C
    if (!self) return;

    Mix_HaltChannel(0);

    AssetManager_destroy(self->assets);
    TitleCanvas_destroy(self->canvas);
    Input_destroy(self->input);
    Text_destroy(self->combo);

    free(self);
}

bool TitleScene_update(TitleScene *self)
{
    TitleCanvas *canvas = self->canvas;
    Input *input = self->input;

    // Met à jour les entrées utilisateur
    Input_update(input);
    bool quit = TitleCanvas_update(canvas);

    return quit || self->input->quitPressed;
}

void TitleScene_render(TitleScene *self)
{
    SDL_Renderer *renderer = TitleScene_getRenderer(self);
    AssetManager *assets = TitleScene_getAssetManager(self);

    // Affichage du fond
    SDL_RenderCopy(renderer, assets->textures.background, NULL, NULL);


    //en fonction de niveau de difficulté affichage des notes
    if (self->config.leveldifficulty.difficultyLevel == 1 && self->config.pageID==1)
    {
        SDL_RenderCopy(renderer, assets->textures.difficulte1, NULL, &(g_titleRects.difficulte1));
        SDL_RenderCopy(renderer, assets->textures.difficulte, NULL, &(g_titleRects.difficulte2));
        SDL_RenderCopy(renderer, assets->textures.difficulte, NULL, &(g_titleRects.difficulte3));
    }
    else if (self->config.leveldifficulty.difficultyLevel == 2 && self->config.pageID == 1)
    {
        SDL_RenderCopy(renderer, assets->textures.difficulte1, NULL, &(g_titleRects.difficulte1));
        SDL_RenderCopy(renderer, assets->textures.difficulte1, NULL, &(g_titleRects.difficulte2));
        SDL_RenderCopy(renderer, assets->textures.difficulte, NULL, &(g_titleRects.difficulte3));
    }
    else if (self->config.leveldifficulty.difficultyLevel == 3 && self->config.pageID == 1)
    {
        SDL_RenderCopy(renderer, assets->textures.difficulte1, NULL, &(g_titleRects.difficulte1));
        SDL_RenderCopy(renderer, assets->textures.difficulte1, NULL, &(g_titleRects.difficulte2));
        SDL_RenderCopy(renderer, assets->textures.difficulte1, NULL, &(g_titleRects.difficulte3));
    }

    // Affichage du canvas
    TitleCanvas_render(self->canvas);
}
