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
    self->textSelectNotes = Text_create(renderer, assets->fonts.normal, u8"Nombres de Notes", assets->colors.bleu_clair);
    self->textNbNotes = Text_create(renderer, assets->fonts.normal, u8"Notes", assets->colors.white);
    self->textSelectDifficulty = Text_create(renderer, assets->fonts.normal, u8"Niveau de difficulté", assets->colors.bleu_clair);
    self->textDifficultyValue = Text_create(renderer, assets->fonts.normal, u8"Difficulté", assets->colors.white);

    return self;
}

void TitleCanvas_destroy(TitleCanvas *self)
{
    if (!self) return;

    // libérer l'espace du texte
    Text_destroy(self->textMusic);  
    Text_destroy(self->textSelectMusic);
    Text_destroy(self->textStart);
    Text_destroy(self->textSelectNotes);
    Text_destroy(self->textNbNotes);
    Text_destroy(self->textSelectDifficulty);
    Text_destroy(self->textDifficultyValue);

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

    //Nombre de Notes
    texture = Text_getTexture(self->textNbNotes);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textNbNotes.x;
    dst.y = g_titleRects.textNbNotes.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // selection nombre de Notes
    texture = Text_getTexture(self->textSelectNotes);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textSelectNotes.x;
    dst.y = g_titleRects.textSelectNotes.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // selection niveau de difficulté
    texture = Text_getTexture(self->textSelectDifficulty);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textSelectDifficulty.x;
    dst.y = g_titleRects.textSelectDifficulty.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // niveau de difficulté
    texture = Text_getTexture(self->textDifficultyValue);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textDifficultyValue.x;
    dst.y = g_titleRects.textDifficultyValue.y;
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
        idx = Int_clamp(idx, 0, 3);

        self->selection = idx;
    }

    if (input->leftPressed || input->rightPressed)
    {
        if (self->selection == 0)                       //choix musique
        {
            int idx = config->musicID;
            idx += (input->rightPressed) ? 1 : -1;
            idx = (idx + MUSIC_COUNT) % MUSIC_COUNT;
            config->musicID = idx;
        }
        if (self->selection == 1)                       //choix nombre de notes
        {
            int idx = config->keyCount;
            idx += (input->rightPressed) ? 1 : -1;
            idx = 3 + (idx) % 3;
            config->keyCount = idx;
        }

        if (self->selection == 2)                       //choix difficulté
        {
            int idx = config->leveldifficulty.difficultyLevel;
            idx += (input->rightPressed) ? 1 : -1;
            idx = 1+((idx+2)%3);
            config->leveldifficulty.difficultyLevel = idx;
        }
    }
    Text_setString(self->textMusic, g_musics[config->musicID].titleName);   // mise a jour du texte en fonction des action sutilisateurs
    char nbnotes[6];
    char difficulty[16];
    sprintf(nbnotes, u8"< %d >", config->keyCount);
    sprintf(difficulty, u8"< %d >", config->leveldifficulty.difficultyLevel);
    Text_setString(self->textNbNotes,nbnotes);// mise a jour du texte en fonction des action sutilisateurs
    Text_setString(self->textDifficultyValue, difficulty);// mise a jour du texte en fonction des action sutilisateurs

    Text *leftTexts[] = {
        self->textSelectMusic,
        self->textSelectNotes,
        self->textSelectDifficulty,
        self->textStart
    };
    for (int i = 0; i < 4; i++)
    {
        SDL_Color colors = (i == self->selection) ?
            assets->colors.marron : assets->colors.bleu_clair;
        Text_setColor(leftTexts[i], colors);
    }

    return (self->selection == 3 && input->startPressed);
}
