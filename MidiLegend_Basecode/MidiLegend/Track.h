/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"

typedef struct LevelScene LevelScene;

// Duree minimale pour qu'une note soit consid�r�e comme longue
#define MINIMAL_LONG_NOTE 0.3

/// @brief Enum�ration des deux types d'une note (courte/longue).
typedef enum NoteType
{
    /// @brief Type d'une note courte.
    TYPE_SHORT,

    /// @brief Type d'une note longue.
    TYPE_LONG
} NoteType;

/// @brief Enum�ration des �tats possibles d'une note.
typedef enum NoteState
{
    /// @brief Etat d'une note en cours de descente.
    NOTE_ACTIVE,

    /// @brief Etat d'une note jou�e et valid�e.
    NOTE_PLAYED,

    /// @brief Etat d'une note (longue) qui est maintenue.
    NOTE_HELD,

    /// @brief Etat d'une note (longue) qui a �t� jou�e mais pas maintenue jusqu'� sa fin.
    NOTE_RELEASED,

    /// @brief Etat d'une note rat�e.
    NOTE_FAILED
} NoteState;

/// @brief Structure repr�sentant une note.
typedef struct Note
{
    /// @brief Type de la note (courte ou longue).
    NoteType type;

    /// @brief Etat de la note (active, jou�e, rat�e...).
    NoteState state;

    /// @brief Identifiant de la touche/corde sur laquelle la note doit �tre jou�e.
    int keyID;

    /// @brief Valeur/hauteur de la note selon la norme MIDI.
    /// Plus cette valeur est grande, plus la note est aigu� (fr�quence �lev�e).
    int value;

    /// @brief Temps de d�but de la note.
    float playingTime;

    /// @brief Temps de fin de la note.
    float endingTime;

    /// @brief Dur�e de la note (= endingTime - playingTime).
    float duration;

} Note;

typedef struct Particules
{
    float xposition;
    float yposition;
    float xspeed;
    float yspeed;
    float duration;
    int textureID;

} Particules;

#define MAX_PARTICLE_COUNT 128

/// @brief Structure repr�sentant la piste du jeu.
/// Il s'agit de la structure principale du jeu.
typedef struct Track
{
    /// @brief La sc�ne du niveau.
    LevelScene *scene;

    /// @brief Nombre de notes de la piste.
    int noteCount;

    /// @brief Tableau des notes de la piste.
    Note *notes;

    /// @brief Nombre de touches/cordes pour le joueur.
    int keyCount;

    /// @brief Temps visible par le joueur.
    /// Correspond au temps qu'une note met pour parcourir enti�rement la zone visible.
    float visibleTime;

    /// @brief Temps pass� visible par le joueur.
    /// Correspond au temps qu'une note met pour parcourir la zone entre
    /// la ligne de validation du joueur et le bas de la piste.
    float pastTime;

    /// @brief Dur�e de la musique.
    /// Correspond au temps de fin de la derni�re note
    /// (+ 1s pour prendre en compte la r�verb�ration).
    float duration;

    /// @brief Indice de la premi�re note visible dans le tableaux des notes.
    int firstIdx;

    /// @brief Indice de la derni�re note visible dans le tableau des notes.
    int lastIdx;

    Particules particule[MAX_PARTICLE_COUNT];
} Track;

/// @brief Cr�e la piste du niveau.
/// @param scene la sc�ne.
/// @param filename le chemin du fichier contenant la partition.
/// @return La piste cr��e.
Track *Track_create(LevelScene *scene, char *filename);

/// @brief D�truit la piste du niveau.
/// @param self la piste.
void Track_destroy(Track *self);

/// @brief Supprime les accords (plusieurs notes jou�es en m�me temps).
/// Cela permet de simplifier le placement des notes et le gameplay.
/// @param self la piste.
void Track_clearChords(Track *self);

/// @brief Algorithme de placement des notes sur les touches/cordes pour le joueur.
/// Se base sur les valeurs (ou hauteurs) r�elles des notes pour donner un gameplay coh�rent.
/// @param self la piste.
/// @param keyCount le nombre de touches/cordes pour le joueur.
void Track_placeNotes(Track *self, int keyCount);

/// @brief Met � jour la piste.
/// Cette m�thode analyse les actions du joueur ainsi que l'�tat de piste
/// pour d�terminer les notes valid�es ou rat�es.
/// Elle met �galement � jour le score du joueur.
/// @param self la piste.
void Track_update(Track *self);

/// @brief Dessine la piste dans le moteur de rendu.
/// @param self la piste.
void Track_render(Track *self);

void update_particle(Track* self);
void render_particle(Track* self);
void create_particle(Track* self);