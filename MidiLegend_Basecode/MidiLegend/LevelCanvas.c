/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "LevelCanvas.h"
#include "LevelScene.h"
#include "Math.h"

LevelCanvas *LevelCanvas_create(LevelScene *scene)
{
    LevelCanvas *self = (LevelCanvas *)calloc(1, sizeof(LevelCanvas));
    AssertNew(self);

    AssetManager *assets = LevelScene_getAssetManager(scene);
    SDL_Renderer *renderer = LevelScene_getRenderer(scene);

    self->scene = scene;
    self->textPoints = Text_create(renderer, assets->fonts.big, "0", assets->colors.white);
    self->textcombo = Text_create(renderer, assets->fonts.normal, u8"0", assets->colors.white);
    self->textPerfect = Text_create(renderer, assets->fonts.normal, u8"0", assets->colors.green);


    return self;
}

void LevelCanvas_destroy(LevelCanvas *self)
{
    if (!self) return;

    Text_destroy(self->textPoints);
    Text_destroy(self->textcombo);
    Text_destroy(self->textPerfect);

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

    texture = Text_getTexture(self->textcombo);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_levelRects.combo.x + (g_levelRects.combo.w - w) / 2;
    dst.y = g_levelRects.combo.y - 23;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    texture = Text_getTexture(self->textPerfect);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dst.x = g_levelRects.textPerfect.x + (g_levelRects.textPerfect.w - w) / 2;
    dst.y = g_levelRects.textPerfect.y - 23;
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

    float ComboRatio = (float)((scene->score.combo/10) / 5.f);
    ComboRatio = Float_clamp(ComboRatio, 0.0f, 1.0f);
    dst.x += 2; dst.y += 2;
    dst.w -= 4; dst.h -= 4;
    dst.w = (int)(ComboRatio * dst.w);

    color = assets->colors.marron;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &dst);

    // DEBUG
    //Gizmos du canvas en jaune
    //SDL_RenderDrawRect(renderer, &(g_levelRects.points));
    SDL_SetRenderDrawColor(renderer, 222, 138, 78, 255);
    SDL_RenderDrawRect(renderer, &(g_levelRects.uiRect));
    SDL_RenderDrawRect(renderer, &(g_levelRects.progressBar));
    SDL_RenderDrawRect(renderer, &(g_levelRects.comboRect));
    
}

void LevelCanvas_update(LevelCanvas *self)
{
    Track *track = LevelScene_getTrack(self->scene);
    LevelScore score = LevelScene_getScore(self->scene);
    AssetManager* assets = LevelScene_getAssetManager(self->scene);
    char buffer[128] = { 0 };
    
    // Met à jour l'affichage du nombre de points du joueur
    sprintf(buffer, "%d", (int)score.points);
    Text_setString(self->textPoints, buffer);
    sprintf(buffer, "x %d", (int)(score.combo / 10));//affichage du combo
    Text_setString(self->textcombo, buffer);
    if (score.Type == 1)
    {
        sprintf(buffer, "Perfect !");
        Text_setString(self->textPerfect, buffer);
    }
    else if (score.Type == 2)
    {
        sprintf(buffer, "Good :)");
        Text_setString(self->textPerfect, buffer);
    }
    else if (score.Type == 3)
    {
        sprintf(buffer, "Bof :(");
        Text_setString(self->textPerfect, buffer);
    }
    else if (score.Type == 4)
    {
        sprintf(buffer, "Nope...");
        Text_setString(self->textPerfect, buffer);
    }
}
