/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "Track.h"
#include "LevelScene.h"
#include "Math.h"

/// @brief Indique si une note a déjà parcouru la zone de la piste visible
/// par le joueur.
/// @param self la note.
/// @param track la piste.
/// @return true si la note est passée,
/// false si elle est actuellement visble ou si elle sera visible.
bool Note_isPast(Note *self, Track *track)
{
    float trackTime = (float)(track->scene->trackTime);
    return (self->endingTime < trackTime - track->pastTime);
}

/// @brief Indique si une note n'est pas entre apparue dans la zone
/// de la piste visible par le joueur.
/// @param self la note.
/// @param track la piste.
/// @return true si la note n'est pas encore apparue,
/// false si elle est actuellement visble ou si elle l'a déjà été.
bool Note_isFutur(Note *self, Track *track)
{
    float trackTime = (float)(track->scene->trackTime);
    float futurTime = track->visibleTime - track->pastTime;
    return (self->playingTime > trackTime + futurTime);
}

/// @brief Met à jour les indices de la première note et de la dernière note
/// qui sont visibles par joueur dans la piste.
/// @param self la piste.
void Track_updateVisibleRange(Track *self)
{
    int noteCount = self->noteCount;
    for (int i = self->firstIdx; i < noteCount; i++)
    {
        Note *note = self->notes + i;

        if (Note_isPast(note, self))
        {
            self->firstIdx++;
            continue;
        }

        if (Note_isFutur(note, self))
        {
            self->lastIdx = i;
            break;
        }
    }
}

Track *Track_create(LevelScene *scene, char *filename)
{
    FILE *file = fopen(filename, "r");
    AssertNew(file);

    Track *self = (Track *)calloc(1, sizeof(Track));
    AssertNew(self);

    int scanCount = 0;
    int noteCount = 0;
    float duration = 0.0f;
    float timeShift = 0.0f;

    scanCount = fscanf(file, "%d", &noteCount);
    assert(scanCount == 1);

    scanCount = fscanf(file, "%f", &timeShift);
    assert(scanCount == 1);

    scanCount = fscanf(file, "%f", &duration);
    assert(scanCount == 1);

    self->scene = scene;
    self->keyCount = 1;
    self->noteCount = noteCount;
    self->duration = duration;

    self->notes = (Note *)calloc(noteCount, sizeof(Note));
    AssertNew(self->notes);

    self->visibleTime = 2.0f*scene->difficultyLevel.FallingSpeed;
    self->pastTime = 0.15f * self->visibleTime;

    for (int i = 0; i < noteCount; i++)
    {
        Note *note = self->notes + i;

        scanCount = fscanf(file, "%d", &(note->value));
        assert(scanCount == 1);

        scanCount = fscanf(file, "%f", &(note->playingTime));
        assert(scanCount == 1);

        scanCount = fscanf(file, "%f", &(note->duration));
        assert(scanCount == 1);

        note->playingTime += timeShift;
        note->endingTime = note->playingTime + note->duration;

        if (note->duration >= MINIMAL_LONG_NOTE)
        {
            note->type = TYPE_LONG;
        }
        else
        {
            note->type = TYPE_SHORT;
        }
    }

    return self;
}

void Track_destroy(Track *self)
{
    if (!self) return;

    free(self->notes);
    free(self);
}

void Track_clearChords(Track *self)
{
    assert(self && self->notes);

    Note *notes = self->notes;
    int noteCount = self->noteCount;

    int noteIdx = 1;
    for (int i = 1; i < noteCount; i++)
    {
        if (notes[i].playingTime - notes[i - 1].playingTime >= 0.05 + self->scene->difficultyLevel.EasyPeasy * 0.1)   //si le niveau de difficulté est 1 alors les notes sont simplifiées
        {
            notes[noteIdx++] = notes[i];
        }
    }
    self->noteCount = noteIdx;
}

