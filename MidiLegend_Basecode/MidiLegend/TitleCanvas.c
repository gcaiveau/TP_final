/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "TitleCanvas.h"
#include "TitleScene.h"
#include "Math.h"

TitleCanvas *TitleCanvas_create(TitleScene *scene)
{
    TitleCanvas *self = (TitleCanvas *)calloc(1, sizeof(TitleCanvas));
    AssertNew(self);

    AssetManager *assets = TitleScene_getAssetManager(scene);
    SDL_Renderer *renderer = TitleScene_getRenderer(scene);

    self->scene = scene;
    
    //initialise un espace emoire pour le texte (render, police, "texte", couleur)
    self->textSelectMusic = Text_create(renderer, assets->fonts.normal, u8"Musique", assets->colors.bleu_clair);
    self->textMusic = Text_create(renderer, assets->fonts.normal, u8"Nom de la musique", assets->colors.white);
    self->textStart = Text_create(renderer, assets->fonts.normal, u8"Commencer", assets->colors.bleu_clair);
    self->textNotes = Text_create(renderer, assets->fonts.normal, u8"Nombres de Notes", assets->colors.bleu_clair);
    self->textSelectNotes = Text_create(renderer, assets->fonts.normal, u8"Notes", assets->colors.white);

    return self;
}

void TitleCanvas_destroy(TitleCanvas *self)
{
    if (!self) return;

    // libérer l'espace du texte
    Text_destroy(self->textMusic);  
    Text_destroy(self->textSelectMusic);
    Text_destroy(self->textStart);
    Text_destroy(self->textNotes);

    free(self);
}

void TitleCanvas_render(TitleCanvas *self)
{
    TitleScene *scene = self->scene;
    SDL_Renderer *renderer = TitleScene_getRenderer(scene);

    int w, h;
    SDL_Rect dst = { 0 };
    SDL_Texture *texture = NULL;

    // Choix de la musique
    texture = Text_getTexture(self->textSelectMusic);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textSelectMusic.x;
    dst.y = g_titleRects.textSelectMusic.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // récuépérer le texte associé et l'afficher
    texture = Text_getTexture(self->textMusic);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textMusic.x;
    dst.y = g_titleRects.textMusic.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // Notes
    texture = Text_getTexture(self->textNotes);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textNotes.x;
    dst.y = g_titleRects.textNotes.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // INotes
    texture = Text_getTexture(self->textSelectNotes);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textSelectNotes.x;
    dst.y = g_titleRects.textSelectNotes.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    

    // Commencer
    texture = Text_getTexture(self->textStart);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textStart.x;
    dst.y = g_titleRects.textStart.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    /* DEBUG
    // Gizmos du canvas en jaune
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &(g_titleRects.textSelectMusic));
    SDL_RenderDrawRect(renderer, &(g_titleRects.textStart));
    SDL_RenderDrawRect(renderer, &(g_titleRects.textMusic));
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &(g_titleRects.textNotes));
    SDL_RenderDrawRect(renderer, &(g_titleRects.textSelectNotes));
    */
}

bool TitleCanvas_update(TitleCanvas *self)
{
    TitleScene *scene = self->scene;
    AssetManager *assets = TitleScene_getAssetManager(scene);
    Input *input = TitleScene_getInput(scene);
    LevelConfig *config = TitleScene_getLevelConfig(scene);

    if (input->downPressed || input->upPressed)
    {
        int idx = self->selection;
        idx += (input->downPressed) ? 1 : -1;
        idx = Int_clamp(idx, 0, 2);

        self->selection = idx;
    }

    if (input->leftPressed || input->rightPressed)
    {
        if (self->selection == 0)
        {
            int idx = config->musicID;
            idx += (input->rightPressed) ? 1 : -1;
            idx = (idx + MUSIC_COUNT) % MUSIC_COUNT;
            config->musicID = idx;
        }
        if (self->selection == 1)
        {
            int idx = config->keyCount;
            idx += (input->rightPressed) ? 1 : -1;
            idx = 3 + (idx) % 3;
            config->keyCount = idx;
        }
    }
    Text_setString(self->textMusic, g_musics[config->musicID].titleName);   // mise a jour du texte en fonction des action sutilisateurs
    char tmp[6];
    sprintf(tmp, u8"< %d >", config->keyCount);
    Text_setString(self->textSelectNotes,tmp);// mise a jour du texte en fonction des action sutilisateurs

    Text *leftTexts[] = {
        self->textSelectMusic,
        self->textNotes,
        self->textStart
    };
    for (int i = 0; i < 3; i++)
    {
        SDL_Color colors = (i == self->selection) ?
            assets->colors.marron : assets->colors.bleu_clair;
        Text_setColor(leftTexts[i], colors);
    }

    return (self->selection == 2 && input->startPressed);
}
