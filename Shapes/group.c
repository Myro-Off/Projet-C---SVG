#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

#include "group.h"
#include "memory.h"
#include "utils.h"
#include "list.h"
#include "structures.h"

Group* create_group(void) {
    Group* g = malloc(sizeof(Group));
    if (!g) return NULL;

    g->shapes = malloc(sizeof(ShapeList));
    if (!g->shapes) {
        free(g);
        return NULL;
    }

    init_shape_list(g->shapes);
    g->name = NULL;
    return g;
}

bool is_in_group(const Group* group, const ShapeNode* node) {
    if (!group || !group->shapes || !node) return false;

    ShapeNode* current = group->shapes->head;
    while (current) {
        if (current->data.ptr == node->data.ptr)
            return true;
        current = current->next;
    }
    return false;
}

void add_to_group(Group* group, ShapeNode* node) {
    if (!group || !node) return;

    if (is_in_group(group, node)) {
        temp_message(ORANGE"\n╭─────────────────────────────╮\n"
                             "│ Shape already in the group! │\n"
                             "╰─────────────────────────────╯\n"RESET, 4, 2);
        return;
    }

    ShapeData d = node->data;
    add_shape(group->shapes, node->type, d);
}

bool remove_shape_from_group(Group* group, int sub_index) {
    if (!group || !group->shapes || sub_index < 0 || sub_index >= group->shapes->size)
        return false;

    ShapeNode* current = group->shapes->head;
    for (int i = 0; i < sub_index; i++)
        current = current->next;

    if (!current) return false;

    if (current->prev)
        current->prev->next = current->next;
    else
        group->shapes->head = current->next;

    if (current->next)
        current->next->prev = current->prev;
    else
        group->shapes->tail = current->prev;

    free(current);
    group->shapes->size--;

    temp_message(GREEN"\n╭────────────────────────────────────────╮\n"
                        "│ Shape removed from group successfully! │\n"
                        "╰────────────────────────────────────────╯\n"RESET, 0, 2);
    return true;
}

void remove_shape_from_all_groups(ShapeList* list, const ShapeNode* node) {
    if (!list || !node) return;

    ShapeNode* current = list->head;
    while (current) {
        if (current->type == GROUP && current->data.group && current->data.group->shapes) {
            ShapeList* subgroup = current->data.group->shapes;
            ShapeNode* sub = subgroup->head;
            while (sub) {
                ShapeNode* next = sub->next;
                if (sub->data.ptr == node->data.ptr) {
                    if (sub->prev)
                        sub->prev->next = sub->next;
                    else
                        subgroup->head = sub->next;

                    if (sub->next)
                        sub->next->prev = sub->prev;
                    else
                        subgroup->tail = sub->prev;

                    free(sub);
                    subgroup->size--;
                }
                sub = next;
            }
        }
        current = current->next;
    }
}