void Track_placeNotes(Track *self, int keyCount)
{
    assert(self && self->notes);

    self->keyCount = keyCount;

    Note *notes = self->notes;
    int noteCount = self->noteCount;

    // TODO : Ameliorer le placement des notes   
    for (int i = 0; i < noteCount; i++)
    {
        if (i == 0)
        {
            notes[i].keyID = rand() % keyCount;
        }
        else if (notes[i].value > notes[i - 1].value)
        {
            notes[i].keyID = (notes[i-1].keyID)+1;
            notes[i].keyID = Int_clamp(notes[i].keyID, 0, keyCount-1);
        }
        else if (notes[i].value < notes[i - 1].value)
        {
            notes[i].keyID = (notes[i-1].keyID)-1;
            notes[i].keyID = Int_clamp(notes[i].keyID, 0, keyCount-1);
        }
        else
        {
            notes[i].keyID = notes[i-1].keyID;
        }
    }
    
}

void Track_update(Track *self)
{
    LevelScene *scene = self->scene;
    Input *input = LevelScene_getInput(scene);
    LevelScore score = LevelScene_getScore(scene);
    float trackTime = (float)scene->trackTime;
    bool LegalKeys[5] = { 0 };
    //scene->config.leveldifficulty.difficultyLevel = 1;

    // Met à jour les indices des notes visibles par le joueur
    // Cela évite de parcourir toutes les notes de la musique
    // à chaque frame (optimisation).
    Track_updateVisibleRange(self);

    for (int i = self->firstIdx; i <= self->lastIdx; i++)//parcours des notes 
    {
        Note* note = self->notes + i;
        int keyID = note->keyID;

        if (fabsf(note->playingTime - trackTime) < 0.15f)//vérification si la touche correspondante à la note est touché au bon moment
            LegalKeys[keyID] = 1;
    }

    for (int j = 0; j < self->keyCount; j++) {// vérification si une autre touche n'est pas appuyé en même temps
        if (self->scene->difficultyLevel.NoMistakesAllowed && input->keyHit[j] && !LegalKeys[j])
        {
            score.points--;                                         //pour la difficulté la plus importante, l'utilisateur
            score.points = (score.points < 0) ? 0 : score.points;    //perd des points si il se trompe de touche
            LevelScene_setScore(scene, score);
        }

        //if (input->keyDown[j] && !LegalKeys[j]) {    //si c'est le cas on quitte la fonction
        //    return;
        //}
    }

    // Modifie l'état des notes visibles par le joueur
    for (int i = self->firstIdx; i <= self->lastIdx; i++)
    {
        Note *note = self->notes + i;
        int keyID = note->keyID;
        int NbKeyDown=0;

        if (note->state == NOTE_ACTIVE)
        {
            // La note est active, elle est donc en cours de descente
            // et n'a pas encore été traitée par le joueur
            if (fabsf(note->playingTime - trackTime) < self->scene->difficultyLevel.Imprecision && input->keyHit[keyID])  // timer d'acceptation touche 0.1 qausi impossible 0.15 moyen 0.25 ez
            {
                if (note->type == TYPE_LONG)
                    note->state = NOTE_HELD;

                if (fabsf(note->playingTime - trackTime) < 0.01)
                    score.PerfectCount = 1;
                else
                    score.PerfectCount = 0;
                // L'écart entre le temps courant de la musique et le début
                // de la note est inférieur à 0.5s

                score.combo+=1+score.PerfectCount;//ajout du combo
                score.combo = Int_clamp(score.combo, 1, 50);
                if ((score.combo / 5) == 0)
                    score.points = self->scene->difficultyLevel.multiplicator * 1 + score.points + score.PerfectCount;        //le "multiplicator" permet de valoriser une difficulté plus importante
                else
                    score.points += ((self->scene->difficultyLevel.multiplicator * 1) + score.PerfectCount) * (score.combo/5.0f);
                note->state = (note->state==NOTE_HELD) ? NOTE_HELD : NOTE_PLAYED;
                break;//break pour prendre les notes une par une

            }
            else if (note->playingTime + self->scene->difficultyLevel.Imprecision < trackTime)
            {
                // La note devait être jouée il y a plus de 0.2s
                /*if (score.points > 0)
                {
                    score.points--;
                }*/
                score.combo -= 5 ;
                score.combo = Int_clamp(score.combo, 0, 50);
                note->state = NOTE_FAILED;
                break;//pour prendre les notes une par une
            }
                
        }
        if (note->state == NOTE_HELD) {
            /*if (note->endingTime - trackTime == 0)
                note->state = NOTE_PLAYED;*/
            if (!input->keyDown[keyID]) {
            note->state = NOTE_RELEASED;
            }
            else
                score.points += 1;
        }
    }

    // Met à jour le score du joueur
    LevelScene_setScore(scene, score);  // TODO : decommenter
}

