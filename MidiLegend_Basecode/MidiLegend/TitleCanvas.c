/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "TitleCanvas.h"
#include "TitleScene.h"
#include "Math.h"
#include "Audio.h"

TitleCanvas *TitleCanvas_create(TitleScene *scene, bool game_statue)
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
    self->textSelectMode = Text_create(renderer, assets->fonts.normal, u8"Mode de jeu", assets->colors.white);
    self->textMode = Text_create(renderer, assets->fonts.normal, u8"piano ou guitare", assets->colors.white);
    
    self->textMenu1 = Text_create(renderer, assets->fonts.normal, u8"Menu", assets->colors.bleu_clair);
    self->textRecommencer = Text_create(renderer, assets->fonts.normal, u8"Recommencer", assets->colors.bleu_clair);
    self->textQuit1 = Text_create(renderer, assets->fonts.normal, u8"Quitter", assets->colors.bleu_clair);
    self->textStat = Text_create(renderer, assets->fonts.big, u8"Stats", assets->colors.bleu_clair);

    self->textPerfect = Text_create(renderer, assets->fonts.normal, u8"Perfect :", assets->colors.bleu_clair);
    self->countPerfect = Text_create(renderer, assets->fonts.normal, u8"0", assets->colors.bleu_clair);

    self->textGood = Text_create(renderer, assets->fonts.normal, u8"Good :", assets->colors.bleu_clair);
    self->countGood = Text_create(renderer, assets->fonts.normal, u8"0", assets->colors.bleu_clair);

    self->textBof = Text_create(renderer, assets->fonts.normal, u8"Bof :", assets->colors.bleu_clair);
    self->countBof = Text_create(renderer, assets->fonts.normal, u8"0", assets->colors.bleu_clair);

    self->textNope = Text_create(renderer, assets->fonts.normal, u8"Nope :", assets->colors.bleu_clair);
    self->countNope = Text_create(renderer, assets->fonts.normal, u8"0", assets->colors.bleu_clair);

    self->textTotal = Text_create(renderer, assets->fonts.normal, u8"Total :", assets->colors.bleu_clair);
    self->countTotal = Text_create(renderer, assets->fonts.normal, u8"0", assets->colors.bleu_clair);

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
    Text_destroy(self->textMode);
    Text_destroy(self->textSelectMode);

    Text_destroy(self->textMenu1);
    Text_destroy(self->textQuit1);
    Text_destroy(self->textRecommencer);

    Text_destroy(self->textStat);

    Text_destroy(self->textTotal);
    Text_destroy(self->textGood);
    Text_destroy(self->textPerfect);
    Text_destroy(self->textBof);
    Text_destroy(self->textNope);
    Text_destroy(self->countNope);
    Text_destroy(self->countBof);
    Text_destroy(self->countGood);
    Text_destroy(self->countPerfect);
    Text_destroy(self->countTotal);

    Mix_HaltMusic();
    free(self);
}
void TitleCanvas_render(TitleCanvas* self)
{
    TitleScene* scene = self->scene;
    if (scene->config.pageID == 0 )
        TitleCanvas_renderMain(self);
    else if (scene->config.pageID == 1)
        TitleCanvas_renderSettings(self);
    else if (scene->config.pageID == 2)
        TitleCanvas_renderFin(self);

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

    // Sélection du Mode
    texture = Text_getTexture(self->textSelectMode);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textSelectMode.x;
    dst.y = g_titleRects.textSelectMode.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // Affichage du Mode
    texture = Text_getTexture(self->textMode);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textMode.x;
    dst.y = g_titleRects.textMode.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

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
void TitleCanvas_renderFin(TitleCanvas* self)
{
    TitleScene* scene = self->scene;
    SDL_Renderer* renderer = TitleScene_getRenderer(scene);
    LevelConfig* config = TitleScene_getLevelConfig(scene);

    int w, h;
    SDL_Rect dst = { 0 };
    SDL_Texture* texture = NULL;

    texture = Text_getTexture(self->textTitre);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textTitre.x;
    dst.y = g_titleRects.textTitre.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textQuit1);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textQuit1.x;
    dst.y = g_titleRects.textQuit1.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textMenu1);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textMenu1.x;
    dst.y = g_titleRects.textMenu1.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textRecommencer);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textRecommencer.x;
    dst.y = g_titleRects.textRecommencer.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    /*texture = Text_getTexture(self->textPerfect);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textPerfect.x;
    dst.y = g_titleRects.textPerfect.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textBof);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textBof.x;
    dst.y = g_titleRects.textBof.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textGood);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textGood.x;
    dst.y = g_titleRects.textGood.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textPerfect);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textPerfect.x;
    dst.y = g_titleRects.textPerfect.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textNope);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textNope.x;
    dst.y = g_titleRects.textNope.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textTotal);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textTotal.x;
    dst.y = g_titleRects.textTotal.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textStat);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textStat.x;
    dst.y = g_titleRects.textStat.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->countBof);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.countBof.x;
    dst.y = g_titleRects.countBof.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->countTotal);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.countTotal.x;
    dst.y = g_titleRects.countTotal.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->countGood);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.countGood.x;
    dst.y = g_titleRects.countGood.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->countPerfect);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.countPerfect.x;
    dst.y = g_titleRects.countPerfect.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->countNope);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.countNope.x;
    dst.y = g_titleRects.countNope.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);*/
}


