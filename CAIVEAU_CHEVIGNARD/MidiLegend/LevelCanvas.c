/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "LevelCanvas.h"
#include "LevelScene.h"
#include "TitleCanvas.h"
#include "Math.h"

LevelCanvas *LevelCanvas_create(LevelScene *scene)
{
    LevelCanvas *self = (LevelCanvas *)calloc(1, sizeof(LevelCanvas));
    AssertNew(self);

    AssetManager *assets = LevelScene_getAssetManager(scene);
    SDL_Renderer *renderer = LevelScene_getRenderer(scene);

    self->scene = scene;
    self->textPoints = Text_create(renderer, assets->fonts.big, "0", assets->colors.white);
    self->textcombo = Text_create(renderer, assets->fonts.big, u8"0", assets->colors.white);
    self->textcomboA = Text_create(renderer, assets->fonts.normal, u8"Combo", assets->colors.white);
    self->textPointsA = Text_create(renderer, assets->fonts.normal, u8"Points", assets->colors.white);
    self->textRecordA = Text_create(renderer, assets->fonts.normal, u8"Record", assets->colors.white);
    self->textPerfect = Text_create(renderer, assets->fonts.perfect, u8"0", assets->colors.green);
    self->textRecord = Text_create(renderer, assets->fonts.big, u8"0", assets->colors.white);

    return self;
}

