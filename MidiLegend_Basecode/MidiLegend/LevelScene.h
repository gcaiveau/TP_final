
/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "Common.h"
#include "TitleScene.h"

#include "AssetManager.h"
#include "LevelCanvas.h"
#include "Input.h"
#include "Track.h"

/// @brief Structure repr�sentant les �lements servant au score du joueur.
/// Peut �galement contenir le nombre de combo, la valeur du multiplicateur, la pr�cision...
typedef struct LevelScore
{
    /// @brief Nombre de points du joueur.
    float points;

    int combo;

    int PerfectCount;
    int GoodCount;
    int BofCount;

    int Type;

    float BestScore[3][3];

} LevelScore;

/// @brief Structure repr�sentant la sc�ne principale du jeu (niveau).
typedef struct LevelScene
{
    /// @brief Moteur de rendu.
    SDL_Renderer *renderer;

    /// @brief Assets de la sc�ne (textures, fontes, couleurs, son...).
    AssetManager *assets;

    /// @brief Musique du niveau.
    Mix_Chunk *music;

    /// @brief Piste du niveau. Il s'agit de la zone de jeu.
    Track *track;

    /// @brief Temps courant dans la musique.
    /// Sa valeur est n�gative avant que la musique ne commence.
    double trackTime;

    /// @brief Bool�en indiquant si la musique est en train d'�tre jou�e.
    /// Est � false uniquement au d�but du niveau.
    bool playing;

    /// @brief Entr�es utilisateur.
    Input *input;

    /// @brief Canvas. Correspond � l'interface utilisateur (UI).
    LevelCanvas *canvas;

    /// @brief Score du joueur.
    /// Contient les informations essentielles de l'interface utilisateur (UI).
    LevelScore score;

    LevelDifficulty difficultyLevel;

} LevelScene;

/// @brief Cr�e la sc�ne du niveau.
/// @param renderer le moteur de rendu.
/// @param musicID l'identifiant de la musique.
/// @param keyCount le nombre de touches.
/// @param pianoMode bool�en indiquant si le jeu est en mode piano (ou guitare).
/// @return La sc�ne cr��e.
LevelScene *LevelScene_create(
    SDL_Renderer *renderer, LevelConfig config);

/// @brief D�truit la sc�ne du niveau.
/// @param self la sc�ne.
void LevelScene_destroy(LevelScene *self);

/// @brief Initialise la sc�ne du niveau.
/// @param self la sc�ne.
void LevelScene_start(LevelScene *self);

/// @brief Met � jour la sc�ne.
/// Cette fonction est appel�e � chaque tour de la boucle de rendu.
/// @param self la sc�ne.
/// @return true s'il faut quitter la boucle de rendu, false sinon.
bool LevelScene_update(LevelScene *self);
/// @brief Dessine la sc�ne dans le moteur de rendu.
/// @param self la sc�ne.
void LevelScene_render(LevelScene *self);

/// @brief Renvoie le moteur de rendu de la sc�ne.
/// @param self la sc�ne.
/// @return Le moteur de rendu de la sc�ne.
INLINE SDL_Renderer *LevelScene_getRenderer(LevelScene *self)
{
    return self->renderer;
}

/// @brief Renvoie les assets de la sc�ne.
/// @param self la sc�ne.
/// @return Les assets de la sc�ne.
INLINE AssetManager *LevelScene_getAssetManager(LevelScene *self)
{
    return self->assets;
}

/// @brief Renvoie les entr�es utilisateur de la sc�ne.
/// @param self la sc�ne.
/// @return Les entr�es utilisateur de la sc�ne.
INLINE Input *LevelScene_getInput(LevelScene *self)
{
    return self->input;
}

/// @brief Renvoie la piste (structure principale du jeu) de la sc�ne.
/// @param self la sc�ne.
/// @return La piste de la sc�ne.
INLINE Track *LevelScene_getTrack(LevelScene *self)
{
    return self->track;
}

/// @brief Renvoie la structure contenant toutes les informations sur le score du joueur.
/// @param self la sc�ne.
/// @return Le score du joueur.
INLINE LevelScore LevelScene_getScore(LevelScene *self)
{
    return self->score;
}

/// @brief D�finit le score du joueur.
/// @param self la sc�ne.
/// @param score le nouveau score du joueur.
INLINE void LevelScene_setScore(LevelScene *self, LevelScore score)
{
    self->score = score;
}