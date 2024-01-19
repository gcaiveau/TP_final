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

    if (game_statue == true)
        self->config.pageID = 2;

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
