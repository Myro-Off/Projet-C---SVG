/**
 * @file group.h
 * @brief Gestion des groupes de formes.
 * 
 * Ce module permet de créer, manipuler et libérer des groupes de formes.
 */

#ifndef GROUP_H
#define GROUP_H

#include "structures.h"
#include "list.h"

/**
 * @brief Crée un nouveau groupe vide.
 * 
 * @return Un pointeur vers le groupe créé, ou NULL en cas d'erreur.
 */
Group* create_group(void);

/**
 * @brief Ajoute une forme à un groupe.
 * 
 * Si la forme est déjà présente, elle n'est pas ajoutée et un message est affiché.
 * 
 * @param g Pointeur vers le groupe.
 * @param node Pointeur vers la forme à ajouter.
 */
void add_to_group(Group* g, ShapeNode* node);

/**
 * @brief Supprime une forme d'un groupe à partir de son index dans le sous-groupe.
 * 
 * @param group Le groupe dans lequel retirer la forme.
 * @param sub_index Index de la forme dans le sous-groupe.
 * @return true si la suppression a réussi, false sinon.
 */
bool remove_shape_from_group(Group* group, int sub_index);

/**
 * @brief Supprime une forme de tous les groupes présents dans une liste de formes.
 * 
 * @param list La liste de formes à parcourir.
 * @param node La forme à retirer de tous les groupes.
 */
void remove_shape_from_all_groups(ShapeList* list, const ShapeNode* node);

/**
 * @brief Libère toutes les formes contenues dans une liste de groupe.
 * 
 * @param list Liste de formes à libérer.
 */
void free_group_shape_list(ShapeList* list);

/**
 * @brief Libère un groupe et toutes ses formes.
 * 
 * @param g Groupe à libérer.
 */
void free_group(Group* g);

/**
 * @brief Lit le nom d'un groupe depuis l'entrée utilisateur.
 * 
 * Bloque l'entrée standard pour gérer le retour arrière et limite le nom à 12 caractères.
 * 
 * @param result Buffer pour stocker le nom.
 * @param prompt Message à afficher pour l'utilisateur.
 * @return INPUT_SUCCESS si le nom est saisi, INPUT_CANCEL si l'utilisateur annule.
 */
InputStatus get_group_name(char* result, const char* prompt);

/**
 * @brief Menu interactif pour créer un groupe.
 * 
 * Permet de sélectionner plusieurs formes avec les flèches et la barre espace,
 * et d'ajouter ces formes dans un nouveau groupe.
 * 
 * @param list Liste de formes disponibles pour la sélection.
 */
void create_group_menu(ShapeList* list);

#endif