/// @brief Calcule la position relative d'une note dans la zone visible par le joueur.
/// Elle vaut 0.0f pour une note future (en haut) qui vient d'être visible par le joueur.
/// Elle vaut 1.0f pour une note passée (en bas) qui vient de ne plus être visible par le joueur.
/// @param self la piste.
/// @param note la note.
/// @return La position relative de la note dans la zone visible par le joueur.
float Track_getNoteRelativePosition(Track *self, Note *note)
{
    float trackTime = (float)self->scene->trackTime;
    float relativeNotePos = 1.0f - (note->playingTime - trackTime + self->pastTime) / self->visibleTime;
    return Float_clamp(relativeNotePos, -0.05f, 1.05f);
}

void Track_render(Track *self)
{
    LevelScene *scene = self->scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);
    Input *input = LevelScene_getInput(scene);
    SDL_Renderer *renderer = LevelScene_getRenderer(scene);

    // Dessine le fond de la piste
    SDL_Rect trackRect = g_levelRects.trackFill;
    SDL_RenderCopy(renderer, assets->textures.trackFill, NULL, &trackRect); // TODO : decommenter

    // Boucle de rendu des notes
    for (int i = self->firstIdx; i <= self->lastIdx; i++)
    {
        Note *note = &(self->notes[i]);

        // Si la note est jouée, on ne la dessine pas
        if (note->state == NOTE_PLAYED) continue;

        // On récupère la texture du la note
        SDL_Texture *texture = assets->textures.note;

        // On modifie son opacité en fonction de l'état de la note
        // 0 = complètement transparent, 255 = complètement opaque
        Uint8 alpha = (note->state == NOTE_FAILED) ? 100 : 255;
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(texture, alpha);

        // On calcule la position relative de la note dans la piste.
        float noteRelPos = Track_getNoteRelativePosition(self, note);

        // On définit la position et les dimensions de la note
        SDL_Rect dst = { 0 };
        dst.w = 37;
        if (note->type == TYPE_LONG)
            dst.h = 25 * 2;
        else
            dst.h = 25;
        
        dst.x = (580 - ((self->keyCount - 1) * 50 + 37)) / 2 + trackRect.x + note->keyID * 50;//centrer la descente des notes
        dst.y = (int)(trackRect.y + noteRelPos * trackRect.h);
        dst.y -= dst.h;

        // On dessine la note
        SDL_Rect src = { note->keyID * 37,0,37,25 };
        SDL_RenderCopy(renderer, texture, &src, &dst);
        
    }

    // Calcule la position relative de la ligne de validation du joueur
    // dans la piste.
    float validationRelPos = 1.0f - self->pastTime / self->visibleTime;

    // Boucle de rendu des touches du joueur
    for (int i = 0; i < self->keyCount; i++)
    {
        SDL_Rect dst = { 0 };
        dst.w = 37;
        dst.h = 25;
        dst.x = (580 - ((self->keyCount - 1) * 50 + 37)) / 2 + trackRect.x + i * 50;//centrer la descente des notes
        dst.y = (int)(trackRect.y + validationRelPos * trackRect.h);
        dst.y -= dst.h;

        SDL_Texture *texture = input->keyDown[i] ? assets->textures.keyDown : assets->textures.keyUp;
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }

    // On dessine le masque en dernier (pour qu'il soit au dessus des notes)
    SDL_RenderCopy(renderer, assets->textures.trackMask, NULL, &(g_levelRects.trackMask));


    /* DEBUG
    // Gizmos de la piste en magenta
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &(g_levelRects.trackMask));
    SDL_RenderDrawRect(renderer, &(g_levelRects.trackFill));
    */
}