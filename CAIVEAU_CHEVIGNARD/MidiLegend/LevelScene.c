/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "LevelScene.h"
#include "Audio.h"

LevelScene *LevelScene_create(
    SDL_Renderer* renderer, LevelConfig config)
{
    LevelScene *self = (LevelScene *)calloc(1, sizeof(LevelScene));
    AssertNew(self);

    self->difficultyLevel.difficultyLevel = config.leveldifficulty.difficultyLevel;
    self->musicID = config.musicID;
    self->pageId = config.pageID;

    switch (self->difficultyLevel.difficultyLevel)
    {
    case 1:
        self->difficultyLevel.multiplicator = 1.0f;       //multiplicateur variant en fonction de la difficult� (plus c'est dur, plus �a rapporte)
        self->difficultyLevel.NoMistakesAllowed = 0;    
        self->difficultyLevel.Imprecision = 0.2f;         //imprecision autoris�e entre le moment ou la note doit etre jouee et celui ou le joueur appuie sur la touche
        self->difficultyLevel.FallingSpeed = 0.7f;        //la vitesse de chute des notes d�pend de la difficult�
        self->difficultyLevel.EasyPeasy = 1;             //simplifie le niveau en supprimant des notes trop proche
        break;
    case 2:
        self->difficultyLevel.multiplicator = 1.2f;
        self->difficultyLevel.NoMistakesAllowed = 0;
        self->difficultyLevel.Imprecision = 0.15f;
        self->difficultyLevel.FallingSpeed = 1.0f;
        self->difficultyLevel.EasyPeasy = 0;
        break;
    case 3:
        self->difficultyLevel.multiplicator = 1.5f;
        self->difficultyLevel.NoMistakesAllowed = 1;
        self->difficultyLevel.Imprecision = 0.07f;
        self->difficultyLevel.FallingSpeed = 1.3f;
        self->difficultyLevel.EasyPeasy = 0;
        break;
    }

 
    FILE* file = NULL;
    file = fopen("BestScore.txt", "r");
    
    if (file != NULL) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int check = fscanf(file, "%f", &(self->score.BestScore[i][j]));
            }
        }
        fclose(file);
    }


    self->renderer = renderer;
    self->assets = AssetManager_create(renderer);
    self->input = Input_create(config.piano);
    self->track = Track_create(self, g_musics[config.musicID].textPath);
    self->trackTime = -2.0f;
    self->music = Mix_LoadWAV(g_musics[config.musicID].musicPath);
    self->canvas = LevelCanvas_create(self);
    self->score.points = 0.0f;
    self->score.combo = 1;
    self->score.Type = 4;

    //self->difficultyLevel = ;

    Track_clearChords(self->track);
    Track_placeNotes(self->track, config.keyCount);

    return self;
}

void LevelScene_destroy(LevelScene *self)
{
    if (!self) return;

    FILE* file = NULL;
    file = fopen("BestScore.txt", "w");

    if (file != NULL) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                fprintf(file, "%f ", self->score.BestScore[i][j]);
            }
        }
        fclose(file);
    }

    Mix_HaltChannel(0);
    Mix_FreeChunk(self->music);

    AssetManager_destroy(self->assets);
    Input_destroy(self->input);
    Track_destroy(self->track);
    LevelCanvas_destroy(self->canvas);

    free(self);
}

void LevelScene_start(LevelScene *self)
{
    self->trackTime = -2.0f;
    self->playing = false;
}
bool LevelScene_update(LevelScene *self)
{
    // Met � jour les entr�es utilisateur
    Input_update(self->input);
    

    self->trackTime += Timer_getDelta(g_time);

    if (self->playing == false && self->trackTime >= 0.0)
    {
        int exitStatus = Mix_PlayChannel(0, self->music, 0);
        if (exitStatus < 0)
        {
            assert(false);
        }

        self->playing = true;
        self->trackTime = 0.0;
    }

    if (self->score.points > self->score.BestScore[self->musicID][self->difficultyLevel.difficultyLevel -1])
        self->score.BestScore[self->musicID][self->difficultyLevel.difficultyLevel-1] = self->score.points;



    Track_update(self->track);
    LevelCanvas_update(self->canvas); 

    bool quit = self->input->quitPressed || self->input->menuPressed;
    if (self->input->pressf)
    {
        playSwitchSound();
        quit = true;
        return quit;
    }

    quit = quit || (self->trackTime > self->track->duration + 2.0f); 
    self->pageId = 2;
    return quit;

}
void LevelScene_render(LevelScene* self)
{
    SDL_Renderer* renderer = LevelScene_getRenderer(self);
    AssetManager* assets = LevelScene_getAssetManager(self);
    SDL_RenderCopy(renderer, assets->textures.background, NULL, NULL);

    Track_render(self->track);
    LevelCanvas_render(self->canvas);
}
