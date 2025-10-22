/**
 * @file edit.h
 * @brief Gestion de l'édition des formes et des groupes.
 * 
 * Ce module fournit les fonctions permettant de modifier les propriétés des
 * formes individuelles ou des groupes, ainsi que de déplacer des groupes.
 */

#ifndef EDIT_H
#define EDIT_H

#include "structures.h"

/**
 * @brief Déplace toutes les formes d'un groupe selon un décalage en x et y.
 * 
 * @param group Pointeur vers le groupe à déplacer.
 * @param dx Déplacement horizontal (positif vers la droite, négatif vers la gauche).
 * @param dy Déplacement vertical (positif vers le bas, négatif vers le haut).
 */
void move_group(Group* group, int dx, int dy);

/**
 * @brief Édite une forme ou un groupe spécifique.
 * 
 * Cette fonction affiche un menu interactif permettant de modifier les propriétés
 * d'une forme ou d'un groupe.
 * 
 * @param node Pointeur vers la forme ou le groupe à éditer.
 * @param list Liste de toutes les formes disponibles (utile pour vérifier les conflits ou relations).
 * @param start_col Position de départ de l'affichage (colonne du terminal).
 * @param idx Index de la forme/groupe dans la liste.
 * @param from_details Indique si l'édition est lancée depuis le menu de détails (true) ou depuis la liste principale (false).
 */
void edit_node(ShapeNode* node, ShapeList* list, int start_col, int idx, bool from_details);

/**
 * @brief Menu interactif pour éditer une forme ou un groupe.
 * 
 * Permet à l'utilisateur de sélectionner une forme ou un groupe dans la liste
 * et d'accéder au menu d'édition correspondant.
 * 
 * @param list Liste de toutes les formes et groupes disponibles.
 */
void edit_shape_or_group_menu(ShapeList* list);

#endif