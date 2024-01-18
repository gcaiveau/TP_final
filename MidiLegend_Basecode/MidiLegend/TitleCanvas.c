/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "TitleCanvas.h"
#include "TitleScene.h"
#include "Math.h"
#include "Audio.h"

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
    self->textStart1 = Text_create(renderer, assets->fonts.normal, u8"Commencer", assets->colors.bleu_clair);
    self->textSelectNotes = Text_create(renderer, assets->fonts.normal, u8"Nombres de Notes", assets->colors.bleu_clair);
    self->textNbNotes = Text_create(renderer, assets->fonts.normal, u8"Notes", assets->colors.white);
    self->textSelectDifficulty = Text_create(renderer, assets->fonts.normal, u8"Niveau de difficulté", assets->colors.bleu_clair);
    self->textSettings = Text_create(renderer, assets->fonts.normal, u8"Settings", assets->colors.bleu_clair);
    self->textMenu = Text_create(renderer, assets->fonts.normal, u8"Menu", assets->colors.bleu_clair);
    self->textTitre = Text_create(renderer, assets->fonts.big, u8"Midi Legend", assets->colors.bleu_clair);
    self->textQuit = Text_create(renderer, assets->fonts.normal, u8"Quitter", assets->colors.bleu_clair);
    self->textSelectBinding = Text_create(renderer, assets->fonts.normal, u8"Sélection des touches", assets->colors.bleu_clair);
    self->textBinding1 = Text_create(renderer, assets->fonts.normal, u8"1", assets->colors.white);
    self->textBinding2 = Text_create(renderer, assets->fonts.normal, u8"2", assets->colors.white);
    self->textBinding3 = Text_create(renderer, assets->fonts.normal, u8"3", assets->colors.white);
    self->textBinding4 = Text_create(renderer, assets->fonts.normal, u8"4", assets->colors.white);
    self->textBinding5 = Text_create(renderer, assets->fonts.normal, u8"5", assets->colors.white);
    playMainAudio();
    

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
    Text_destroy(self->textSettings);
    Text_destroy(self->textMenu);
    Text_destroy(self->textTitre);
    Text_destroy(self->textStart1);
    Text_destroy(self->textQuit);
    Text_destroy(self->textBinding1);
    Text_destroy(self->textBinding2);
    Text_destroy(self->textBinding3);
    Text_destroy(self->textBinding4);
    Text_destroy(self->textBinding5);
    Text_destroy(self->textSelectBinding);

    Mix_HaltMusic();
    free(self);
}
void TitleCanvas_render(TitleCanvas* self)
{
    if (self->pageID == 0)
        TitleCanvas_renderMain(self);
    else if (self->pageID==1)
        TitleCanvas_renderSettings(self);

}
void TitleCanvas_renderMain(TitleCanvas *self)
{
    TitleScene *scene = self->scene;
    SDL_Renderer *renderer = TitleScene_getRenderer(scene);

    int w, h;
    SDL_Rect dst = { 0 };
    SDL_Texture *texture = NULL;

    // Commencer
    texture = Text_getTexture(self->textStart);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textStart.x;
    dst.y = g_titleRects.textStart.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textSettings);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textSettings.x;
    dst.y = g_titleRects.textSettings.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textTitre);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textTitre.x;
    dst.y = g_titleRects.textTitre.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textQuit);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textQuit.x;
    dst.y = g_titleRects.textQuit.y;
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
void TitleCanvas_renderSettings(TitleCanvas* self)
{
    TitleScene* scene = self->scene;
    SDL_Renderer* renderer = TitleScene_getRenderer(scene);
    LevelConfig* config = TitleScene_getLevelConfig(scene);

    int w, h;
    SDL_Rect dst = { 0 };
    SDL_Texture* texture = NULL;

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

    texture = Text_getTexture(self->textMenu);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textMenu.x;
    dst.y = g_titleRects.textMenu.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textTitre);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textTitre.x;
    dst.y = g_titleRects.textTitre.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // Sélection des touches
    texture = Text_getTexture(self->textSelectBinding);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textSelectBinding.x;
    dst.y = g_titleRects.textSelectBinding.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // Sélection des touches

    
        texture = Text_getTexture(self->textBinding1);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dst.x = g_titleRects.textBinding1.x;
        dst.y = g_titleRects.textBinding1.y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        // Sélection des touches
        texture = Text_getTexture(self->textBinding2);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dst.x = g_titleRects.textBinding2.x;
        dst.y = g_titleRects.textBinding2.y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(renderer, texture, NULL, &dst);

        // Sélection des touches
        texture = Text_getTexture(self->textBinding3);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dst.x = g_titleRects.textBinding3.x;
        dst.y = g_titleRects.textBinding3.y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    

    if (config->keyCount == 4)
    {
         texture = Text_getTexture(self->textBinding4);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dst.x = g_titleRects.textBinding4.x;
        dst.y = g_titleRects.textBinding4.y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }

    if (config->keyCount == 5)
    {
        texture = Text_getTexture(self->textBinding4);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dst.x = g_titleRects.textBinding4.x;
        dst.y = g_titleRects.textBinding4.y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(renderer, texture, NULL, &dst);

        texture = Text_getTexture(self->textBinding5);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dst.x = g_titleRects.textBinding5.x;
        dst.y = g_titleRects.textBinding5.y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }

    // Commencer
    texture = Text_getTexture(self->textStart1);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textStart1.x;
    dst.y = g_titleRects.textStart1.y;
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
    if (self->pageID == 0)
        return TitleCanvas_updateMain(self);
    else if (self->pageID == 1)
        return TitleCanvas_updateSettings(self);
}
bool TitleCanvas_updateMain(TitleCanvas* self)
{
    TitleScene* scene = self->scene;
    AssetManager* assets = TitleScene_getAssetManager(scene);
    Input* input = TitleScene_getInput(scene);
    LevelConfig* config = TitleScene_getLevelConfig(scene);


    if (input->startPressed && self->selection==0)
    {
        playSwitchSound();
        self->pageID = 1;
        self->selection = 0;
        return false;
    }

    if (input->startPressed && self->selection == 2)
    {
        playSwitchSound();
        input->quitPressed = true;
    }

    if (input->downPressed || input->upPressed)
    {
        playSwitchSound();
        int idx = self->selection;
        idx += (input->downPressed) ? 1 : -1;
        idx = Int_clamp(idx, 0, 2);

        self->selection = idx;
    }

    Text* leftTexts[] = 
    {
        self->textSettings,
        self->textStart,
        self->textQuit,
    };
    for (int i = 0; i < 3; i++)
    {
        SDL_Color colors = (i == self->selection) ?
            assets->colors.marron : assets->colors.bleu_clair;
        Text_setColor(leftTexts[i], colors);
    }

    return (self->selection == 1 && input->startPressed);
}
bool TitleCanvas_updateSettings(TitleCanvas* self)
{
    TitleScene* scene = self->scene;
    AssetManager* assets = TitleScene_getAssetManager(scene);
    Input* input = TitleScene_getInput(scene);
    LevelConfig* config = TitleScene_getLevelConfig(scene);

    if (input->startPressed && self->selection == 4)
    {
        self->pageID = 0;
        self->selection = 0;
        playSwitchSound();
        return false;
    }

    if (input->downPressed || input->upPressed)
    {
        int idx = self->selection;
        idx += (input->downPressed) ? 1 : -1;
        idx = Int_clamp(idx, 0, 5);
        playSwitchSound();
        self->selection = idx;
    }

    if (input->leftPressed || input->rightPressed)
    {
        playSwitchSound();
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
            idx = 1 + ((idx + 2) % 3);
            scene->config.leveldifficulty.difficultyLevel = idx;
        }

        if (self->selection == 2)                       //Key binding
        {
            int idx = config->leveldifficulty.difficultyLevel;
            idx += (input->rightPressed) ? 1 : -1;
            idx = 1 + ((idx + 2) % 3);
            scene->config.leveldifficulty.difficultyLevel = idx;
        }
    }
    Text_setString(self->textMusic, g_musics[config->musicID].titleName);   // mise a jour du texte en fonction des action sutilisateurs
    char nbnotes[6];
    char difficulty[16];
    sprintf(nbnotes, u8"< %d >", config->keyCount);
    sprintf(difficulty, u8"< %d >", config->leveldifficulty.difficultyLevel);
    Text_setString(self->textNbNotes, nbnotes);// mise a jour du texte en fonction des action sutilisateurs


    Text* leftTexts[] = {
        self->textSelectMusic,
        self->textSelectNotes,
        self->textSelectDifficulty,
        self->textSelectBinding,
        self->textMenu,
        self->textStart1,
    };
    for (int i = 0; i < 6; i++)
    {
        SDL_Color colors = (i == self->selection) ?
            assets->colors.marron : assets->colors.bleu_clair;
        Text_setColor(leftTexts[i], colors);
    }
    return (self->selection == 5 && input->startPressed);
}
