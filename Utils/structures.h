/**
 * @file structures.h
 * @brief Définitions des structures de données principales utilisées pour représenter les formes SVG.
 *
 * Ce fichier contient toutes les structures, énumérations et unions nécessaires
 * à la représentation, la manipulation et la gestion des formes graphiques dans le programme :
 * cercles, rectangles, polygones, groupes, chemins (Path), etc.
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

/* === Déclarations anticipées === */
typedef struct ShapeNode ShapeNode;
typedef struct ShapeList ShapeList;
typedef struct Group Group;

/** ============================================================
 *                     FORMES SIMPLES
 *  ============================================================ */

/**
 * @struct Square
 * @brief Représente un carré.
 */
typedef struct {
    int x, y;                   /**< Coordonnées du coin supérieur gauche. */
    int side;                   /**< Taille d’un côté du carré. */
    char stroke_color[16];      /**< Couleur du contour (ex : "#FF0000" ou "red"). */
    char fill_color[16];        /**< Couleur de remplissage. */
    int thickness;              /**< Épaisseur du contour. */
    int rounded;                /**< Rayon d’arrondi des coins (0 = non arrondi). */
    int scalex, scaley;         /**< Facteurs d’échelle horizontale et verticale. */
} Square;

/**
 * @struct Circle
 * @brief Représente un cercle.
 */
typedef struct {
    int x, y;                   /**< Coordonnées du centre. */
    int radius;                 /**< Rayon du cercle. */
    char stroke_color[16];      /**< Couleur du contour. */
    char fill_color[16];        /**< Couleur de remplissage. */
    int thickness;              /**< Épaisseur du contour. */
    int scalex, scaley;         /**< Facteurs d’échelle horizontale et verticale. */
} Circle;

/**
 * @struct Ellipse
 * @brief Représente une ellipse.
 */
typedef struct {
    int x, y;                   /**< Coordonnées du centre. */
    int radiusx, radiusy;       /**< Rayons horizontal et vertical. */
    char stroke_color[16];      /**< Couleur du contour. */
    char fill_color[16];        /**< Couleur de remplissage. */
    int thickness;              /**< Épaisseur du contour. */
    int scalex, scaley;         /**< Facteurs d’échelle horizontale et verticale. */
} Ellipse;

/**
 * @struct Line
 * @brief Représente un segment de droite.
 */
typedef struct {
    int x1, y1;                 /**< Coordonnées du point de départ. */
    int x2, y2;                 /**< Coordonnées du point d’arrivée. */
    char stroke_color[16];      /**< Couleur du trait. */
    int thickness;              /**< Épaisseur du trait. */
    int scalex, scaley;         /**< Facteurs d’échelle horizontale et verticale. */
} Line;

/**
 * @struct Rectangle
 * @brief Représente un rectangle.
 */
typedef struct {
    int x, y;                   /**< Coordonnées du coin supérieur gauche. */
    int width, height;          /**< Largeur et hauteur du rectangle. */
    char stroke_color[16];      /**< Couleur du contour. */
    char fill_color[16];        /**< Couleur de remplissage. */
    int thickness;              /**< Épaisseur du contour. */
    int rounded;                /**< Rayon d’arrondi des coins. */
    int scalex, scaley;         /**< Facteurs d’échelle horizontale et verticale. */
} Rectangle;

/** ============================================================
 *                     STRUCTURES COMPLEXES
 *  ============================================================ */

/**
 * @struct Point
 * @brief Coordonnée 2D (x, y).
 */
typedef struct {
    int x; /**< Coordonnée X. */
    int y; /**< Coordonnée Y. */
} Point;

/**
 * @struct Polygon
 * @brief Représente un polygone avec plusieurs points.
 */
typedef struct {
    Point* points;              /**< Tableau dynamique de points. */
    int nb_points;              /**< Nombre total de points. */
    char stroke_color[16];      /**< Couleur du contour. */
    char fill_color[16];        /**< Couleur de remplissage. */
    int thickness;              /**< Épaisseur du contour. */
    int scalex, scaley;         /**< Facteurs d’échelle horizontale et verticale. */
} Polygon;

/**
 * @struct Multiline
 * @brief Représente une série de segments connectés.
 */
typedef struct {
    Point* points;              /**< Tableau dynamique de points. */
    int nb_points;              /**< Nombre total de points. */
    char stroke_color[16];      /**< Couleur du trait. */
    int thickness;              /**< Épaisseur du trait. */
    int scalex, scaley;         /**< Facteurs d’échelle horizontale et verticale. */
} Multiline;

/** ============================================================
 *                     CHEMINS SVG (PATH)
 *  ============================================================ */

/**
 * @enum PathCoordinateType
 * @brief Type de coordonnées utilisées dans les commandes SVG.
 */
typedef enum {
    COORD_ABSOLUTE, /**< Commandes majuscules (M, L, C, etc.). */
    COORD_RELATIVE  /**< Commandes minuscules (m, l, c, etc.). */
} PathCoordinateType;

