/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "Track.h"
#include "LevelScene.h"
#include "Math.h"

/// @brief Indique si une note a d�j� parcouru la zone de la piste visible
/// par le joueur.
/// @param self la note.
/// @param track la piste.
/// @return true si la note est pass�e,
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
/// false si elle est actuellement visble ou si elle l'a d�j� �t�.
bool Note_isFutur(Note *self, Track *track)
{
    float trackTime = (float)(track->scene->trackTime);
    float futurTime = track->visibleTime - track->pastTime;
    return (self->playingTime > trackTime + futurTime);
}

/// @brief Met � jour les indices de la premi�re note et de la derni�re note
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

    self->visibleTime = 2.0f/scene->difficultyLevel.FallingSpeed;
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
        if (notes[i].playingTime - notes[i - 1].playingTime >= 0.05 + self->scene->difficultyLevel.EasyPeasy * 0.1)   //si le niveau de difficult� est 1 alors les notes sont simplifi�es
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

    //affichage des notes en fonction de la position de la pr�c�dente
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
//permet de charger la texture de la particules
void render_particle(Track* self)
{

    LevelScene* scene = self->scene;
    AssetManager* assets = LevelScene_getAssetManager(scene);
    Input* input = LevelScene_getInput(scene);
    SDL_Renderer* renderer = LevelScene_getRenderer(scene);
    for (int i = 0; i < MAX_PARTICLE_COUNT; i++)//parcours du tableau 
    {
        SDL_Rect dst2 = { 0 };//d�finition du rectangle de la particule
        dst2.w = (2 + rand() % 6);//taille al�atoire 
        dst2.h = dst2.w;
        dst2.x = (int)self->particule[i].xposition;//position �quivalente au milieu de la note pr�ss�
        dst2.y = (int)self->particule[i].yposition;
        if (self->particule[i].duration > 0)//choix de la couleur de la particule

        {
            SDL_Rect src = { 8 * self->particule[i].textureID, 0, 8, 8 };//choix de la particule via le fichier 
            SDL_RenderCopy(renderer, assets->textures.particules, &src, &dst2);

        }
    }
}
//cr�taion de la particule 
void create_particle(Track* self, int keyID)
{
    LevelScene* scene = self->scene;
    Note* note = self->notes;
    Input* input = LevelScene_getInput(scene);
    float validationRelPos = 1.0f - self->pastTime / self->visibleTime;

    for (int i = 0; i < MAX_PARTICLE_COUNT; i++)//parcours du tableau
    {
        if (self->particule[i].duration <= 0)//recherche de la premi�re particule inactive
        {

            self->particule[i].duration = 1.5;//dur� de vie de la particule
            self->particule[i].xposition = 10 + (580 - ((self->keyCount - 1) * 100 + 40)) / 2 + 360.0f + keyID * 100;//position de la note pr�ss�
            self->particule[i].yposition = (float)(validationRelPos * 685) + 25;//position de la note pr�ss�
            self->particule[i].yspeed = (float)(rand() % 150) - 125;//vitesse initiale al�atoire entre -125 et 25
            self->particule[i].xspeed = (float)(rand() % 100) - 50;// vitesse initiale al�atoire entre -50 et 50
            self->particule[i].textureID = keyID;//permet d'avoir le num�ro de la touche pour la couleur de la particule

            break;
        }
    }
}
//fonction update particule appel� plus loin qui permet de modifier la position, vitesse et �tat de la particule
void update_particle(Track *self)
{
    LevelScene* scene = self->scene;
    for (int i = 0; i < MAX_PARTICLE_COUNT; i++)//parcours du tableau de particule
    {
        if (self->particule[i].duration > 0)//v�rification si la particule est en vie ou non
        {
            self->particule[i].duration -= Timer_getDelta(g_time);//r�duction de sa vie
            self->particule[i].yposition += Timer_getDelta(g_time) * self->particule[i].yspeed;//position en y
            self->particule[i].xposition += Timer_getDelta(g_time) * self->particule[i].xspeed;//position en x
            self->particule[i].yspeed += Timer_getDelta(g_time) * 100;//vitesse en y
            //modification de sa vitesse en x
            if (self->particule[i].xspeed<0)
                self->particule[i].xspeed += Timer_getDelta(g_time) * (-10);
            else
                self->particule[i].xspeed += Timer_getDelta(g_time) * 10;
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
    srand((unsigned int)time(NULL));

    update_particle(self);


    

    // Met � jour les indices des notes visibles par le joueur
    // Cela �vite de parcourir toutes les notes de la musique
    // � chaque frame (optimisation).
    Track_updateVisibleRange(self);

    for (int i = self->firstIdx; i <= self->lastIdx; i++)//parcours des notes 
    {
        Note* note = self->notes + i;
        int keyID = note->keyID;

        if (fabsf(note->playingTime - trackTime) < 0.15f)//v�rification si la touche correspondante � la note est touch� au bon moment
            LegalKeys[keyID] = 1;
    }

    for (int j = 0; j < self->keyCount; j++) {// v�rification si une autre touche n'est pas appuy� en m�me temps
        if (self->scene->difficultyLevel.NoMistakesAllowed && input->keyHit[j] && !LegalKeys[j])
        {
            score.points--;                                         //pour la difficult� la plus importante, l'utilisateur
            score.points = (score.points < 0) ? 0 : score.points;    //perd des points si il se trompe de touche
            LevelScene_setScore(scene, score);
        }

        if (input->keyDown[j] && !LegalKeys[j]) {    //si c'est le cas on quitte la fonction
            return;
        }
    }

    // Modifie l'�tat des notes visibles par le joueur
    for (int i = self->firstIdx; i <= self->lastIdx; i++)
    {
        Note *note = self->notes + i;
        int keyID = note->keyID;
        int NbKeyDown=0;

        if (note->state == NOTE_ACTIVE)
        {
            // La note est active, elle est donc en cours de descente
            // et n'a pas encore �t� trait�e par le joueur
            if (fabsf(note->playingTime - trackTime) < self->scene->difficultyLevel.Imprecision && input->keyHit[keyID])  // timer d'acceptation touche 0.1 qausi impossible 0.15 moyen 0.25 ez
            {
                if (note->type == TYPE_LONG) 
                    note->state = NOTE_HELD;
                    
                if (fabsf(note->playingTime - trackTime) < 0.01)
                {
                    score.Type = 1;//type de touche perfect
                    score.PerfectCount++;//compte de la touche (pour une page stats pas faite)
                }
                else if (fabsf(note->playingTime - trackTime) < 0.1)
                {
                    score.Type = 2;//type de touche good
                    score.GoodCount++;
                }
                else if (fabsf(note->playingTime - trackTime) < self->scene->difficultyLevel.Imprecision)
                {
                    score.Type = 3;//type de touche boof
                    score.BofCount++;
                }
                // L'�cart entre le temps courant de la musique et le d�but
                // de la note est inf�rieur � 0.5s

                score.combo+=1+score.PerfectCount;//ajout du combo
                score.combo = Int_clamp(score.combo, 1, 50);//combo ne depasse pas 50
                if ((score.combo / 10) == 0)
                    score.points = self->scene->difficultyLevel.multiplicator * 1 + score.points;        //le "multiplicator" permet de valoriser une difficult� plus importante
                else
                    score.points += (self->scene->difficultyLevel.multiplicator * 1) * (score.combo/5.0f);
                note->state = (note->state==NOTE_HELD) ? NOTE_HELD : NOTE_PLAYED;
                for (int j = 0; j < 2*(1+(score.combo/10)); j++)
                    create_particle(self, note->keyID);
                break;//break pour prendre les notes une par une

            }
            else if (note->playingTime + self->scene->difficultyLevel.Imprecision < trackTime)
            {
                // La note devait �tre jou�e il y a plus de 0.2s
                if (score.points > 0)
                {
                    score.points--;//retrait d'un point
                }
                score.combo -= 10 ;//retrait de 10 l'�quivalent d'un combo
                score.combo = Int_clamp(score.combo, 0, 50);//que le combo ne descende pas en dessous de 0
                note->state = NOTE_FAILED;
                score.Type = 4;//type de touche nope
                score.NopeCount++;
                break;//pour prendre les notes une par une
            }
                
        }
        if (note->state == NOTE_HELD) {

            note->playingTime = (float)scene->trackTime;

            if (trackTime > note->endingTime)
                note->state = NOTE_PLAYED;

            else if (!input->keyDown[keyID]) {
                note->state = NOTE_RELEASED;
            }

            else {
                score.points += 0.05f;
            }
        }
    }

    // Met � jour le score du joueur
    LevelScene_setScore(scene, score);  // TODO : decommenter
}

/// @brief Calcule la position relative d'une note dans la zone visible par le joueur.
/// Elle vaut 0.0f pour une note future (en haut) qui vient d'�tre visible par le joueur.
/// Elle vaut 1.0f pour une note pass�e (en bas) qui vient de ne plus �tre visible par le joueur.
/// @param self la piste.
/// @param note la note.
/// @return La position relative de la note dans la zone visible par le joueur.
float Track_getNoteRelativePosition(Track* self, Note* note)
{
    float trackTime = (float)self->scene->trackTime;
    float relativeNotePos = 1.0f - (note->playingTime - trackTime + self->pastTime) / self->visibleTime;
    return Float_clamp(relativeNotePos, -0.05f, 1.05f);
}
float Track_getNoteRelativePositionEnd(Track* self, Note* note)
{
    float trackTime = (float)self->scene->trackTime;
    float relativeNotePosEnd = 1.0f - (note->endingTime - trackTime + self->pastTime) / self->visibleTime;
    return Float_clamp(relativeNotePosEnd, -0.05f, 1.05f);
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

    float validationRelPos = 1.0f - self->pastTime / self->visibleTime;
    SDL_Color colortab[5] = {assets->colors.violet, assets->colors.blue, assets->colors.cyan, assets->colors.green, assets->colors.jaune};
    for (int i = 0; i < self->keyCount; i++)
    {
        Note* note = &(self->notes[i]);
        SDL_Rect barre = { 0 };

        barre.w = 2;
        barre.h = (int)(trackRect.y + validationRelPos * trackRect.h) - 25;
        barre.x = 20 + (580 - ((self->keyCount - 1) * 100 + 40)) / 2 + trackRect.x + i * 100;//centrer la descente des notes
        barre.y = 0;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
        SDL_RenderDrawRect(renderer, &barre);
        if (input->keyDown[i] == true)
        {
            SDL_SetRenderDrawColor(renderer, colortab[i].r, colortab[i].g, colortab[i].b, 255);
            SDL_RenderDrawRect(renderer, &barre);
        }
    }

    // Boucle de rendu des notes
    for (int i = self->firstIdx; i <= self->lastIdx; i++)
    {
        Note *note = &(self->notes[i]);

        // Si la note est jou�e, on ne la dessine pas
        if (note->state == NOTE_PLAYED) continue;

        // On r�cup�re la texture du la note
        SDL_Texture *texture = assets->textures.note;

        // On modifie son opacit� en fonction de l'�tat de la note
        // 0 = compl�tement transparent, 255 = compl�tement opaque
        Uint8 alpha = (note->state == NOTE_FAILED || note->state == NOTE_RELEASED) ? 100 : 255;
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(texture, alpha);

        // On calcule la position relative de la note dans la piste.
        float noteRelPos = Track_getNoteRelativePosition(self, note);
        float noteRelPosEnd = Track_getNoteRelativePositionEnd(self, note);

        // On d�finit la position et les dimensions de la note
        SDL_Rect dst = { 0 };
        dst.w = 40;
        if (note->type == TYPE_LONG) {
                dst.h = (int)(noteRelPos - noteRelPosEnd) * 720;
        }
        else
            dst.h = 25;
        
        dst.x = (580 - ((self->keyCount - 1) * 100 + 40)) / 2 + trackRect.x + note->keyID * 100;//centrer la descente des notes
        dst.y = (int)(trackRect.y + noteRelPos * trackRect.h);
        dst.y -= dst.h;

        // On dessine la note
        SDL_Rect src = { note->keyID * 37,0,37,25 };
        SDL_RenderCopy(renderer, texture, &src, &dst);
        
    }
    render_particle(self);
    // Calcule la position relative de la ligne de validation du joueur
    // dans la piste.
    // Boucle de rendu des touches du joueur
    for (int i = 0; i < self->keyCount; i++)
    {
        Note* note = &(self->notes[i]);

        SDL_Rect dst = { 0 };
        dst.w = 40;
        dst.h = 25;
        dst.x = (580 - ((self->keyCount - 1) * 100 + 40)) / 2 + trackRect.x + i * 100;//centrer la descente des notes
        dst.y = (int)(trackRect.y + validationRelPos * trackRect.h);
        dst.y -= dst.h;
        //SDL_Texture* texture = input->keyDown[i] ? assets->textures.keyDown : assets->textures.keyUp;
        SDL_RenderCopy(renderer, assets->textures.keyUp, NULL, &dst);
        if (input->keyDown[i] == true)
        {
            SDL_SetRenderDrawColor(renderer, colortab[i].r, colortab[i].g, colortab[i].b, 255);
            SDL_RenderFillRect(renderer, &dst);
            SDL_RenderDrawRect(renderer, &dst);
        }
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