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
    
    self->textTitre = Text_create(renderer, assets->fonts.big, u8"Midi Legend", assets->colors.bleu_clair);

    //--------------------------------------------------------------------------------------------------------
    //---Menu Settings----//
    self->textSelectMusic = Text_create(renderer, assets->fonts.normal, u8"Musique", assets->colors.bleu_clair);
    self->textMusic = Text_create(renderer, assets->fonts.normal, u8"Nom de la musique", assets->colors.white);
    self->textStart = Text_create(renderer, assets->fonts.normal, u8"Commencer", assets->colors.bleu_clair);
    self->textSelectNotes = Text_create(renderer, assets->fonts.normal, u8"Nombres de Notes", assets->colors.bleu_clair);
    self->textNbNotes = Text_create(renderer, assets->fonts.normal, u8"Notes", assets->colors.white);
    self->textSelectDifficulty = Text_create(renderer, assets->fonts.normal, u8"Niveau de difficulté", assets->colors.bleu_clair);
    self->textMenu = Text_create(renderer, assets->fonts.normal, u8"Menu", assets->colors.bleu_clair);
    self->textSelectMode = Text_create(renderer, assets->fonts.normal, u8"Mode de jeu", assets->colors.white);
    self->textMode = Text_create(renderer, assets->fonts.normal, u8"piano ou guitare", assets->colors.white);
    //--------------------------------------------------------------------------------------------------------
    //---Menu----//    
    self->textSettings = Text_create(renderer, assets->fonts.normal, u8"Settings", assets->colors.bleu_clair);
    self->textStart1 = Text_create(renderer, assets->fonts.normal, u8"Commencer", assets->colors.bleu_clair);
    self->textQuit = Text_create(renderer, assets->fonts.normal, u8"Quitter", assets->colors.bleu_clair);
    //--------------------------------------------------------------------------------------------------------
//---Menu Fin----//
    self->textMenu1 = Text_create(renderer, assets->fonts.normal, u8"Menu", assets->colors.bleu_clair);
    self->textRecommencer = Text_create(renderer, assets->fonts.normal, u8"Recommencer", assets->colors.bleu_clair);
    self->textQuit1 = Text_create(renderer, assets->fonts.normal, u8"Quitter", assets->colors.bleu_clair);
    self->textBj = Text_create(renderer, assets->fonts.Bj, u8"Bien joué !", assets->colors.white);

    playMainAudio();
    

    return self;
}

void TitleCanvas_destroy(TitleCanvas *self)
{
    if (!self) return;

    Text_destroy(self->textTitre);
    //--------------------------------------------------------------------------------------------------------
    //---Menu---//
    Text_destroy(self->textStart);
    Text_destroy(self->textQuit);    
    Text_destroy(self->textSettings);
    //--------------------------------------------------------------------------------------------------------
    //---Menu Settings----//
    Text_destroy(self->textMusic);  
    Text_destroy(self->textSelectMusic);
    Text_destroy(self->textSelectNotes);
    Text_destroy(self->textNbNotes);
    Text_destroy(self->textMenu);
    Text_destroy(self->textSelectDifficulty);
    Text_destroy(self->textStart1);
    Text_destroy(self->textMode);
    Text_destroy(self->textSelectMode);
    //--------------------------------------------------------------------------------------------------------
    //---Menu fin----//
    Text_destroy(self->textMenu1);
    Text_destroy(self->textQuit1);
    Text_destroy(self->textRecommencer);
    Text_destroy(self->textBj);

    Mix_HaltMusic();//couper la musique de fond
    free(self);
}

