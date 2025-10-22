#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "utils.h"
#include "structures.h"

struct termios oldt, newt;

const char* standard_color[] = {
    "black", "white", "red", "green", "blue",
    "yellow", "orange", "purple", "pink", "brown",
    "gray", "cyan", "magenta", "none"
};
const int nb_standard_color = sizeof(standard_color) / sizeof(standard_color[0]);

const char* get_shape_type(ShapeType type) {
    switch (type) {
        case CIRCLE:    return "Circle";
        case SQUARE:    return "Square";
        case RECTANGLE: return "Rectangle";
        case LINE:      return "Line";
        case ELLIPSE:   return "Ellipse";
        case POLYGON:   return "Polygon";
        case MULTILINE: return "Multiline";
        case GROUP:     return "Group";
        default:        return "Unknown";
    }
}

void empty_buffer() {
    tcflush(STDIN_FILENO, TCIFLUSH);
}

bool is_hex(const char* s) {
    if (!s || strlen(s) != 7 || s[0] != '#')
        return false;
    for (int i = 1; i < 7; i++) {
        if (!isxdigit(s[i])) {
            return false;
        }
    }
    return true;
}

bool is_standard_color(const char* s) {
    for (int i = 0; i < nb_standard_color; i++) {
        if (strcasecmp(s, standard_color[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool valid_color(const char* s) {
    return is_hex(s) || is_standard_color(s);
}

void temp_message(const char* msg, int n, int seconds) {
    // Sauvegarde des paramètres du terminal
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Désactive saisie canonique et echo pour bloquer l'entrée
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Affiche le msg
    printf("%s", msg);
    fflush(stdout);

    // Attendre le temps voulu
    sleep(seconds);

    // Effacer les lignes du msg
    for (int i = 0; i < n; i++) {
        printf("\x1b[1F\x1b[2K");
    }
    fflush(stdout);

    tcflush(STDIN_FILENO, TCIFLUSH);
    
    // Rétablit les paramètres du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void block_enter() {
    // Sauvegarde de la configuration actuelle du terminal
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Désactive la saisie canonique (entrée ligne par ligne)
    // et l’écho (affichage des caractères tapés)
    newt.c_lflag &= ~(ICANON | ECHO);

    // Applique immédiatement la nouvelle configuration
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void unblock_enter() {
    // Vide le tampon d’entrée (empêche toute touche tapée d’être conservée)
    tcflush(STDIN_FILENO, TCIFLUSH);
    // Restaure la configuration d’origine
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // Vide le tampon d’entrée (empêche toute touche tapée d’être conservée)
    tcflush(STDIN_FILENO, TCIFLUSH);
}

// ------------------------------------------------------------
// 🔹 Affichage animé du texte
// ------------------------------------------------------------
void slow_print(const char *text, int delay_us) {
    for (const char *p = text; *p; p++) {
        printf("%c", *p);
        fflush(stdout);
        usleep(delay_us);
    }
}

void move_cursor(int row, int col) {
    printf("\x1b[%d;%dH", row, col);
}

InputStatus get_integer(
    int *result,
    const char *prompt,
    int max_digits,
    int min_digit_value,
    int max_digit_value,
    bool allow_negative
) {
    char buffer[34] = {0}; 
    int index = 0;           
    int digit_count = 0;     
    int c;
    
    // Omettre la validation des bornes ici pour rester concentré sur le bug Entrée/Annulation.
    printf("%s", prompt);
    fflush(stdout);

    block_enter(); 
    
    while (1) {
        c = getchar(); 
        
        int digit_value = c - '0';

        // 1. Gestion de la Validation (Entrée)
        if (c == '\n' || c == EOF) { 
            unblock_enter();
            
            if (index == 0) {
                return INPUT_CANCEL; 
            }
            
            // L'utilisateur a saisi uniquement le signe moins ("-") et a validé.
            if (digit_count == 0 && allow_negative && buffer[0] == '-') {
                return INPUT_INVALID; 
            }

            break; 
        }
        
        // 2. Gestion de la Suppression (Backspace ou Delete)
        if (c == 8 || c == 127) { 
            if (index > 0) {
                if (isdigit(buffer[index - 1])) digit_count--;
                index--; 
                buffer[index] = '\0';
                printf("\b \b"); 
                fflush(stdout);
            }
            continue; 
        }

        // 3. Gestion de la Saisie (Chiffres et Signe Moins)
        if (isdigit(c) && digit_value >= min_digit_value && digit_value <= max_digit_value) {
            if (digit_count < max_digits && index < 34 - 1) { 
                buffer[index++] = (char)c;
                digit_count++;
                putchar(c);
                fflush(stdout);
            }
        }
        else if (c == '-') {
            if (index == 0 && allow_negative) {
                buffer[index++] = (char)c;
                putchar(c);
                fflush(stdout);
            }
        }
    }
    printf("\n");
    // 4. Conversion finale (et vérification de dépassement)
    char *endptr;
    long val = strtol(buffer, &endptr, 10);

    *result = (int)val;
    return INPUT_SUCCESS;
}

InputStatus get_color(char* result, const char* prompt) {
    int c;
    int index = 0;
    const int MAX_LEN = 7;

    printf("%s", prompt);
    fflush(stdout);

    block_enter();

    while (1) {
        c = getchar();

        // --- Validation (Entrée) ---
        if (c == '\n' || c == EOF) {
            unblock_enter();

            if (index == 0) {
                return INPUT_CANCEL;
            }

            result[index] = '\0';

            if (valid_color(result)) {
                printf("\n");
                return INPUT_SUCCESS;
            }

            printf("\n");
            temp_message(RED"╭──────────────────────────────────────────────────────────────╮\n"
                            "│ Invalid color! Use hexadecimal (#"BOLD RED"RR"GREEN"GG"BLUE"BB"RESET RED") or a standard color │\n"
                            "│                                                              │\n"
                            "│ "BOLD RED"red  "GREEN"green  "BLUE"blue  "YELLOW"yellow  "ORANGE"orange  "PURPLE"purple  "PINK"pink"RESET RED"               │\n"
                            "│ "BOLD BROWN"brown  "GRAY"gray  "CYAN"cyan  "MAGENTA"magenta  "RESET BOLD"none "RESET"(transparent)"RED"               │\n"
                            "╰──────────────────────────────────────────────────────────────╯\n"RESET, 7, 2);

            // relancer la saisie
            printf("%s", prompt);
            fflush(stdout);
            index = 0;
            result[0] = '\0';
            block_enter();
            continue;
        }

        // --- Suppression (Backspace/Delete) ---
        if (c == 8 || c == 127) {
            if (index > 0) {
                index--;
                result[index] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }

        // --- Saisie normale ---
        if (isprint(c)) {
            if (index < MAX_LEN) {
                result[index++] = (char)c;
                putchar(c);
                fflush(stdout);
            } else {
                printf("\a"); // bloqué à 7 caractères
                fflush(stdout);
            }
        }
    }
}

bool creation_error(InputStatus status) { 
    if (status == INPUT_INVALID) {
        temp_message(RED"\n╭───────────────────────────────────╮\n"
                          "│ Please enter an integer after \"-\" │\n"
                          "╰───────────────────────────────────╯\n"RESET, 4, 2);
        return false;
    }
    
    if (status == INPUT_CANCEL) {
        char choice = '\0';
        printf(YELLOW"\n╭────────────────────────╮  ╭───╮\n"
                       "│ "RESET"Confirm Cancel ? (Y/N) "YELLOW"│->│   │\n"
                 YELLOW"╰────────────────────────╯  ╰───╯\033[1A\033[3D"RESET);
    
        fflush(stdout);
        InputStatus confirm_status = get_confirmation_char(&choice, "");
        if (confirm_status == INPUT_CANCEL || toupper(choice) == 'Y') {
            temp_message (ORANGE"\n\n╭───────────────────╮\n"
                                    "│ Creation Canceled │\n"
                                    "╰───────────────────╯", 0, 1);
            return true;
        } else {
            temp_message("\033[1B", 4, 0);
            return false;
        }
    }
    return false;
}

InputStatus get_confirmation_char(char *result, const char *prompt) {
    char buffer[2] = {0};
    int index = 0; // Index dans le buffer (0 ou 1)
    int c;
    
    printf("%s", prompt);
    fflush(stdout);
    
    block_enter(); 
    
    while (1) {
        c = getchar();
        
        // 1. Gestion de la Validation (Entrée)
        if (c == '\n' || c == EOF) {
            unblock_enter();
            
            // Si le buffer est vide (rien n'a été tapé avant Entrée), ne rien faire et retourner CANCEL
            if (index == 0) {
                // Pas de validation, pas de saut de ligne, juste retourner l'annulation
                return INPUT_CANCEL; 
            }
            
            // Validation réussie : un caractère Y/N est dans le buffer.
            printf("\n"); // Ajouter le saut de ligne de validation
            *result = buffer[0];
            return INPUT_SUCCESS;
        }

        // 2. Gestion du Retour Arrière (Backspace/Delete)
        if (c == 8 || c == 127) {
            if (index > 0) {
                index--; // Reculer (supprimer l'élément)
                buffer[index] = '\0';
                
                // Effacer l'affichage (reculer, espace, reculer)
                printf("\b \b"); 
                fflush(stdout);
            }
            continue;
        }

        // 3. Gestion de la Saisie (Y, y, N, n)
        // N'autoriser qu'un seul caractère à la fois (index == 0)
        if (index == 0 && (c == 'Y' || c == 'y' || c == 'N' || c == 'n')) {
            buffer[index++] = (char)c;
            
            // Afficher le caractère tapé
            putchar(c);
            fflush(stdout);
        }
        
        // Tous les autres caractères (et le deuxième caractère s'il est tapé) sont ignorés.
    }
}

#define BAR_WIDTH 20                // nombre total de blocs affichés

/* Dessine la barre sur la même ligne */
void draw_progress(double fraction)
{
    int filled = (int)(fraction * BAR_WIDTH);   // blocs pleins
    int empty  = BAR_WIDTH - filled;            // blocs vides

    /* Retour au début de la ligne et ré‑écriture du texte fixe */
    move_cursor(20, 39);
    printf("Exporting... ");

    /* ---- blocs pleins (U+2588 = █) ---- */
    for (int i = 0; i < filled; ++i) {
        /* On écrit les trois octets UTF‑8 un par un.
         * Les littéraux sont de type unsigned char, donc aucun shift n’est nécessaire. */
        putchar(0xE2);
        putchar(0x96);
        putchar(0x88);
    }

    /* ---- blocs vides (U+2591 = ░) ---- */
    for (int i = 0; i < empty; ++i) {
        putchar(0xE2);
        putchar(0x96);
        putchar(0x91);
    }

    /* Pourcentage */
    printf(" %3.0f%%", fraction * 100.0);

    fflush(stdout);   // affichage immédiat
}