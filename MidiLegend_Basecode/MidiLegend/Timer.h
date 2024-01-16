/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#pragma once

#include "Settings.h"

/// @brief Structure repr�sentant un chronom�tre.
typedef struct Timer
{
    /// @brief Temps de d�part.
    float m_startTime;

    /// @brief Temps du dernier appel � Timer_update().
    float m_currentTime;

    /// @brief Temps de l'avant dernier appel � Timer_update().
    float m_previousTime;

    /// @brief Ecart entre les deux derniers appels � Timer_update().
    /// Ce membre est affect� par le facteur d'�chelle.
    float m_delta;

    /// @brief Ecart entre les deux derniers appels � Timer_update().
    /// Ce membre n'est pas affect� par le facteur d'�chelle.
    float m_unscaledDelta;

    /// @brief Facteur d'�chelle appliqu� au temps.
    float m_scale;

    /// @brief Ecart entre le lancement du timer Timer_start()
    /// et le dernier appel � Timer_update().
    /// Ce membre est affect� par le facteur d'�chelle.
    float m_elapsed;

    /// @brief Ecart entre le lancement du timer Timer_start()
    /// et le dernier appel � Timer_update().
    /// Ce membre n'est pas affect� par le facteur d'�chelle.
    float m_unscaledElapsed;
} Timer;

/// @brief Cr�e un nouveau timer.
/// @return Le timer cr��.
Timer* Timer_create();

/// @brief D�truit un timer pr�alablement allou� avec Timer_create().
/// @param self le timer � d�truire.
void Timer_destroy(Timer* self);

/// @brief Lance le timer.
/// @param self le timer.
void Timer_start(Timer* self);

/// @brief Met � jour le timer en fonction du temps actuel.
/// Cette fonction est typiquement appel�e � chaque tour de la boucle de rendu.
/// @param self le timer.
void Timer_update(Timer* self);

/// @brief D�finit le facteur d'�chelle de temps appliqu�e � un timer.
/// Si l'�chelle vaut 0.5f, le temps s'�coule deux fois moins rapidement.
/// @param self le timer.
/// @param scale le facteur d'�chelle.
INLINE void Timer_setTimeScale(Timer *self, float scale)
{
    assert(self && scale >= 0.0f);
    self->m_scale = scale;
}

/// @brief Renvoie le facteur d'�chelle de temps d'un timer.
/// @param self le timer.
/// @return Le facteur d'�chelle de temps d'un timer.
INLINE float Timer_getTimeScale(Timer *self)
{
    assert(self);
    return self->m_scale;
}

/// @brief Renvoie l'�cart de temps (en secondes) entre les deux derniers
/// appels � la fonction Timer_update().
/// @param self le timer.
/// @return L'�cart de temps entre les deux derni�res mises � jour.
INLINE float Timer_getDelta(Timer *self)
{
    assert(self);
    return self->m_delta;
}

/// @brief Renvoie l'�cart de temps (en secondes) entre les deux derniers
/// appels � la fonction Timer_Update().
/// Cette m�thode ne tient pas compte de l'�chelle de temps du timer.
/// @param self le timer.
/// @return L'�cart de temps entre les deux derni�res mises � jour
/// (sans �chelle de temps).
INLINE float Timer_getUnscaledDelta(Timer *self)
{
    assert(self);
    return self->m_unscaledDelta;
}

/// @brief Renvoie l'�cart de temps (en secondes) entre le lancement du timer
/// avec Timer_start() et le dernier appel � Timer_update().
/// @param self le timer.
/// @return Le nombre de secondes �coul�es depuis le lancement du timer et la derni�re mise � jour.
INLINE float Timer_getElapsed(Timer *self)
{
    assert(self);
    return self->m_elapsed;
}

/// @brief Renvoie l'�cart de temps (en secondes) entre le lancement du timer
/// avec Timer_Start() et le dernier appel � Timer_Update().
/// Cette m�thode en tient pas compte de l'�chelle de temps du timer.
/// @param self le timer.
/// @return Le nombre de secondes �coul�es depuis le lancement du timer et la
/// derni�re mise � jour  (sans �chelle de temps).
INLINE float Timer_getUnscaledElapsed(Timer *self)
{
    assert(self);
    return self->m_unscaledElapsed;
}