//Fonction render qui permet de choisir sur quelle page faut-il appliquer la fonction
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
    //Settings
    texture = Text_getTexture(self->textSettings);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textSettings.x;
    dst.y = g_titleRects.textSettings.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    //Titre
    texture = Text_getTexture(self->textTitre);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textTitre.x;
    dst.y = g_titleRects.textTitre.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    //Quit
    texture = Text_getTexture(self->textQuit);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textQuit.x;
    dst.y = g_titleRects.textQuit.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

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
    
    //Retour Menu
    texture = Text_getTexture(self->textMenu);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textMenu.x;
    dst.y = g_titleRects.textMenu.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //Titre
    texture = Text_getTexture(self->textTitre);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textTitre.x;
    dst.y = g_titleRects.textTitre.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

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

}
void TitleCanvas_renderFin(TitleCanvas* self)
{
    TitleScene* scene = self->scene;
    SDL_Renderer* renderer = TitleScene_getRenderer(scene);
    LevelConfig* config = TitleScene_getLevelConfig(scene);

    int w, h;
    SDL_Rect dst = { 0 };
    SDL_Texture* texture = NULL;

    //Titre
    texture = Text_getTexture(self->textTitre);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textTitre.x;
    dst.y = g_titleRects.textTitre.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //selectiion quit
    texture = Text_getTexture(self->textQuit1);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textQuit1.x;
    dst.y = g_titleRects.textQuit1.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //retour menu
    texture = Text_getTexture(self->textMenu1);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textMenu1.x;
    dst.y = g_titleRects.textMenu1.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //rejouer
    texture = Text_getTexture(self->textRecommencer);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textRecommencer.x;
    dst.y = g_titleRects.textRecommencer.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //bien joué
    texture = Text_getTexture(self->textBj);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_titleRects.textBj.x;
    dst.y = g_titleRects.textBj.y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

//Fonction update qui permet de choisir sur quelle page faut-il appliquer la fonction
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

    //vérifie si la touche entrer est préssé et si l'élément sélectionnné est bien le 0-> aller menu settings
    if (input->startPressed && self->selection==0)
    {
        playSwitchSound();
        scene->config.pageID = 1;//changment id page
        self->selection = 0;//remise de la variable selection à 0
        return false;//retourne faux pour ne pas passer à la fenetre de jeu
    }
    //vérifie si la touche entrer est préssé et si l'élément sélectionnné est bien le 2->quitter
    if (input->startPressed && self->selection == 2)
    {
        playSwitchSound();
        input->quitPressed = true;
    }
    //vérifie si une des flêches est préssé pour appliqué un changement de varaible ex: nombre de notes, mdj...
    if (input->downPressed || input->upPressed)
    {
        playSwitchSound();
        int idx = self->selection;
        idx += (input->downPressed) ? 1 : -1;
        idx = Int_clamp(idx, 0, 2);

        self->selection = idx;
    }
    //ordre des textes avec la varaiable sélection
    Text* leftTexts[] = 
    {
        self->textSettings,
        self->textStart,
        self->textQuit,
    };
    //changer la couleur du texte sur lequelle est sélection
    for (int i = 0; i < 3; i++)
    {
        SDL_Color colors = (i == self->selection) ?
            assets->colors.marron : assets->colors.bleu_clair;
        Text_setColor(leftTexts[i], colors);
    }

    return (self->selection == 1 && input->startPressed);//si sélection est sur 1 et que entrer est préssé -> le jeu commence
}
bool TitleCanvas_updateSettings(TitleCanvas* self)
{
    TitleScene* scene = self->scene;
    AssetManager* assets = TitleScene_getAssetManager(scene);
    Input* input = TitleScene_getInput(scene);
    LevelConfig* config = TitleScene_getLevelConfig(scene);
    

    if (input->startPressed && self->selection == 4)
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

        if (self->selection == 3)                       //choix difficulté
        {
            int idx = config->leveldifficulty.difficultyLevel;
            idx += (input->rightPressed) ? 1 : -1;
            idx = 1 + ((idx + 2) % 3);
            scene->config.leveldifficulty.difficultyLevel = idx;
        }

        if (self->selection == 2)                       //Key binding
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
        Text_setString(self->textMode, u8"< Piano >");
    else
        Text_setString(self->textMode, u8"< Guitare >");



    Text* leftTexts[] = {
        self->textSelectMusic,
        self->textSelectNotes,
        self->textSelectMode,
        self->textSelectDifficulty,
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
        idx = Int_clamp(idx, 0, 2);
        playSwitchSound();
        self->selection = idx;
    }

    Text* leftTexts[] = {
        self->textMenu1,
        self->textRecommencer,
        self->textQuit1,

    };

    for (int i = 0; i < 3; i++)
    {
        SDL_Color colors = (i == self->selection) ?
            assets->colors.marron : assets->colors.bleu_clair;

        Text_setColor(leftTexts[i], colors);
    }

    return (self->selection == 1 && input->startPressed);
}