bool TitleCanvas_update(TitleCanvas *self)
{
    TitleScene* scene = self->scene;
    if (scene->config.pageID == 0)
        return TitleCanvas_updateMain(self);
    else if (scene->config.pageID == 1)
        return TitleCanvas_updateSettings(self);
    else if (scene->config.pageID == 2)
        return TitleCanvas_updateFin(self);
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
        scene->config.pageID = 1;
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
    

    if (input->startPressed && self->selection == 5)
    {
        scene->config.pageID = 0;
        self->selection = 0;
        playSwitchSound();
        return false;
    }

    if (input->downPressed || input->upPressed)
    {
        int idx = self->selection;
        idx += (input->downPressed) ? 1 : -1;
        idx = Int_clamp(idx, 0, 6);
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

        if (self->selection == 3)                       //Key binding
        {
            int idx = config->bindselected;
            idx += (input->rightPressed) ? 1 : -1;
            idx = Int_clamp(idx, 0, config->keyCount-1);
            config->bindselected = idx;
            
        }

        if (self->selection == 4)                       //Key binding
        {
            int idx = config->piano;
            idx += (input->rightPressed) ? 1 : -1;
            idx = (idx + 2) % 2;
            config->piano = idx;

        }
    }
    Text_setString(self->textMusic, g_musics[config->musicID].titleName);   // mise a jour du texte en fonction des action sutilisateurs
    char nbnotes[16];
    char difficulty[16];
    char key1[16];
    char mode[16];
    
    sprintf(nbnotes, u8"< %d >", config->keyCount);
    sprintf(difficulty, u8"< %d >", config->leveldifficulty.difficultyLevel);
    //sprintf(key1, u8"< %d >", scene->input->config.keyCodes[0]);
    Text_setString(self->textNbNotes, nbnotes);// mise a jour du texte en fonction des action sutilisateurs
   // Text_setString(self->textBinding1, key1);
    if (config->piano) 
        Text_setString(self->textMode, u8"Piano");
    else
        Text_setString(self->textMode, u8"Guitare");



    Text* leftTexts[] = {
        self->textSelectMusic,
        self->textSelectNotes,
        self->textSelectDifficulty,
        self->textSelectBinding,
        self->textSelectMode,
        self->textMenu,
        self->textStart1,
    };
    Text* bidings[] = {
        self->textBinding1,
        self->textBinding2,
        self->textBinding3,
        self->textBinding4,
        self->textBinding5 
    }; 

    if (self->selection == 3) {
        for (int i = 0; i < 5; i++) {
            SDL_Color colors = (i == config->bindselected) ?
                assets->colors.marron : assets->colors.bleu_clair;
            Text_setColor(bidings[i], colors);
        }
    }
    else {
        int i = config->bindselected;
        SDL_Color colors =  assets->colors.bleu_clair;
        Text_setColor(bidings[i], colors);
        config->bindselected = 0;
    }
    for (int i = 0; i < 7; i++)
    {
        SDL_Color colors = (i == self->selection) ?
            assets->colors.marron : assets->colors.bleu_clair;
        
        Text_setColor(leftTexts[i], colors);
    }
    return (self->selection == 6 && input->startPressed);
}
bool TitleCanvas_updateFin(TitleCanvas* self)
{
    TitleScene* scene = self->scene;
    AssetManager* assets = TitleScene_getAssetManager(scene);
    Input* input = TitleScene_getInput(scene);
    LevelConfig* config = TitleScene_getLevelConfig(scene);
    

    if (input->startPressed && self->selection == 0)
    {
       scene->config.pageID = 0;
        self->selection = 0;
        playSwitchSound();
        return false;
    }

    if (input->startPressed && self->selection == 2)
    {
        playSwitchSound();
        input->quitPressed = true;
    }

    if (input->downPressed || input->upPressed)
    {
        int idx = self->selection;
        idx += (input->downPressed) ? 1 : -1;
        idx = Int_clamp(idx, 0, 3);
        playSwitchSound();
        self->selection = idx;
    }

    Text* leftTexts[] = {
        self->textMenu1,
        self->textRecommencer,
        self->textQuit1,

    };
    //char count[16];
    //sprintf(count, u8"< %d >", score.PerfectCount );
    //text_setstring(self->countPerfect, count);// mise a jour du texte en fonction des action sutilisateurs
    //sprintf(count, u8"< %d >", score.GoodCount);
    //text_setstring(self->countGood, count);// mise a jour du texte en fonction des action sutilisateurs
    //sprintf(count, u8"< %d >", score.BofCount);
    //text_setstring(self->countBof, count);// mise a jour du texte en fonction des action sutilisateurs
    //sprintf(count, u8"< %d >", score.NopeCount);
    //text_setstring(self->countNope, count);// mise a jour du texte en fonction des action sutilisateurs
    //int total = score.PerfectCount + score.GoodCount + score.BofCount + score.NopeCount;
    //int sur = score.PerfectCount + score.GoodCount + score.BofCount;
    //sprintf(count, u8"< %d / %d >", sur, total);
    //text_setstring(self->countNope, count);// mise a jour du texte en fonction des action sutilisateurs

    for (int i = 0; i < 3; i++)
    {
        SDL_Color colors = (i == self->selection) ?
            assets->colors.marron : assets->colors.bleu_clair;

        Text_setColor(leftTexts[i], colors);
    }

    return (self->selection == 1 && input->startPressed);
}
