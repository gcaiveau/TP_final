/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "Text.h"


typedef struct LevelScene LevelScene;

/// @brief Structure repr�sentant le canvas du niveau.
/// Correspond � l'interface utilisateur (UI).
typedef struct LevelCanvas
{
    /// @brief La sc�ne du niveau.
    LevelScene *scene;

    /// @brief Texte affichant le nombre de points du joueur.
    Text *textPoints;
    Text* textPointsA;
    Text* textcombo;
    Text* textcomboA;
    Text* textPerfect;
    Text* textRecord;
    Text* textRecordA;

} LevelCanvas;

/// @brief Cr�e le canvas du niveau.
/// @param scene la sc�ne.
/// @return Le canvas cr��.
LevelCanvas *LevelCanvas_create(LevelScene *scene);

/// @brief D�truit le canvas du niveau.
/// @param self le canvas.
void LevelCanvas_destroy(LevelCanvas *self);

/// @brief Met � jour le canvas du niveau.
/// @param self le canvas.
void LevelCanvas_update(LevelCanvas *self);
/// @brief Dessine le canvas du niveau dans le moteur de rendu.
/// @param self le canvas.
void LevelCanvas_render(LevelCanvas *self);
