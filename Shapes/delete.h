/**
 * @file delete.h
 * @brief Gestion de la suppression des formes et groupes.
 * 
 * Ce module fournit des fonctions permettant de supprimer des formes ou des groupes
 * depuis la liste principale ou depuis un menu de détails.
 */

#ifndef DELETE_H
#define DELETE_H

#include <stdbool.h>
#include "structures.h"

/**
 * @brief Affiche le menu interactif pour supprimer une forme ou un groupe.
 * 
 * L'utilisateur peut sélectionner un élément de la liste et le supprimer via ce menu.
 * 
 * @param list Liste de toutes les formes et groupes disponibles.
 */
void delete_shape_or_group_menu(ShapeList* list);

/**
 * @brief Supprime une forme ou un groupe spécifique de la liste.
 * 
 * Cette fonction effectue toutes les opérations nécessaires pour retirer
 * l'élément de la liste, mettre à jour les liens et libérer la mémoire associée.
 * 
 * @param list Liste contenant la forme ou le groupe à supprimer.
 * @param node Pointeur vers la forme ou le groupe à supprimer.
 * @param idx Index de l'élément dans la liste.
 * @param details_lines Nombre de lignes utilisées pour l'affichage des détails (utile pour la mise à jour de l'affichage).
 * @param from_details Indique si la suppression est lancée depuis le menu de détails (true) ou depuis la liste principale (false).
 */
void delete_shape_or_group(ShapeList* list, ShapeNode* node, int idx, int details_lines, bool from_details);

#endif