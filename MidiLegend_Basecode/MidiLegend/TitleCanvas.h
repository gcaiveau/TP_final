/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"
#include "Text.h"

typedef struct TitleScene TitleScene;

/// @brief Structure représentant le canvas du menu principal.
/// Correspond à l'interface utilisateur (UI).
typedef struct TitleCanvas
{
    /// @brief La scène du menu principal.
    TitleScene *scene;

    /// @brief Le texte indiquant de sélectionner la musique du niveau.
    Text *textSelectMusic;

    /// @brief Le texte indiquant le nombre de cordes.
    Text* textNbNotes;

    /// @brief Le texte indiquant de sélectionner le nombre de cordes.
    Text* textSelectNotes;

    /// @brief texte indiquant de choisir le niveau de difficulté
    Text* textSelectDifficulty;

    Text* textSettings;

    Text* textMenu;
    Text* textTitre;
    Text* textQuit;
    Text* textSelectBinding;
    Text* textBinding1;
    Text* textBinding2;
    Text* textBinding3;
    Text* textBinding4;
    Text* textBinding5;
    /// @brief texte indiquant le niveau de difficulté
    Text* textDifficultyValue;

    Text* textNotes;

    /// @brief Le texte affichant le nom de la musique sélectionnée.
    Text *textMusic;  // TODO : decommenter

    /// @brief Le texte de début du niveau.
    Text *textStart;

    Text* textStart1;

    /// @brief Indice du paramètre sélectionné dans le menu.
    int selection;

    int pageID;

} TitleCanvas;

/// @brief Crée le canvas du menu principal.
/// @param scene la scène.
/// @return Le canvas créé.
TitleCanvas *TitleCanvas_create(TitleScene *scene);

/// @brief Détruit le canvas du menu principal.
/// @param self le canvas.
void TitleCanvas_destroy(TitleCanvas *self);

/// @brief Met à jour le canvas du menu principal.
/// @param self le canvas.
bool TitleCanvas_update(TitleCanvas *self);
bool TitleCanvas_updateMain(TitleCanvas* self);
bool TitleCanvas_updateSettings(TitleCanvas* self);

/// @brief Dessine le canvas du menu principal dans le moteur de rendu.
/// @param self le canvas.
void TitleCanvas_render(TitleCanvas *self);
void TitleCanvas_renderMain(TitleCanvas* self);
void TitleCanvas_renderSettings(TitleCanvas* self);