void LevelCanvas_destroy(LevelCanvas *self)
{
    if (!self) return;

    Text_destroy(self->textPoints);
    Text_destroy(self->textcombo);
    Text_destroy(self->textPointsA);
    Text_destroy(self->textcomboA);
    Text_destroy(self->textPerfect);
    Text_destroy(self->textRecord);
    Text_destroy(self->textRecordA);

    free(self);
}
void LevelCanvas_render(LevelCanvas *self)
{
    LevelScene *scene = self->scene;
    Track *track = LevelScene_getTrack(scene);
    AssetManager *assets = LevelScene_getAssetManager(scene);
    SDL_Renderer *renderer = LevelScene_getRenderer(scene);

    // Rectangle de score
    SDL_Color color = assets->colors.bleu_fonce;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 150);
    SDL_RenderFillRect(renderer, &(g_levelRects.uiRect));

    int w, h;
    SDL_Rect dst = { 0 };
    SDL_Texture *texture = NULL;

    // Rectangle de combo;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 150);
    SDL_RenderFillRect(renderer, &(g_levelRects.comboRect));

    color = assets->colors.violet;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
    SDL_RenderFillRect(renderer, &(g_levelRects.barre));

    // Points
    texture = Text_getTexture(self->textPoints);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_levelRects.points.x + (g_levelRects.points.w - w) / 2;
    dst.y = g_levelRects.points.y - 23;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //Record
    texture = Text_getTexture(self->textRecord);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = 100;
    dst.y = g_levelRects.textRecord.y - 23;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //combo
    texture = Text_getTexture(self->textcombo);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_levelRects.combo.x + (g_levelRects.combo.w - w) / 2;
    dst.y = g_levelRects.combo.y - 23;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //affichage point
    texture = Text_getTexture(self->textPointsA);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_levelRects.pointsA.x + (g_levelRects.pointsA.w - w) / 2;
    dst.y = g_levelRects.pointsA.y - 23;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //affichage combo 
    texture = Text_getTexture(self->textcomboA);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_levelRects.comboA.x + (g_levelRects.comboA.w - w) / 2;
    dst.y = g_levelRects.comboA.y - 23;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //affichage perfect
    texture = Text_getTexture(self->textPerfect);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_levelRects.textPerfect.x + (g_levelRects.textPerfect.w - w) / 2;
    dst.y = g_levelRects.textPerfect.y - 23;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    //affichage record
    texture = Text_getTexture(self->textRecordA);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_levelRects.textRecordA.x ;
    dst.y = g_levelRects.textRecordA.y ;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // Barre de progression
    dst = g_levelRects.progressBar;
    color = assets->colors.white;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
    SDL_RenderFillRect(renderer, &dst);

    float trackRatio = (float)(scene->trackTime / track->duration);
    trackRatio = Float_clamp(trackRatio, 0.0f, 1.0f);
    dst.x += 2; dst.y += 2;
    dst.w -= 4; dst.h -= 4;
    dst.w = (int)(trackRatio * dst.w);

    color = assets->colors.marron;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &dst);

    // Barre de progression combo
    dst = g_levelRects.progressBarCombo;
    color = assets->colors.white;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
    SDL_RenderFillRect(renderer, &dst);

    float ProgressRatio = (float)((scene->score.combo)/50.f);
    ProgressRatio = Float_clamp(ProgressRatio, 0.0f, 1.0f);
    dst.x += 2; dst.y += 2;
    dst.w -= 4; dst.h -= 4;
    dst.w = (int)(ProgressRatio * dst.w);

    color = assets->colors.marron;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &dst);

    //progrees record
    dst = g_levelRects.recordBar;
    color = assets->colors.white;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
    SDL_RenderFillRect(renderer, &dst);

    float RecordRatio = (float)(scene->score.points/(float)scene->score.BestScore[scene->musicID][scene->difficultyLevel.difficultyLevel - 1]);
    RecordRatio = Float_clamp(RecordRatio, 0.0f, 1.0f);
    dst.x += 2; dst.y += 2;
    dst.w -= 4; dst.h -= 4;
    dst.w = (int)(RecordRatio * dst.w);

    color = assets->colors.marron;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &dst);

       //contour des barres de progression guizmo
    SDL_SetRenderDrawColor(renderer, 222, 138, 78, 255);
    SDL_RenderDrawRect(renderer, &(g_levelRects.uiRect));
    SDL_RenderDrawRect(renderer, &(g_levelRects.progressBar));
    SDL_RenderDrawRect(renderer, &(g_levelRects.comboRect));
    
}
void LevelCanvas_update(LevelCanvas* self)
{
    Track* track = LevelScene_getTrack(self->scene);
    LevelScore score = LevelScene_getScore(self->scene);
    AssetManager* assets = LevelScene_getAssetManager(self->scene);
    Input* input = LevelScene_getInput(self->scene);
    char buffer[128] = { 0 };

    // Met � jour l'affichage du nombre de points du joueur
    sprintf(buffer, "%d", (int)score.points);
    Text_setString(self->textPoints, buffer);
    sprintf(buffer, "%d", (int)score.points);
    Text_setString(self->textPoints, buffer);
    sprintf(buffer, "x %d", (int)(score.combo / 10));//affichage du combo
    Text_setString(self->textcombo, buffer);
    sprintf(buffer, "%d", (int)score.BestScore[track->scene->musicID][track->scene->difficultyLevel.difficultyLevel-1]);
    Text_setString(self->textRecord, buffer);
    if (score.Type == 1)//v�rification du type de pr�cision
    {
        sprintf(buffer, "Perfect !");//passage dans un tableau
        Text_setColor(self->textPerfect, assets->colors.green);//changements de couleur
        Text_setString(self->textPerfect, buffer);
    }
    else if (score.Type == 2)
    {
        sprintf(buffer, "Good :)");
        Text_setColor(self->textPerfect, assets->colors.jaune_clair);
        Text_setString(self->textPerfect, buffer);
    }
    else if (score.Type == 3)
    {
        sprintf(buffer, "Bof :(");
        Text_setColor(self->textPerfect, assets->colors.rouge);
        Text_setString(self->textPerfect, buffer);
    }
    else if (score.Type == 4)
    {
        sprintf(buffer, "Nope...");
        Text_setColor(self->textPerfect, assets->colors.violet);
        Text_setString(self->textPerfect, buffer);
    }
}
