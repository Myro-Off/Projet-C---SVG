#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ctype.h>

#include "delete.h"
#include "list.h"
#include "memory.h"
#include "utils.h"
#include "structures.h"
#include "menu.h"

void delete_shape_or_group_menu(ShapeList* list) {
    if (!list || list->size == 0) {
        move_cursor(19, 37); printf(ORANGE"╭───────────────────────────────╮");
        move_cursor(20, 35); printf(    "->│ No shapes or groups to delete │");
        move_cursor(21, 37); printf(      "╰───────────────────────────────╯");
        move_cursor(20 + list->size, 32);
        temp_message(ORANGE"4", 0, 2);
        return;
    }

    int max_idx = list->size - 1;
    int idx;
    InputStatus status;
    
    display_list(list);

    if (list->size < 10) {
        printf(RED"╭─────────────────────────────────╮  ╭───╮\n"
                  "│ "RESET"Select an item (0->%d) to delete "RED"│->│   │\n"
               RED"╰─────────────────────────────────╯  ╰───╯", max_idx);
        move_cursor(11 + list->size , 40); 
        fflush(stdout);
        status = get_integer(&idx, "", 1, 0, max_idx, false);
    }
    else {
        printf(RED"╭──────────────────────────────────╮  ╭────╮\n"
                  "│ "RESET"Select an item (0->%d) to delete "RED"│->│    │\n"
               RED"╰──────────────────────────────────╯  ╰────╯", max_idx);
        move_cursor(11 + list->size , 40); 
        fflush(stdout);
        status = get_integer(&idx, "", 2, 0, max_idx, false);
    }

    if (status == INPUT_CANCEL) return;
    
    ShapeNode* node = get_node_at(list, idx);

    delete_shape_or_group(list, node, idx, list->size, false);
}

void delete_shape_or_group(ShapeList* list, ShapeNode* node, int idx, int details_lines, bool from_details) {
    if (!list || !node) return;

    char choice = '\0';
    printf(YELLOW"\n╭──────────────────────────╮  ╭───╮\n"
                   "│ "RESET"Confirm Deletion ? (Y/N) "YELLOW"│->│   │\n"
             YELLOW"╰──────────────────────────╯  ╰───╯\n");

    int lines = 0;

    if (from_details)
        lines = details_lines;
    else
        lines = list->size;
    
    move_cursor(14 + lines, 33);
    fflush(stdout);
    get_confirmation_char(&choice, "");
    if (toupper(choice) == 'Y') {
        delete_node(list, node);
        
        if (node->type == GROUP) {
            move_cursor(14 + lines, 38); printf( GREEN"╭─────────────────────────────╮");
            move_cursor(15 + lines, 36); printf(    "->│ Group successfully deleted! │");
            move_cursor(16 + lines, 38); printf(      "╰─────────────────────────────╯");
            move_cursor(15 + lines, 33);
            temp_message(GREEN"Y", 0, 2);
        } else {
            move_cursor(13 + lines, 38); printf( GREEN"╭─────────────────────────────╮");
            move_cursor(14 + lines, 36); printf(    "->│ Shape successfully deleted! │");
            move_cursor(15 + lines, 38); printf(      "╰─────────────────────────────╯");
            move_cursor(14 + lines, 33);
            temp_message(GREEN"Y", 0, 2);
        }
    }
    else {
        move_cursor(13 + lines, 38); printf( ORANGE"╭───────────────────╮");
        move_cursor(14 + lines, 36); printf(     "->│ Deletion Canceled │");
        move_cursor(15 + lines, 38); printf(       "╰───────────────────╯");
        move_cursor(14 + lines, 33);
        temp_message(ORANGE"N", 0, 2);
        if (from_details) {
            if (node->type == GROUP)
                display_group_details(node, node->data.group, idx, (ShapeList*)list);
            else
                display_shape_details(node, idx, list);
        }
        else
            delete_shape_or_group_menu(list);
    }
    if (list->size == 0) {
        display_introduction(false);
        return;
    }
    else return;
}