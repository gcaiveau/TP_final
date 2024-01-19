/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"

typedef struct InputConfig
{
    /// @brief Bool�en indiquant si le jeu est en mode piano (true) ou guitare (false).
    bool pianoMode;

    /// @brief Valeurs des touches de la piste.
    SDL_Scancode keyCodes[MAX_KEY_COUNT];

    /// @brief Valeur de la touche de validation en mode guitare.
    SDL_Scancode strumCode;
} InputConfig;

void InputConfig_init(InputConfig *config, bool pianoMode);

/// @brief Structure repr�sentant le gestionnaire des entr�es utilisateur.
typedef struct Input
{
    /// @brief Configuration des touches de jeu.
    InputConfig config;

    /// @brief Bool�en indiquant si le bouton "quitter" vient d'�tre press�.
    bool quitPressed;

    /// @brief Bool�en indiquant si le bouton "commencer"
    /// vient d'�tre press� (menu principal).
    bool startPressed;

    /// @brief Bool�en indiquant si le bouton "retour au menu"
    /// vient d'�tre press� (niveau).
    bool menuPressed;

    /// @brief Tableau de bool�ens indiquant les touches de la piste
    /// qui viennent d'�tre press�es (niveau).
    bool keyHit[MAX_KEY_COUNT];

    /// @brief Tableau de bool�ens indiquant les touches de la piste
    /// qui sont enfonc�es (niveau).
    bool keyDown[MAX_KEY_COUNT];

    /// @brief Bool�en indiquant si la fl�che directionnelle "haut"
    /// vient d'�tre press�e (menu principal).
    bool upPressed;

    /// @brief Bool�en indiquant si la fl�che directionnelle "bas"
    /// vient d'�tre press�e (menu principal).
    bool downPressed;

    bool pressf;

    /// @brief Bool�en indiquant si la fl�che directionnelle "gauche"
    /// vient d'�tre press�e (menu principal).
    bool leftPressed;

    /// @brief Bool�en indiquant si la fl�che directionnelle "droite"
    /// vient d'�tre press�e (menu principal).
    bool rightPressed;
} Input;

/// @brief Cr�e un nouveau gestionnaire des entr�es utilisateur.
/// @param pianoMode indique si le jeu est configur� pour un piano ou un guitare.
/// @return Le gestionnaire des entr�es utilisateur cr��.
Input *Input_create(bool pianoMode);

/// @brief D�truit un gestionnaire des entr�es utilisateur.
/// @param self le gestionnaire des entr�es utilisateur.
void Input_destroy(Input *self);

/// @brief Met � jour le gestionnaire des entr�es utilisateur.
/// Cette fonction effectue la boucle des �v�nement SDL.
/// @param self le gestionnaire des entr�es utilisateur.
void Input_update(Input *self);
