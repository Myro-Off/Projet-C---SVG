/**
 * @file list.h
 * @brief Gestion des listes de formes et de groupes.
 *
 * Ce module contient l’ensemble des fonctions nécessaires à la
 * manipulation, l’affichage et la gestion de la liste principale
 * de formes (`ShapeList`) ainsi que des groupes de formes.
 * 
 * Il gère :
 * - L’initialisation et la libération mémoire des listes.
 * - L’ajout, la suppression et la recherche de formes.
 * - L’affichage détaillé des formes et des groupes.
 * - Les interactions avec le menu de sélection.
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "structures.h"

/**
 * @brief Initialise une liste de formes vide.
 *
 * Met à zéro les pointeurs `head`, `tail` et le compteur `size`.
 *
 * @param list Pointeur vers la liste à initialiser.
 */
void init_shape_list(ShapeList* list);

/**
 * @brief Ajoute une nouvelle forme à la liste.
 *
 * Alloue un nouveau nœud contenant la forme spécifiée et l’ajoute
 * à la fin de la liste.
 *
 * @param list Pointeur vers la liste de formes.
 * @param type Type de la forme à ajouter.
 * @param data Données spécifiques à la forme.
 */
void add_shape(ShapeList* list, ShapeType type, ShapeData data);

/**
 * @brief Supprime un nœud spécifique de la liste.
 *
 * Retire la forme de tous les groupes où elle est présente,
 * libère sa mémoire, puis met à jour les pointeurs de la liste.
 *
 * @param list Pointeur vers la liste principale.
 * @param node Nœud à supprimer.
 */
void delete_node(ShapeList* list, ShapeNode* node);

/**
 * @brief Libère complètement la mémoire de la liste.
 *
 * Supprime toutes les formes et groupes de la liste, puis
 * réinitialise ses champs à zéro.
 *
 * @param list Pointeur vers la liste à libérer.
 */
void free_shape_list(ShapeList* list);

/**
 * @brief Récupère un nœud à un index donné.
 *
 * Parcourt la liste et retourne le pointeur vers le nœud situé
 * à la position spécifiée.
 *
 * @param list Pointeur vers la liste.
 * @param index Position du nœud recherché (0-indexé).
 * @return ShapeNode* Pointeur vers le nœud correspondant, ou NULL si invalide.
 */
ShapeNode* get_node_at(ShapeList* list, int index);

/**
 * @brief Affiche les détails d’une forme avec un menu d’action.
 *
 * Affiche toutes les propriétés d’une forme (position, couleur, etc.)
 * et propose les actions d’édition ou de suppression.
 *
 * @param node Pointeur vers la forme à afficher.
 * @param idx Index de la forme dans la liste.
 * @param list Liste principale des formes.
 */
void display_shape_details(ShapeNode* node, int idx, ShapeList* list);

/**
 * @brief Affiche les informations d’un groupe sans interaction.
 *
 * Utilisée pour montrer les propriétés générales du groupe
 * (nom, nombre d’éléments, liste de sous-formes, etc.)
 *
 * @param group Pointeur vers le groupe à afficher.
 * @param idx Index du groupe dans la liste principale.
 * @return int Nombre de lignes affichées (utile pour le positionnement terminal).
 */
int display_group_info(const Group* group, int idx);

/**
 * @brief Affiche les détails complets d’un groupe avec menu interactif.
 *
 * Propose à l’utilisateur de modifier, supprimer ou explorer
 * le contenu du groupe sélectionné.
 *
 * @param node Nœud représentant le groupe.
 * @param group Pointeur vers le groupe correspondant.
 * @param index Position du groupe dans la liste principale.
 * @param global_list Liste principale contenant tous les éléments.
 */
void display_group_details(ShapeNode* node, Group* group, int index, ShapeList* global_list);

/**
 * @brief Affiche la liste globale des formes et groupes.
 *
 * Montre chaque élément avec son type, son index et sa
 * potentielle appartenance à un groupe.
 *
 * @param list Pointeur vers la liste principale.
 */
void display_list(const ShapeList* list);

/**
 * @brief Affiche le menu de sélection d’un élément dans la liste.
 *
 * Permet à l’utilisateur de choisir une forme ou un groupe à afficher
 * en détail via son index.
 *
 * @param list Pointeur vers la liste principale.
 */
void ask_list(const ShapeList* list);

/**
 * @brief Retourne le nom du groupe auquel appartient une forme.
 *
 * Recherche récursivement dans tous les groupes pour déterminer
 * si la forme spécifiée appartient à l’un d’entre eux.
 *
 * @param list Liste principale contenant les groupes.
 * @param node Nœud représentant la forme à tester.
 * @return const char* Nom du groupe si trouvé, NULL sinon.
 */
const char* get_group_name_for_shape(const ShapeList* list, const ShapeNode* node);

/**
 * @brief Affiche les informations d’une forme (sans menu).
 *
 * Affiche en détail les propriétés de la forme : coordonnées,
 * taille, couleur, épaisseur, transformation, etc.
 *
 * @param node Pointeur vers la forme à afficher.
 * @param list Liste principale pour déterminer l’appartenance à un groupe.
 * @param idx Index de la forme dans la liste.
 * @return int Nombre de lignes affichées (utile pour l’alignement terminal).
 */
int display_shape_info(const ShapeNode* node, const ShapeList* list, int idx);

#endif