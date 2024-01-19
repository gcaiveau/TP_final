/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "Timer.h"

#define MUSIC_COUNT 4
typedef struct MusicSpec
{
    char *textPath;
    char *musicPath;
    char *titleName;
    char *composer;
    char *fullName;
    int bestScore;
} MusicSpec;


/// @brief Temps global du jeu.
extern Timer *g_time;

/// @brief Informations sur les musiques.
extern MusicSpec g_musics[MUSIC_COUNT];

typedef struct LevelRects
{
    /// @brief Position et dimensions du logo.
    SDL_Rect logo;

    /// @brief Position et dimensions du fond de la piste.
    SDL_Rect trackFill;

    /// @brief Position et dimensions du fond du masque
    /// en haut de la piste pour l'apparition en fondu des notes.
    SDL_Rect trackMask;

    SDL_Rect difficulte;
    /// @brief Position et dimensions du texte inquant le nombre de points.
    SDL_Rect points;
    SDL_Rect pointsA;
    SDL_Rect points1;
    SDL_Rect textRecord;

    SDL_Rect comboRect;

    SDL_Rect combo;
    SDL_Rect comboA;

    SDL_Rect textPerfect;
    /// @brief Position et dimensions du rectangle délimitant
    /// l'interface utilisateur (UI).
    SDL_Rect uiRect;
    SDL_Rect barre;

    /// @brief Position et dimensions de la barre de progression de la musique.
    SDL_Rect progressBar;

    SDL_Rect progressBarCombo;
    SDL_Rect recordBar;

} LevelRects;

typedef struct TitleRects
{
    //@brief Position et dimensions du texte indiquant de sélectionner les touches jouables
    SDL_Rect textSelectBinding;
    SDL_Rect textBinding1;
    SDL_Rect textBinding2;
    SDL_Rect textBinding3;
    SDL_Rect textBinding4;
    SDL_Rect textBinding5;

    //@brief Position et dimension du texte indiquant de sélectionner le mode de jeu
    SDL_Rect textSelectMode;
    SDL_Rect textMode;

    /// @brief Position et dimensions du texte inquant la musique
    /// à sélectionner.
    SDL_Rect textSelectMusic;

    SDL_Rect textQuit;
    SDL_Rect textQuit1;
    /// @brief Position et dimensions du texte inquant le nom de la musique
    /// sélectionnée.
    SDL_Rect textMusic;

    /// @brief Position et dimensions du texte pour commencer le niveau.
    SDL_Rect textStart;
    SDL_Rect textStart1;
    SDL_Rect textSettings;
    SDL_Rect textMenu;
    SDL_Rect textMenu1;
    SDL_Rect textRecommencer;
    SDL_Rect textTitre;
    SDL_Rect textBj;
    /// @brief Position et dimensions du texte pour changer le nombre de corde.
    SDL_Rect textNbNotes;

    /// @brief Position et dimensions du texte indiquant le nombre de corde.
    SDL_Rect textSelectNotes;

    /// @brief Position et dimensions du texte indiquant de choisir le niveau de difficulté.
    SDL_Rect textSelectDifficulty;

    SDL_Rect difficulte1;
    SDL_Rect difficulte2;
    SDL_Rect difficulte3;
    SDL_Rect barre;

    /// @brief Position et dimensions du texte indiquant le niveau de difficulté.

} TitleRects;

/// @brief Positions et dimensions des éléments fixes du niveau.
extern LevelRects g_levelRects;

/// @brief Positions et dimensions des éléments fixes du menu principal.
extern TitleRects g_titleRects;

/// @brief Initialise les librairies utilisées par le jeu.
/// @param sdlFlags les flags pour la SDL.
/// @param imgFlags les flags pour la SDL Image.
void Game_init(int sdlFlags, int imgFlags);

/// @brief Quitte les librairies utilisées par le jeu.