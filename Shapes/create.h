/**
 * @file create.h
 * @brief Fonctions pour la création interactive de formes.
 * 
 * Ce module fournit des fonctions permettant à l'utilisateur de créer
 * différentes formes (cercle, carré, rectangle, ligne, ellipse, polygone, polyline)
 * et de les ajouter à la liste principale des formes.
 */

#ifndef CREATE_H
#define CREATE_H

#include "list.h"

/**
 * @brief Crée un cercle à partir des valeurs saisies par l'utilisateur.
 * 
 * Demande à l'utilisateur les coordonnées du centre et le rayon, puis ajoute
 * le cercle à la liste de formes.
 * 
 * @param list Liste chaînée de formes à compléter.
 */
void ask_circle(ShapeList* list);

/**
 * @brief Crée un carré à partir des valeurs saisies par l'utilisateur.
 * 
 * Demande à l'utilisateur la position et la taille du carré, puis l'ajoute à la liste.
 * 
 * @param list Liste chaînée de formes à compléter.
 */
void ask_square(ShapeList* list);

/**
 * @brief Crée un rectangle à partir des valeurs saisies par l'utilisateur.
 * 
 * Demande les coordonnées et dimensions du rectangle, puis l'ajoute à la liste.
 * 
 * @param list Liste chaînée de formes à compléter.
 */
void ask_rectangle(ShapeList* list);

/**
 * @brief Crée une ligne à partir des valeurs saisies par l'utilisateur.
 * 
 * Demande les coordonnées des points de départ et d'arrivée de la ligne,
 * puis l'ajoute à la liste.
 * 
 * @param list Liste chaînée de formes à compléter.
 */
void ask_line(ShapeList* list);

/**
 * @brief Crée une ellipse à partir des valeurs saisies par l'utilisateur.
 * 
 * Demande les coordonnées du centre et les rayons horizontal et vertical,
 * puis ajoute l'ellipse à la liste.
 * 
 * @param list Liste chaînée de formes à compléter.
 */
void ask_ellipse(ShapeList* list);

/**
 * @brief Crée un polygone à partir des points saisis par l'utilisateur.
 * 
 * L'utilisateur saisit les coordonnées de chaque point du polygone,
 * puis la forme est ajoutée à la liste.
 * 
 * @param list Liste chaînée de formes à compléter.
 */
void ask_polygon(ShapeList* list);

/**
 * @brief Crée une polyline (ligne brisée ouverte) à partir des points saisis.
 * 
 * L'utilisateur saisit plusieurs points consécutifs, et la polyline
 * est ajoutée à la liste.
 * 
 * @param list Liste chaînée de formes à compléter.
 */
void ask_multiline(ShapeList* list);

/**
 * @brief Crée un chemin (path) à partir des points saisis.
 * 
 * Fonction similaire à ask_multiline mais adaptée pour les chemins spécifiques.
 * 
 * @param list Liste chaînée de formes à compléter.
 */
void ask_path(ShapeList* list);

#endif