/**
 * @enum PathSegmentType
 * @brief Type de segment composant un chemin SVG.
 */
typedef enum {
    SEGMENT_MOVE,              /**< M/m : Déplacement du curseur. */
    SEGMENT_LINE,              /**< L/l : Ligne droite. */
    SEGMENT_HORIZONTAL,        /**< H/h : Ligne horizontale. */
    SEGMENT_VERTICAL,          /**< V/v : Ligne verticale. */
    SEGMENT_QUADRATIC_BEZIER,  /**< Q/q : Courbe quadratique. */
    SEGMENT_CUBIC_BEZIER,      /**< C/c : Courbe cubique. */
    SEGMENT_SMOOTH_QUADRATIC,  /**< T/t : Courbe quadratique lisse. */
    SEGMENT_SMOOTH_CUBIC,      /**< S/s : Courbe cubique lisse. */
    SEGMENT_ARC,               /**< A/a : Arc elliptique. */
    SEGMENT_CLOSE              /**< Z/z : Fermeture du chemin. */
} PathSegmentType;

/**
 * @struct PathPoint
 * @brief Représente un point ou une commande dans un chemin SVG.
 */
typedef struct {
    int x, y;                          /**< Coordonnées principales. */
    PathSegmentType type;              /**< Type de segment (ligne, courbe, etc.). */
    PathCoordinateType coord_type;     /**< Type de coordonnées (absolu ou relatif). */

    Point control1;                    /**< Premier point de contrôle (pour Q/q, C/c, S/s). */
    Point control2;                    /**< Deuxième point de contrôle (pour C/c). */

    int radiusX, radiusY;              /**< Rayons pour les arcs (A/a). */
    int xAxisRotation;                 /**< Rotation de l’axe de l’ellipse. */
    bool largeArcFlag;                 /**< Indique si l’arc est grand (0 ou 1). */
    bool sweepFlag;                    /**< Indique le sens du tracé (0 ou 1). */
} PathPoint;

/**
 * @struct Path
 * @brief Représente un chemin SVG complet.
 */
typedef struct {
    PathPoint* points;         /**< Tableau dynamique de points et commandes. */
    int nb_points;             /**< Nombre total d’éléments. */
    char stroke_color[16];     /**< Couleur du contour. */
    char fill_color[16];       /**< Couleur de remplissage. */
    int thickness;             /**< Épaisseur du trait. */
    int closed;                /**< 1 si le chemin est fermé, 0 sinon. */
    int scalex, scaley;        /**< Facteurs d’échelle horizontale et verticale. */
} Path;

/** ============================================================
 *                     STRUCTURES DE GESTION
 *  ============================================================ */

/**
 * @enum ShapeType
 * @brief Type de forme gérée par le programme.
 */
typedef enum {
    CIRCLE, SQUARE, RECTANGLE, LINE,
    ELLIPSE, POLYGON, MULTILINE,
    PATH, GROUP
} ShapeType;

/**
 * @union ShapeData
 * @brief Contient un pointeur vers la structure spécifique d’une forme.
 */
typedef union {
    Circle* circle;
    Square* square;
    Rectangle* rectangle;
    Line* line;
    Ellipse* ellipse;
    Polygon* polygon;
    Multiline* multiline;
    Path* path;
    Group* group;
    void* ptr; /**< Accès générique. */
} ShapeData;

/**
 * @struct ShapeNode
 * @brief Élément d’une liste doublement chaînée contenant une forme.
 */
struct ShapeNode {
    ShapeType type;        /**< Type de la forme. */
    ShapeData data;        /**< Données spécifiques de la forme. */
    struct ShapeNode* next; /**< Pointeur vers le nœud suivant. */
    struct ShapeNode* prev; /**< Pointeur vers le nœud précédent. */
};

/**
 * @struct ShapeList
 * @brief Liste doublement chaînée de formes.
 */
struct ShapeList {
    ShapeNode* head; /**< Premier élément. */
    ShapeNode* tail; /**< Dernier élément. */
    int size;        /**< Nombre total d’éléments. */
};

/**
 * @struct Group
 * @brief Regroupe plusieurs formes dans une structure commune.
 */
struct Group {
    ShapeList* shapes; /**< Liste des formes contenues dans le groupe. */
    char* name;        /**< Nom du groupe. */
};

/** ============================================================
 *                     STATUTS ET BARRES
 *  ============================================================ */

/**
 * @enum InputStatus
 * @brief Représente l’état d’une saisie utilisateur.
 */
typedef enum {
    INPUT_SUCCESS, /**< Saisie réussie. */
    INPUT_CANCEL,  /**< Saisie annulée. */
    INPUT_INVALID  /**< Saisie invalide. */
} InputStatus;

/**
 * @struct Bar
 * @brief Représente une barre de progression.
 */
typedef struct {
    const char *label; /**< Texte descriptif de la barre. */
    int width;         /**< Largeur en caractères. */
    double progress;   /**< Niveau de progression (0.0 → 1.0). */
} Bar;

#endif