/**
 * @file export.h
 * @brief Déclaration des fonctions d'exportation des formes en format SVG.
 * @author Adam
 * @date 8 Octobre 2025
 */

#ifndef EXPORT_H
#define EXPORT_H

#include <stdio.h>

#include "structures.h"

/**
 * @brief Exporte toutes les formes dans un fichier SVG.
 * @param filename Nom du fichier de sortie (ex : "export.svg").
 * @param shapes Tableau de formes stockées.
 * @param nb_shapes Nombre total de formes à exporter.
 */
void export_svg_all(const char* filename, ShapeList* list);

#endif