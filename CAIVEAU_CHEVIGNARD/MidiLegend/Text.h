/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"

/// @brief Structure repr�sentant d'un texte affichable.
typedef struct Text
{
    char *str;
    TTF_Font *fonts;
    SDL_Color color;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
} Text;

/// @brief Cr�e un texte affichable.
/// @param renderer le moteur de rendu.
/// @param font la fonte utilis�e pour rendre le texte.
/// @param str la cha�ne associ�e au texte.
/// @param color la couleur de rendu.
/// @return Le texte affichable cr��.
Text *Text_create(SDL_Renderer *renderer, TTF_Font *font, const char *str, SDL_Color color);

/// @brief D�truit un texte affichable.
/// @param self le texte.
void Text_destroy(Text *self);

/// @brief Change le contenu du texte.
/// @param self le texte.
/// @param str la nouvelle cha�ne � rendre.
void Text_setString(Text *self, const char *str);

/// @brief Change le couleur de rendu d'un texte affichable.
/// @param self le texte.
/// @param color la couleur.
void Text_setColor(Text *self, SDL_Color color);

/// @brief Renvoie la cha�ne associ�e � un texte affichable.
/// @param self le texte.
/// @return La cha�ne (contenu) du texte.
INLINE const char *Text_getString(Text *self)
{
    return (const char*)(self->str);
}

/// @brief Renvoie la texture (rendu) d'un texte affichable.
/// @param self le texte.
/// @return La texture du texte.
INLINE SDL_Texture *Text_getTexture(Text *self)
{
    return self->texture;
}
