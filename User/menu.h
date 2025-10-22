/**
 * @file menu.h
 * @brief Définition des fonctions liées à l'affichage et à la gestion du menu principal.
 *
 * Ce module gère l'introduction du programme, le menu principal,
 * et le sous-menu de création de formes. Il constitue le point d'entrée
 * principal du programme graphique en terminal.
 *
 * @author
 * Adam Valsan
 */

#ifndef MENU_H
#define MENU_H

#include "structures.h"

/**
 * @brief Affiche l'introduction du programme et le menu principal.
 *
 * Cette fonction efface le terminal, affiche une introduction stylisée
 * lors du premier lancement, puis montre le menu principal avec les
 * différentes options disponibles pour interagir avec les formes et groupes.
 *
 * @param first_launch Définit si le programme est lancé pour la première fois.
 *                     Si vrai, affiche l'écran d’introduction avant le menu.
 */
void display_introduction(bool first_launch);

/**
 * @brief Affiche le menu de création de formes.
 *
 * Permet à l’utilisateur de choisir quel type de forme créer parmi les
 * différentes options disponibles (cercle, carré, ligne, etc.).  
 * Cette fonction gère les entrées utilisateur et redirige vers la fonction
 * de création appropriée.
 *
 * @param list Pointeur vers la liste principale contenant toutes les formes.
 */
void create_menu(ShapeList* list);

#endif