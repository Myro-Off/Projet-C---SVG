/**
 * @file memory.h
 * @brief Gestion de la libération de la mémoire pour les formes et les listes de formes.
 *
 * Ce module contient les fonctions responsables de la libération correcte
 * des structures allouées dynamiquement dans le programme, notamment les formes
 * (cercles, rectangles, polygones, groupes, etc.) et les listes de formes.
 *
 * Chaque fonction s’assure que la mémoire est libérée proprement,
 * y compris les champs internes dynamiques (comme les tableaux de points),
 * tout en évitant les double-free ou les fuites mémoire.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "structures.h"

/**
 * @brief Libère la mémoire allouée pour la forme contenue dans un nœud.
 *
 * Cette fonction libère la mémoire associée au champ `data` du `ShapeNode`,
 * selon son type (`CIRCLE`, `RECTANGLE`, `POLYGON`, `GROUP`, etc.).
 * 
 * @warning Cette fonction **ne libère pas** le nœud `ShapeNode` lui-même,
 * uniquement la structure de la forme et ses champs internes.
 *
 * @param node Pointeur vers le nœud dont la forme doit être libérée.
 */
void free_shape_node(ShapeNode* node);

/**
 * @brief Libère la mémoire de toutes les formes contenues dans une liste.
 *
 * Cette fonction parcourt toute la liste chaînée de formes et libère :
 * - la mémoire des formes (via `free_shape_node()`),
 * - la mémoire de chaque nœud (`ShapeNode`).
 *
 * À la fin de l’opération, la liste est réinitialisée :
 * `head = tail = NULL` et `size = 0`.
 *
 * @param list Pointeur vers la liste de formes à libérer.
 */
void free_all_shapes(ShapeList* list);

#endif