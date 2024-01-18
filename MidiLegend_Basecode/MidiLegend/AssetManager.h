/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"

/// @brief Structure contenant l'ensemble des assets du jeu.
/// Cela correspond aux ressources utilis�es (textures, fontes, couleurs, son...)
typedef struct AssetManager
{
    /// @brief Ensemble des textures du jeu.
    struct Textures
    {
        /// @brief Texture de fond.
        SDL_Texture *background;

        /// @brief Texture du logo du jeu.
        SDL_Texture *logo;

        /// @brief Texture du fond de la piste.
        SDL_Texture *trackFill;  // TODO : decommenter

        /// @brief Texture du masque utilis� en haut de la piste pour faire
        /// apparaitre les notes en fondu.
        SDL_Texture *trackMask;

        /// @brief Texture d'une note.
        SDL_Texture *note;

        /// @brief Texture d'une touche non enfonc�e de la piste.
        SDL_Texture *keyUp;

        /// @brief Texture d'une touche enfonc�e de la piste.
        SDL_Texture *keyDown;

        SDL_Texture* keyDown1;

        SDL_Texture* difficulte;

        SDL_Texture* difficulte1;



    } textures;

    /// @brief Ensemble des fontes de caract�res du jeu.
    struct Fonts
    {
        /// @brief Fonte pour un texte de taille interm�diaire.
        TTF_Font *normal;

        /// @brief Fonte pour un texte de grande taille.
        TTF_Font *big;

        /// @brief Fonte pour un texte de petite taille.
        TTF_Font *small;

        TTF_Font* perfect;
    } fonts;

    /// @brief Ensemble des couleurs du jeu.
    struct Colors
    {
        /// @brief Couleur repr�sentant du bleu.
        SDL_Color blue;

        /// @brief Couleur repr�sentant du vert.
        SDL_Color green;

        /// @brief Couleur repr�sentant du noir.
        SDL_Color black;

        /// @brief Couleur repr�sentant du blanc.
        SDL_Color white;
        SDL_Color marron;
        SDL_Color bleu_clair;
        SDL_Color bleu_fonce;
        SDL_Color cyan;
        SDL_Color rouge;
        SDL_Color jaune_clair;
        SDL_Color jaune;
        SDL_Color violet;

    } colors;
} AssetManager;

/// @brief Cr�e la structure contenant les assets du jeu.
/// @param renderer le moteur de rendu.
/// @return La structure contenant les assets du jeu.
AssetManager *AssetManager_create(SDL_Renderer *renderer);

/// @brief D�truit la structure contenant les assets du jeu.
/// @param self les assets.
void AssetManager_destroy(AssetManager *self);