// ------------------------------------------------------------
// FREE MEMORY
// ------------------------------------------------------------
void free_group_shape_list(ShapeList* list) {
    if (!list) return;

    ShapeNode* current = list->head;
    while (current) {
        ShapeNode* next = current->next;
        free(current);
        current = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;
}

void free_group(Group* group) {
    if (!group) return;

    free_group_shape_list(group->shapes);
    free(group->shapes);

    if (group->name) free((char*)group->name);
    free(group);
}

InputStatus get_group_name(char* result, const char* prompt) {
    int c, index = 0;
    const int MAX_LEN = 12;

    printf("%s", prompt);
    fflush(stdout);
    block_enter();

    while (1) {
        c = getchar();

        if (c == '\n' || c == EOF) {
            unblock_enter();
            if (index == 0) {
                move_cursor(4 ,32); printf(ORANGE"╭────────────────────╮");
                move_cursor(5 ,30); printf(    "->│ Creation cancelled │");
                move_cursor(6 ,32); printf(      "╰────────────────────╯");
                temp_message("", 0, 1);
                return INPUT_CANCEL;
            }
            result[index] = '\0';
            printf("\n");
            return INPUT_SUCCESS;
        }

        if (c == 8 || c == 127) {
            if (index > 0) {
                index--;
                result[index] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }

        if (isprint(c)) {
            if (index < MAX_LEN) {
                result[index++] = (char)c;
                putchar(c);
                fflush(stdout);
            } else {
                printf("\a");
                fflush(stdout);
            }
        }
    }
}

void create_group_menu(ShapeList* list) {
    if (!list || list->size == 0) {
        temp_message(ORANGE"╭───────────────────────────────────────╮\n"
                           "│ No shapes available to create a group │\n"
                           "╰───────────────────────────────────────╯\n"RESET, 4, 2);
        return;
    }

    system("clear");
    printf(PINK"╔══════════════════════════════════════════════╗\n"
               "║                 CREATE GROUP                 ║\n"
               "╚══════════════════════════════════════════════╝\n");

    char name[32];
    printf("╭────────────┬──────────────╮\n"
           "│ Group Name │              │\n"
           "╰────────────┴──────────────╯");
    move_cursor(5, 16);
    if (get_group_name(name, "") != INPUT_SUCCESS) return;

    Group* g = create_group();
    if (!g) return;
    g->name = strdup(name);

    bool* selected = calloc(list->size, sizeof(bool));
    if (!selected) { free_group(g); return; }

    int cursor = 0;
    int column = 0; // 0 = gauche, 1 = droite
    char c;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int half = (list->size + 1) / 2;

    while (1) {
        system("clear");
        printf(PINK"╔══════════════════════════════════════════════╗\n"
                   "║                 CREATE GROUP                 ║\n"
                   "╚══════════════════════════════════════════════╝\n");
        printf("╭──────────────────────────────────────────────╮\n"
               "│"RESET"    Use ↑/↓ to move, ←/→ to change column,    "PINK"│\n"
               "│"RESET"        SPACE toggle, Enter to finish         "PINK"│\n"
               "╰──────────────────────────────────────────────╯\n"RESET);

        ShapeNode* node_left = list->head;
        ShapeNode* node_right = list->head;
        for (int i = 0; i < half && node_right; i++) node_right = node_right->next ? node_right->next : node_right;

        for (int i = 0; i < half; i++) {
            // gauche
            node_left = get_node_at(list, i);
            const char* left_type = get_shape_type(node_left->type);
            const char* left_name = (node_left->type == GROUP && node_left->data.group) ? node_left->data.group->name : "";
            const char* left_checkbox = selected[i] ? GREEN"[x]" : RESET"[ ]";

            // droite
            node_right = get_node_at(list, i + half);
            const char* right_type = node_right ? get_shape_type(node_right->type) : "";
            const char* right_name = (node_right && node_right->type == GROUP && node_right->data.group) ? node_right->data.group->name : "";
            const char* right_checkbox = (node_right && selected[i + half]) ? GREEN"[x]" : RESET"[ ]";

            // curseur pour deux colonnes
            bool cursor_left = (cursor == i && column == 0);
            bool cursor_right = (cursor == i && column == 1);

            if (cursor_left)
                printf(PINK">"RESET" %s %2d - %s %s", left_checkbox, i, left_type, left_name);
            else
                printf("  %s %2d - %s %s", left_checkbox, i, left_type, left_name);

            if (node_right) {
                printf(" ");
                if (cursor_right)
                    printf(PINK">"RESET" %s %2d - %s %s", right_checkbox, i + half, right_type, right_name);
                else
                    printf("  %s %2d - %s %s", right_checkbox, i + half, right_type, right_name);
            }
            printf("\n");
        }

        c = getchar();
        if (c == '\033') {
            getchar(); // skip '['
            switch(getchar()) {
                case 'A': cursor = (cursor - 1 + half) % half; break; // up
                case 'B': cursor = (cursor + 1) % half; break;        // down
                case 'C': column = 1; break;                          // right
                case 'D': column = 0; break;                          // left
            }
        } else if (c == ' ') {
            int idx = column == 0 ? cursor : cursor + half;
            selected[idx] = !selected[idx];
        } else if (c == '\n') {
            break;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    for (int i = 0; i < list->size; i++) {
        if (selected[i]) add_to_group(g, get_node_at(list, i));
    }

    free(selected);

    if (g->shapes->size == 0) {
        temp_message(ORANGE"╭─────────────────────────────────────╮\n"
                           "│ Nothing selected, group not created │\n"
                           "╰─────────────────────────────────────╯\n"RESET, 0, 2);
        free_group(g);
        return;
    }

    ShapeData d = {.group = g};
    add_shape(list, GROUP, d);

    temp_message(GREEN"╭─────────────────────────────╮\n"
                      "│ Group created successfully! │\n"
                      "╰─────────────────────────────╯\n"RESET, 0, 2);
}