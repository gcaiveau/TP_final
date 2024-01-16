/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "Text.h"

typedef struct TitleScene TitleScene;

/// @brief Structure repr�sentant le canvas du menu principal.
/// Correspond � l'interface utilisateur (UI).
typedef struct TitleCanvas
{
    /// @brief La sc�ne du menu principal.
    TitleScene *scene;

    /// @brief Le texte indiquant de s�lectionner la musique du niveau.
    Text *textSelectMusic;

    /// @brief Le texte indiquant de s�lectionner la musique du niveau.
    Text* textNotes;

    /// @brief Le texte indiquant de s�lectionner la musique du niveau.
    Text* textSelectNotes;

    /// @brief Le texte affichant le nom de la musique s�lectionn�e.
    Text *textMusic;  // TODO : decommenter

    /// @brief Le texte de d�but du niveau.
    Text *textStart;

    /// @brief Indice du param�tre s�lectionn� dans le menu.
    int selection;
} TitleCanvas;

/// @brief Cr�e le canvas du menu principal.
/// @param scene la sc�ne.
/// @return Le canvas cr��.
TitleCanvas *TitleCanvas_create(TitleScene *scene);

/// @brief D�truit le canvas du menu principal.
/// @param self le canvas.
void TitleCanvas_destroy(TitleCanvas *self);

/// @brief Met � jour le canvas du menu principal.
/// @param self le canvas.
bool TitleCanvas_update(TitleCanvas *self);

/// @brief Dessine le canvas du menu principal dans le moteur de rendu.
/// @param self le canvas.
void TitleCanvas_render(TitleCanvas *self);
