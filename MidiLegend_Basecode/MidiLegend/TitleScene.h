/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "Common.h"

#include "AssetManager.h"
#include "TitleCanvas.h"
#include "Input.h"
#include "Track.h"

typedef struct LevelDiffiCulty {            //choix du niveau de difficult�
    int difficultyLevel;
    float multiplicator;
    bool NoMistakesAllowed;
    float Imprecision;

}LevelDifficulty;

/// @brief Structure repr�sentant les param�tres du niveau s�lectionn� dans le menu principal.
typedef struct LevelConfig
{
    /// @brief Identifiant de la musique.
    int musicID;

    /// @brief Nombre de touches/cordes du niveau (typiquement 3 � 5).
    int keyCount;

    /// @brief Bool�en indiquant si le niveau est jou� en mode piano (true) ou guitare (false).
    bool piano;

    LevelDifficulty leveldifficulty;
} LevelConfig;

/// @brief Structure repr�sentant la sc�ne principale du jeu (niveau).
typedef struct TitleScene
{
    /// @brief Moteur de rendu.
    SDL_Renderer *renderer;

    /// @brief Assets de la sc�ne (textures, fontes, couleurs, son...).
    AssetManager *assets;

    /// @brief Entr�es utilisateur.
    Input *input;

    /// @brief Canvas. Correspond � l'interface utilisateur (UI).
    TitleCanvas *canvas;

    Text* combo;
    TitleScene* scene;
    /// @brief Param�tres du niveau s�lectionn�
    /// (choix de la musique, du mode de jeu, du nombre de touches).
    LevelConfig config;

} TitleScene;

/// @brief Cr�e la sc�ne principale du jeu.
/// @param renderer moteur de rendu.
/// @return La sc�ne cr��e.
TitleScene *TitleScene_create(SDL_Renderer *renderer);

/// @brief D�truit la sc�ne principale.
/// @param self la sc�ne.
void TitleScene_destroy(TitleScene *self);

/// @brief Met � jour la sc�ne.
/// Cette fonction est appel�e � chaque tour de la boucle de rendu.
/// @param self la sc�ne.
/// @return true s'il faut quitter la boucle de rendu, false sinon.
bool TitleScene_update(TitleScene *self);

/// @brief Dessine la sc�ne dans le moteur de rendu.
/// @param self la sc�ne.
void TitleScene_render(TitleScene *self);

/// @brief Renvoie le moteur de rendu de la sc�ne.
/// @param self la sc�ne.
/// @return Le moteur de rendu de la sc�ne.
INLINE SDL_Renderer *TitleScene_getRenderer(TitleScene *self)
{
    return self->renderer;
}

/// @brief Renvoie les assets de la sc�ne.
/// @param self la sc�ne.
/// @return Les assets de la sc�ne.
INLINE AssetManager *TitleScene_getAssetManager(TitleScene *self)
{
    return self->assets;
}

/// @brief Renvoie les entr�es utilisateur de la sc�ne.
/// @param self la sc�ne.
/// @return Les entr�es utilisateur de la sc�ne.
INLINE Input *TitleScene_getInput(TitleScene *self)
{
    return self->input;
}

/// @brief Renvoie les param�tres du niveau s�lectionn� par le menu principal.
/// @param self la sc�ne.
/// @return Les param�tres du niveau s�lectionn�.
INLINE LevelConfig *TitleScene_getLevelConfig(TitleScene *self)
{
    return &(self->config);
}
