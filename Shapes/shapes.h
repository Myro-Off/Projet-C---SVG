#ifndef SHAPES_H
#define SHAPES_H

#include "structures.h"

/**
 * @file shapes.h
 * @brief Fonctions de création de différentes formes géométriques.
 *
 * Ce fichier fournit des fonctions pour créer et initialiser des formes
 * telles que carrés, cercles, rectangles, ellipses, lignes, polygones,
 * polylignes et chemins (paths) de manière dynamique.
 */

/**
 * @brief Crée un nouveau carré.
 *
 * Alloue dynamiquement une structure Square et initialise ses attributs
 * avec les valeurs passées en paramètres.
 *
 * @param x Coordonnée X du coin supérieur gauche.
 * @param y Coordonnée Y du coin supérieur gauche.
 * @param side Longueur du côté (doit être > 0).
 * @param stroke_color Couleur du contour (ex : "black", "#000000").
 * @param fill_color Couleur de remplissage (ex : "white", "#FFFFFF").
 * @param thickness Épaisseur du contour.
 * @param rounded Coins arrondis (0 = pas arrondis).
 * @param scalex Facteur de mise à l'échelle horizontal.
 * @param scaley Facteur de mise à l'échelle vertical.
 * @return Pointeur vers le carré créé, ou NULL en cas d’erreur.
 */
Square* create_square(int x, int y, int side, const char* stroke_color, const char* fill_color, int thickness, int rounded, int scalex, int scaley);

/**
 * @brief Crée un nouveau cercle.
 *
 * Alloue dynamiquement une structure Circle et initialise ses attributs.
 *
 * @param x Coordonnée X du centre.
 * @param y Coordonnée Y du centre.
 * @param radius Rayon du cercle (doit être > 0).
 * @param stroke_color Couleur du contour (ex : "red", "#FF0000").
 * @param fill_color Couleur de remplissage (ex : "blue", "#0000FF").
 * @param thickness Épaisseur du contour.
 * @param scalex Facteur de mise à l'échelle horizontal.
 * @param scaley Facteur de mise à l'échelle vertical.
 * @return Pointeur vers le cercle créé, ou NULL en cas d’erreur.
 */
Circle* create_circle(int x, int y, int radius, const char* stroke_color, const char* fill_color, int thickness, int scalex, int scaley);

/**
 * @brief Crée une nouvelle ellipse.
 *
 * @param x Coordonnée X du centre.
 * @param y Coordonnée Y du centre.
 * @param radiusx Rayon horizontal.
 * @param radiusy Rayon vertical.
 * @param stroke_color Couleur du contour.
 * @param fill_color Couleur de remplissage.
 * @param thickness Épaisseur du contour.
 * @param scalex Facteur de mise à l'échelle horizontal.
 * @param scaley Facteur de mise à l'échelle vertical.
 * @return Pointeur vers l’ellipse créée, ou NULL en cas d’erreur.
 */
Ellipse* create_ellipse(int x, int y, int radiusx, int radiusy, const char* stroke_color, const char* fill_color, int thickness, int scalex, int scaley);

/**
 * @brief Crée une nouvelle ligne.
 *
 * @param x1 Coordonnée X du point de départ.
 * @param y1 Coordonnée Y du point de départ.
 * @param x2 Coordonnée X du point d'arrivée.
 * @param y2 Coordonnée Y du point d'arrivée.
 * @param stroke_color Couleur du trait.
 * @param thickness Épaisseur du trait.
 * @param scalex Facteur de mise à l'échelle horizontal.
 * @param scaley Facteur de mise à l'échelle vertical.
 * @return Pointeur vers la ligne créée, ou NULL en cas d’erreur.
 */
Line* create_line(int x1, int y1, int x2, int y2, const char* stroke_color, int thickness, int scalex, int scaley);

/**
 * @brief Crée un nouveau rectangle.
 *
 * @param x Coordonnée X du coin supérieur gauche.
 * @param y Coordonnée Y du coin supérieur gauche.
 * @param width Largeur (doit être > 0).
 * @param height Hauteur (doit être > 0).
 * @param stroke_color Couleur du contour.
 * @param fill_color Couleur de remplissage.
 * @param thickness Épaisseur du contour.
 * @param rounded Coins arrondis.
 * @param scalex Facteur de mise à l'échelle horizontal.
 * @param scaley Facteur de mise à l'échelle vertical.
 * @return Pointeur vers le rectangle créé, ou NULL en cas d’erreur.
 */
Rectangle* create_rectangle(int x, int y, int width, int height, const char* stroke_color, const char* fill_color, int thickness, int rounded, int scalex, int scaley);

/**
 * @brief Crée un nouveau polygone.
 *
 * @param points Tableau des points du polygone.
 * @param nb_points Nombre de points (au moins 3).
 * @param stroke_color Couleur du contour.
 * @param fill_color Couleur de remplissage.
 * @param thickness Épaisseur du contour.
 * @param scalex Facteur de mise à l'échelle horizontal.
 * @param scaley Facteur de mise à l'échelle vertical.
 * @return Pointeur vers le polygone créé, ou NULL en cas d’erreur.
 */
Polygon* create_polygon(Point* points, int nb_points, const char* stroke_color, const char* fill_color, int thickness, int scalex, int scaley);

/**
 * @brief Crée une nouvelle polyline (multiligne).
 *
 * @param points Tableau des points de la ligne.
 * @param nb_points Nombre de points (au moins 2).
 * @param stroke_color Couleur du trait.
 * @param thickness Épaisseur du trait.
 * @param scalex Facteur de mise à l'échelle horizontal.
 * @param scaley Facteur de mise à l'échelle vertical.
 * @return Pointeur vers la polyline créée, ou NULL en cas d’erreur.
 */
Multiline* create_multiline(Point* points, int nb_points, const char* stroke_color, int thickness, int scalex, int scaley);

/**
 * @brief Crée un nouveau chemin (Path) composé de PathPoints.
 *
 * @param points Tableau de PathPoints.
 * @param nb_points Nombre de points (au moins 2).
 * @param stroke_color Couleur du contour.
 * @param fill_color Couleur de remplissage.
 * @param thickness Épaisseur du contour.
 * @param closed Indique si le chemin est fermé (1 = fermé, 0 = ouvert).
 * @param scalex Facteur de mise à l'échelle horizontal.
 * @param scaley Facteur de mise à l'échelle vertical.
 * @return Pointeur vers le chemin créé, ou NULL en cas d’erreur.
 */
Path* create_path(PathPoint* points, int nb_points, const char* stroke_color, const char* fill_color, int thickness, int closed, int scalex, int scaley);

#endif // SHAPES_H