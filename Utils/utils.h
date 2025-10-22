/**
 * @file utils.h
 * @brief Fonctions utilitaires pour la gestion du terminal, de la saisie utilisateur et de l’affichage coloré.
 * 
 * Ce module regroupe diverses fonctions d’aide utilisées dans le programme, notamment :
 * - la manipulation du terminal (curseur, mode canonique, affichage lent),
 * - la validation et saisie des entrées utilisateur,
 * - la gestion des couleurs ANSI et des messages temporisés.
 * 
 * Il définit également des macros pour l’affichage coloré dans le terminal
 * et plusieurs fonctions de confort pour le rendu et l’interaction utilisateur.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "structures.h"

// ============================================================================
// 🎨 COULEURS ANSI
// ============================================================================
/**
 * @name Couleurs ANSI
 * @brief Macros pour afficher du texte coloré et stylisé dans le terminal.
 * @{
 */
#define RED     "\033[31m"
#define CORAL   "\033[38;2;255;80;80m"
#define GREEN   "\033[32m"
#define BLUE    "\033[38;2;4;140;224m"
#define SEA     "\033[38;2;96;144;247m"
#define YELLOW  "\033[38;2;255;213;0m"
#define ORANGE  "\033[38;2;255;165;0m"
#define PURPLE  "\033[38;2;128;0;128m"
#define PINK    "\033[38;2;255;192;203m"
#define BROWN   "\033[38;2;165;42;42m"
#define GRAY    "\033[30m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[97m"
#define BOLD    "\033[1m"

#define BG_BLUE      "\033[44m"
#define BG_LIGHTBLUE "\033[104m"
#define BG_DARKBLUE  "\033[48;5;18m"
#define BG_CYAN      "\033[48;5;44m"
#define BG_RESET     "\033[49m"
#define RESET        "\033[0m"
#define FOREST       "\033[38;2;34;139;34m"
#define BLINK        "\033[5m"
/** @} */

// ============================================================================
// 🎨 TABLEAU DES COULEURS STANDARD
// ============================================================================
/**
 * @brief Tableau des couleurs SVG standard acceptées par le programme.
 */
extern const char* standard_color[];

/**
 * @brief Nombre total de couleurs standard dans le tableau `standard_color`.
 */
extern const int nb_standard_color;

// ============================================================================
// 🔧 FONCTIONS UTILITAIRES GÉNÉRALES
// ============================================================================

/**
 * @brief Vide le buffer d’entrée standard (`stdin`).
 * 
 * Supprime les caractères résiduels laissés après une saisie incorrecte,
 * évitant que des retours à la ligne non consommés perturbent la saisie suivante.
 */
void empty_buffer();

/**
 * @brief Retourne le nom lisible d’un type de forme.
 * @param type Type de la forme (`ShapeType`)
 * @return Chaîne représentant le nom de la forme (ex : `"Circle"`, `"Rectangle"`, …)
 */
const char* get_shape_type(ShapeType type);

/**
 * @brief Vérifie si une chaîne correspond à un code couleur hexadécimal valide (#RRGGBB).
 * @param s Chaîne à vérifier.
 * @return `true` si le code est valide, `false` sinon.
 */
bool is_hex(const char* s);

/**
 * @brief Vérifie si une chaîne correspond à une couleur SVG standard connue.
 * @param s Chaîne à vérifier.
 * @return `true` si la couleur est dans la liste `standard_color`, `false` sinon.
 */
bool is_standard_color(const char* s);

/**
 * @brief Vérifie si une couleur est valide pour le programme.
 * 
 * Une couleur est considérée valide si elle est :
 * - un code hexadécimal (#RRGGBB),
 * - ou une couleur standard connue.
 * 
 * @param s Chaîne représentant la couleur.
 * @return `true` si la couleur est valide, `false` sinon.
 */
bool valid_color(const char* s);

/**
 * @brief Affiche temporairement un message dans le terminal, puis l’efface après un délai donné.
 * 
 * @param msg Message à afficher.
 * @param n Nombre de lignes à effacer après le délai.
 * @param seconds Durée d’affichage en secondes.
 */
void temp_message(const char* msg, int n, int seconds);

/**
 * @brief Active le mode “non canonique” du terminal (bloque la validation par Entrée).
 * 
 * Permet de lire les caractères en direct, sans attendre la touche Entrée,
 * et désactive également l’écho des touches.
 */
void block_enter();

/**
 * @brief Restaure le mode normal du terminal après `block_enter`.
 */
void unblock_enter();

/**
 * @brief Affiche du texte caractère par caractère avec un délai.
 * 
 * Utile pour des effets d’affichage animés.
 * 
 * @param text Texte à afficher.
 * @param delay_us Délai entre chaque caractère (en microsecondes).
 */
void slow_print(const char *text, int delay_us);

/**
 * @brief Déplace le curseur du terminal à une position donnée.
 * 
 * @param row Ligne cible (commence à 1).
 * @param col Colonne cible (commence à 1).
 */
void move_cursor(int row, int col);

// ============================================================================
// ⌨️ SAISIES UTILISATEUR
// ============================================================================

/**
 * @brief Lit un entier saisi par l’utilisateur avec contrôle précis.
 * 
 * Permet la saisie directe (sans validation par Entrée) et gère :
 * - le retour arrière (⌫),
 * - les bornes numériques,
 * - les annulations.
 * 
 * @param result Pointeur vers la variable où stocker le résultat.
 * @param prompt Message d’invite.
 * @param max_digits Nombre maximum de chiffres autorisés.
 * @param min_digit_value Valeur minimale d’un chiffre.
 * @param max_digit_value Valeur maximale d’un chiffre.
 * @param allow_negative Indique si les nombres négatifs sont autorisés.
 * 
 * @return Code d’état (`INPUT_SUCCESS`, `INPUT_CANCEL`, `INPUT_INVALID`).
 */
InputStatus get_integer(int *result, const char *prompt, int max_digits, int min_digit_value, int max_digit_value, bool allow_negative);

/**
 * @brief Gère la saisie d’une couleur utilisateur (hexadécimale ou standard).
 * 
 * @param result Chaîne où sera stockée la couleur saisie.
 * @param prompt Message d’invite à afficher.
 * 
 * @return Code d’état (`INPUT_SUCCESS`, `INPUT_CANCEL`, `INPUT_INVALID`).
 */
InputStatus get_color(char* result, const char* prompt);

/**
 * @brief Gère les erreurs ou annulations pendant la création d’un élément.
 * 
 * Affiche un message de confirmation ou d’erreur et détermine
 * si la création doit être annulée.
 * 
 * @param status Statut d’entrée retourné par une fonction précédente.
 * @return `true` si la création est annulée, `false` sinon.
 */
bool creation_error(InputStatus status);

/**
 * @brief Attend une confirmation utilisateur sous la forme d’un caractère (`Y`/`N`).
 * 
 * @param result Pointeur où stocker le caractère (`Y`, `N`, `y`, `n`).
 * @param prompt Message d’invite.
 * @return Code d’état (`INPUT_SUCCESS` ou `INPUT_CANCEL`).
 */
InputStatus get_confirmation_char(char *result, const char *prompt);

// ============================================================================
// 📊 AFFICHAGE VISUEL
// ============================================================================

/**
 * @brief Dessine une barre de progression animée dans le terminal.
 * 
 * @param fraction Fraction d’avancement (entre `0.0` et `1.0`).
 */
void draw_progress(double fraction);

#endif