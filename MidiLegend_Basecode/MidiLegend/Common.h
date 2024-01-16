/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "Timer.h"

#define MUSIC_COUNT 3
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

    /// @brief Position et dimensions du texte inquant le nombre de points.
    SDL_Rect points;

    SDL_Rect comboRect;

    SDL_Rect combo;
    /// @brief Position et dimensions du rectangle délimitant
    /// l'interface utilisateur (UI).
    SDL_Rect uiRect;

    /// @brief Position et dimensions de la barre de progression de la musique.
    SDL_Rect progressBar;

    SDL_Rect progressBarCombo;

} LevelRects;

typedef struct TitleRects
{
    /// @brief Position et dimensions du texte inquant la musique
    /// à sélectionner.
    SDL_Rect textSelectMusic;

    /// @brief Position et dimensions du texte inquant le nom de la musique
    /// sélectionnée.
    SDL_Rect textMusic;

    /// @brief Position et dimensions du texte pour commencer le niveau.
    SDL_Rect textStart;

    /// @brief Position et dimensions du texte pour changer le nombre de corde.
    SDL_Rect textNbNotes;

    /// @brief Position et dimensions du texte indiquant le nombre de corde.
    SDL_Rect textSelectNotes;

    /// @brief Position et dimensions du texte indiquant de choisir le niveau de difficulté.
    SDL_Rect textSelectDifficulty;

    /// @brief Position et dimensions du texte indiquant le niveau de difficulté.
    SDL_Rect textDifficultyValue;

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
void Game_quit();

