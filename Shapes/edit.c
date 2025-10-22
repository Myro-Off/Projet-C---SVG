#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "edit.h"
#include "list.h"
#include "utils.h"
#include "structures.h"
#include "memory.h"
#include "group.h"

static void return_options(ShapeNode* node, bool from_details) {
    if (from_details) {
        if (node->type == GROUP) {
            printf(SEA"╭────────────────────────────────────────╮\n"
                      "│ "GRAY BOLD"Enter ↵  "RESET"Return to Group Details       "SEA"│\n"
                      "╰────────────────────────────────────────╯\n");
        } else {
            printf(SEA"╭────────────────────────────────────────╮\n"
                      "│ "GRAY BOLD"Enter ↵  "RESET"Return to Shape Details       "SEA"│\n"
                      "╰────────────────────────────────────────╯\n");
        }
    } else {
        printf(CYAN"╭────────────────────────────────────────╮\n"
                   "│ "GRAY BOLD"Enter ↵  "RESET"Return to Main Menu           "CYAN"│\n"
                   "╰────────────────────────────────────────╯\n");
    }
}

void edit_node(ShapeNode* node, ShapeList* list, int start_col, int idx, bool from_details) {
    if (!node) return;
    while (1) {
        system("clear");
        int details_lines;
        move_cursor(1, 1); 
        if (node->type == GROUP) {
            Group* group = node->data.group;
            details_lines = display_group_info(group, idx);
        }
        else {
            details_lines = display_shape_info(node, list, idx); ;
        }
        move_cursor(6 + details_lines, 1);
        return_options(node, from_details);

        move_cursor(1, start_col); printf(MAGENTA"╔══════════════════════════════════╗\n");
        move_cursor(2, start_col); printf(       "║               "RESET BOLD"EDIT               "RESET MAGENTA"║\n");
        move_cursor(3, start_col); printf(       "╚══════════════════════════════════╝\n");

        switch (node->type) {
            case CIRCLE: {
                Circle* c = node->data.circle;
                
                move_cursor(4, start_col);  printf("╭──────────────────────────────────╮\n");
                move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Edit Position                "MAGENTA"│\n");
                move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Edit Radius                  "MAGENTA"│\n");
                move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Edit Stroke Color            "MAGENTA"│\n");
                move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Edit Fill Color              "MAGENTA"│\n");
                move_cursor(9, start_col);  printf("│ "MAGENTA"5 ➤ "RESET"Edit Thickness               "MAGENTA"│\n");
                move_cursor(10, start_col); printf("│ "MAGENTA"6 ➤ "RESET"Edit Scale                   "MAGENTA"│\n");
                move_cursor(11, start_col); printf("╰──────────────────────────────────╯\n");
                
                InputStatus status;
                int choice;
                int select_line = 12 ;
                move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
                move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->6) "MAGENTA"│->│   │\n");
                move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);
                
                move_cursor(select_line + 1, start_col + 31); 
                fflush(stdout);

                status = get_integer(&choice, "", 1, 1, 6, false);
                
                if (status == INPUT_CANCEL) return;
                move_cursor(6 + details_lines, 1);
                printf(MAGENTA"╭────────────────────────────────────────╮\n"
                              "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
                              "╰────────────────────────────────────────╯\n");
                switch(choice) {
                    case 1: {
                        int x, y;
                        move_cursor(6, 3); printf(MAGENTA"Center");
                        move_cursor(select_line + 3, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 10); 
                        if (get_integer(&x, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Set Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 18 + 10); 
                        if (get_integer(&y, "", 3, 0, 9, true) != INPUT_SUCCESS) break;
                        c->x = x, c->y = y;
                    } break;
                    case 2: {
                        move_cursor(7, 3); printf(MAGENTA"Radius");
                        move_cursor(select_line + 3, start_col); printf("╭────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Radius │     │");
                        move_cursor(select_line + 5, start_col); printf("╰────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 15); 
                        get_integer(&c->radius, "", 3, 0, 9, false); 
                    } break; 
                    case 3: {
                        move_cursor(8, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 21);
                        if (get_color(c->stroke_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 4: {
                        move_cursor(9, 3); printf(MAGENTA"Fill Color");
                        move_cursor(select_line + 3, start_col); printf("╭────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Fill Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 19);
                        if (get_color(c->fill_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 5: {
                        move_cursor(10, 3); printf(MAGENTA"Thickness");
                        move_cursor(select_line + 3, start_col); printf("╭───────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Thickness │    │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&c->thickness, "", 2, 0, 9, false); 
                    } break; 
                    case 6: {
                        int sx, sy;
                        move_cursor(11, 3); printf(MAGENTA"Scale");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Scale X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 16);
                        if (get_integer(&sx, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 


                        move_cursor(select_line + 3, start_col + 23); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 23); printf("│ Set Scale Y │     │");
                        move_cursor(select_line + 5, start_col + 23); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 23 + 16);
                        if (get_integer(&sy, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 
                        c->scalex = sx, c->scaley = sy;
                    } break; 
                    default: continue;
                }
            } break;

            case SQUARE: {
                Square* s = node->data.square;

                move_cursor(4, start_col);  printf("╭──────────────────────────────────╮\n");
                move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Edit Position                "MAGENTA"│\n");
                move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Edit Side Length             "MAGENTA"│\n");
                move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Edit Stroke Color            "MAGENTA"│\n");
                move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Edit Fill Color              "MAGENTA"│\n");
                move_cursor(9, start_col);  printf("│ "MAGENTA"5 ➤ "RESET"Edit Thickness               "MAGENTA"│\n");
                move_cursor(10, start_col); printf("│ "MAGENTA"6 ➤ "RESET"Edit Rounded                 "MAGENTA"│\n");
                move_cursor(11, start_col); printf("│ "MAGENTA"7 ➤ "RESET"Edit Scale                   "MAGENTA"│\n");
                move_cursor(12, start_col); printf("╰──────────────────────────────────╯\n");
                
                InputStatus status;
                int choice;
                int select_line = 13 ;
                move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
                move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->7) "MAGENTA"│->│   │\n");
                move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);
                
                move_cursor(select_line + 1, start_col + 31); 
                fflush(stdout);

                status = get_integer(&choice, "", 1, 1, 7, false);
                
                if (status == INPUT_CANCEL) return;
                move_cursor(6 + details_lines, 1);
                printf(MAGENTA"╭────────────────────────────────────────╮\n"
                              "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
                              "╰────────────────────────────────────────╯\n");
                switch(choice) {
                    case 1: {
                        int x, y;
                        move_cursor(6, 3); printf(MAGENTA"Starting Point");
                        move_cursor(select_line + 3, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 10); 
                        if (get_integer(&x, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Set Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 18 + 10); 
                        if (get_integer(&y, "", 3, 0, 9, true) != INPUT_SUCCESS) break;
                        s->x = x, s->y = y;
                    } break;
                    case 2: {
                        move_cursor(7, 3); printf(MAGENTA"Side Length");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Side Length │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 15); 
                        get_integer(&s->side, "", 3, 0, 9, false); 
                    } break;
                    case 3: {
                        move_cursor(8, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 21);
                        if (get_color(s->stroke_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 4: {
                        move_cursor(9, 3); printf(MAGENTA"Fill Color");
                        move_cursor(select_line + 3, start_col); printf("╭────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Fill Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 19);
                        if (get_color(s->fill_color, "") != INPUT_SUCCESS) break;  
                    } break;
                    case 5: {
                        move_cursor(10, 3); printf(MAGENTA"Thickness");
                        move_cursor(select_line + 3, start_col); printf("╭───────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Thickness │    │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&s->thickness, "", 2, 0, 9, false); 
                    } break; 
                    case 6: {
                        move_cursor(11, 3); printf(MAGENTA"Rounded");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Rounded │    │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&s->rounded, "", 2, 0, 9, false); 
                    } break; 
                    case 7: {
                        int sx, sy;
                        move_cursor(12, 3); printf(MAGENTA"Scale");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Scale X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 16);
                        if (get_integer(&sx, "", 2, 0, 9, true) != INPUT_SUCCESS) break;  

                        move_cursor(select_line + 3, start_col + 23); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 23); printf("│ Set Scale Y │     │");
                        move_cursor(select_line + 5, start_col + 23); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 23 + 16);
                        if (get_integer(&sy, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 
                        s->scalex = sx, s->scaley = sy;
                    } break; 
                    default: continue;
                }
            } break;
            
            case RECTANGLE: {
                Rectangle* r = node->data.rectangle;

                move_cursor(4, start_col); 
                printf(                            "╭──────────────────────────────────╮\n");
                move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Edit Position                "MAGENTA"│\n");
                move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Edit Size                    "MAGENTA"│\n");
                move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Edit Stroke Color            "MAGENTA"│\n");
                move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Edit Fill Color              "MAGENTA"│\n");
                move_cursor(9, start_col);  printf("│ "MAGENTA"5 ➤ "RESET"Edit Thickness               "MAGENTA"│\n");
                move_cursor(10, start_col); printf("│ "MAGENTA"6 ➤ "RESET"Edit Rounded                 "MAGENTA"│\n");
                move_cursor(11, start_col); printf("│ "MAGENTA"7 ➤ "RESET"Edit Scale                   "MAGENTA"│\n");
                move_cursor(12, start_col); printf("╰──────────────────────────────────╯\n");
                
                InputStatus status;
                int choice;
                int select_line = 13 ;
                move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
                move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->7) "MAGENTA"│->│   │\n");
                move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);
                
                move_cursor(select_line + 1, start_col + 31); 
                fflush(stdout);
                status = get_integer(&choice, "", 1, 1, 7, false);
                
                if (status == INPUT_CANCEL) return;
                move_cursor(6 + details_lines, 1);
                printf(MAGENTA"╭────────────────────────────────────────╮\n"
                              "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
                              "╰────────────────────────────────────────╯\n");
                switch(choice) {
                    case 1: {
                        int x, y;
                        move_cursor(6, 3); printf(MAGENTA"Starting Point");
                        move_cursor(select_line + 3, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 10); 
                        if (get_integer(&x, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Set Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 18 + 10); 
                        if (get_integer(&y, "", 3, 0, 9, true) != INPUT_SUCCESS) break;
                        r->x = x, r->y = y;
                    } break;
                    case 2: {
                        int width, height;
                        move_cursor(7, 3); printf(MAGENTA"Size");
                        move_cursor(select_line + 3, start_col); printf("╭───────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Width │     │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 14); 
                        if (get_integer(&width, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 21); printf("╭────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 21); printf("│ Set Height │     │");
                        move_cursor(select_line + 5, start_col + 21); printf("╰────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 21 + 15); 
                        if (get_integer(&height, "", 3, 0, 9, true) != INPUT_SUCCESS) break;
                        r->width = width, r->height = height;
                    } break;
                    case 3: {
                        move_cursor(8, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 19);
                        if (get_color(r->stroke_color, "") != INPUT_SUCCESS) break;  
                    } break;
                    case 4: {
                        move_cursor(9, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 21);
                        if (get_color(r->fill_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 5: {
                        move_cursor(10, 3); printf(MAGENTA"Thickness");
                        move_cursor(select_line + 3, start_col); printf("╭───────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Thickness │    │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&r->thickness, "", 2, 0, 9, false); 
                    } break; 
                    case 6: {
                        move_cursor(11, 3); printf(MAGENTA"Rounded");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Rounded │    │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&r->rounded, "", 2, 0, 9, false); 
                    } break; 
                    case 7: {
                        int sx, sy;
                        move_cursor(12, 3); printf(MAGENTA"Scale");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Scale X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 16);
                        if (get_integer(&sx, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 

                        move_cursor(select_line + 3, start_col + 23); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 23); printf("│ Set Scale Y │     │");
                        move_cursor(select_line + 5, start_col + 23); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 23 + 16);
                        if (get_integer(&sy, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 
                        r->scalex = sx, r->scaley = sy;
                    } break; 
                    default: continue;
                }
            } break;

            case LINE: {
                Line* l = node->data.line;

                move_cursor(4, start_col);  printf("╭──────────────────────────────────╮\n");
                move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Edit Starting Position       "MAGENTA"│\n");
                move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Edit Eding Position          "MAGENTA"│\n");
                move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Edit Stroke Color            "MAGENTA"│\n");
                move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Edit Thickness               "MAGENTA"│\n");
                move_cursor(9, start_col);  printf("│ "MAGENTA"5 ➤ "RESET"Edit Scale                   "MAGENTA"│\n");
                move_cursor(10, start_col); printf("╰──────────────────────────────────╯\n");

                InputStatus status;
                int choice;
                int select_line = 11 ;
                move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
                move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->5) "MAGENTA"│->│   │\n");
                move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);
                
                move_cursor(select_line + 1, start_col + 31); 
                fflush(stdout);

                status = get_integer(&choice, "", 1, 1, 5, false);
                
                if (status == INPUT_CANCEL) return;
                move_cursor(6 + details_lines, 1);
                printf(MAGENTA"╭────────────────────────────────────────╮\n"
                              "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
                              "╰────────────────────────────────────────╯\n");
                switch(choice) {
                    case 1: {
                        int x1, y1;
                        move_cursor(6, 3); printf(MAGENTA"Starting Point");
                        move_cursor(select_line + 3, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 10); 
                        if (get_integer(&x1, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Set Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 18 + 10); 
                        if (get_integer(&y1, "", 3, 0, 9, true) != INPUT_SUCCESS) break;
                        l->x1 = x1, l->y1 = y1;
                    } break;
                    case 2: {
                        int x2, y2;
                        move_cursor(7, 3); printf(MAGENTA"Ending Point");
                        move_cursor(select_line + 3, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 10); 
                        if (get_integer(&x2, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Set Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 18 + 10); 
                        if (get_integer(&y2, "", 3, 0, 9, true) != INPUT_SUCCESS) break;
                        l->x2 = x2, l->y2 = y2;
                    } break;
                    case 3: {
                        move_cursor(8, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 21);
                        if (get_color(l->stroke_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 4: {
                        move_cursor(9, 3); printf(MAGENTA"Thickness");
                        move_cursor(select_line + 3, start_col); printf("╭───────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Thickness │    │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&l->thickness, "", 2, 0, 9, false); 
                    } break; 
                    case 5: {
                        int sx, sy;
                        move_cursor(10, 3); printf(MAGENTA"Scale");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Scale X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 16);
                        if (get_integer(&sx, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 

                        move_cursor(select_line + 3, start_col + 23); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 23); printf("│ Set Scale Y │     │");
                        move_cursor(select_line + 5, start_col + 23); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 23 + 16);
                        if (get_integer(&sy, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 
                        l->scalex = sx, l->scaley = sy;
                    } break; 
                    default: continue;
                }
            } break;
            case ELLIPSE: {
                Ellipse* e = node->data.ellipse;
                
                move_cursor(4, start_col);  printf("╭──────────────────────────────────╮\n");
                move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Edit Position                "MAGENTA"│\n");
                move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Edit Radius                  "MAGENTA"│\n");
                move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Edit Stroke Color            "MAGENTA"│\n");
                move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Edit Fill Color              "MAGENTA"│\n");
                move_cursor(9, start_col);  printf("│ "MAGENTA"5 ➤ "RESET"Edit Thickness               "MAGENTA"│\n");
                move_cursor(10, start_col); printf("│ "MAGENTA"6 ➤ "RESET"Edit Scale                   "MAGENTA"│\n");
                move_cursor(11, start_col); printf("╰──────────────────────────────────╯\n");
                
                InputStatus status;
                int choice;
                int select_line = 12 ;
                move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
                move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->6) "MAGENTA"│->│   │\n");
                move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);
                
                move_cursor(select_line + 1, start_col + 31); 
                fflush(stdout);

                status = get_integer(&choice, "", 1, 1, 6, false);
                if (status == INPUT_CANCEL) return;

                move_cursor(6 + details_lines, 1);
                printf(MAGENTA"╭────────────────────────────────────────╮\n"
                              "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
                              "╰────────────────────────────────────────╯\n");
                switch(choice) {
                    case 1: {
                        int x, y;
                        move_cursor(6, 3); printf(MAGENTA"Center");
                        move_cursor(select_line + 3, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 10); 
                        if (get_integer(&x, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭───────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Set Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰───────┴──────╯");
                        move_cursor(select_line + 4, start_col + 18 + 10); 
                        if (get_integer(&y, "", 3, 0, 9, true) != INPUT_SUCCESS) break;
                        e->x = x, e->y = y;
                    } break;
                    case 2: {
                        int radiusx, radiusy;
                        move_cursor(7, 3); printf(MAGENTA"Radius");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Radius X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 17); 
                        if (get_integer(&radiusx, "", 3, 0, 9, false) != INPUT_SUCCESS) break; 

                        move_cursor(select_line + 3, start_col + 24); printf("╭──────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 24); printf("│ Set Radius Y │     │");
                        move_cursor(select_line + 5, start_col + 24); printf("╰──────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 24 + 17); 
                        if (get_integer(&radiusy, "", 3, 0, 9, false) != INPUT_SUCCESS) break; 
                        e->radiusx = radiusx, e->radiusy = radiusy;
                    } break; 
                    case 3: {
                        move_cursor(8, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 21);
                        if (get_color(e->stroke_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 4: {
                        move_cursor(9, 3); printf(MAGENTA"Fill Color");
                        move_cursor(select_line + 3, start_col); printf("╭────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Fill Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 19);
                        if (get_color(e->fill_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 5: {
                        move_cursor(10, 3); printf(MAGENTA"Thickness");
                        move_cursor(select_line + 3, start_col); printf("╭───────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Thickness │    │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&e->thickness, "", 2, 0, 9, false); 
                    } break; 
                    case 6: {
                        int sx, sy;
                        move_cursor(11, 3); printf(MAGENTA"Scale");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Scale X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 16);
                        if (get_integer(&sx, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 
                        
                        move_cursor(select_line + 3, start_col + 23); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 23); printf("│ Set Scale Y │     │");
                        move_cursor(select_line + 5, start_col + 23); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 23 + 16);
                        if (get_integer(&sy, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 
                        e->scalex = sx, e->scaley = sy;
                    } break; 
                    default: continue;
                }
            } break;
            case POLYGON: {
                Polygon* pg = node->data.polygon;

                move_cursor(4, start_col);  printf("╭──────────────────────────────────╮\n");
                move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Move entire shape            "MAGENTA"│\n");
                move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Move single point            "MAGENTA"│\n");
                move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Edit Stroke Color            "MAGENTA"│\n");
                move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Edit Fill Color              "MAGENTA"│\n");
                move_cursor(9, start_col);  printf("│ "MAGENTA"5 ➤ "RESET"Edit Thickness               "MAGENTA"│\n");
                move_cursor(10, start_col); printf("│ "MAGENTA"6 ➤ "RESET"Edit Scale                   "MAGENTA"│\n");
                move_cursor(11, start_col); printf("╰──────────────────────────────────╯\n");

                int select_line = 12;
                InputStatus status;
                int choice;

                move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
                move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->6) "MAGENTA"│->│   │\n");
                move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);

                move_cursor(select_line + 1, start_col + 31);
                fflush(stdout);

                status = get_integer(&choice, "", 1, 1, 6, false);
                if (status == INPUT_CANCEL) return;

                move_cursor(6 + details_lines, 1);
                printf(MAGENTA"╭────────────────────────────────────────╮\n"
                              "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
                              "╰────────────────────────────────────────╯\n");
                switch (choice) {
                    case 1: {
                        int dx, dy;
                        move_cursor(6, 3); printf(MAGENTA"Points:");
                        move_cursor(select_line + 3, start_col); printf("╭────────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Move X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰────────┴──────╯");
                        move_cursor(select_line + 4, start_col + 11);
                        if (get_integer(&dx, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭────────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Move Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰────────┴──────╯");
                        move_cursor(select_line + 4, start_col + 29);
                        if (get_integer(&dy, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        for (int i = 0; i < pg->nb_points; i++) {
                            pg->points[i].x += dx;
                            pg->points[i].y += dy;
                        }
                    } break;

                    case 2: {
                        int idx, x, y;
                        move_cursor(6, 3); printf(MAGENTA"Points");
                        if (pg->nb_points -1 < 10) {
                            move_cursor(select_line + 3, start_col); printf(MAGENTA"╭────────────────┬───╮");
                            move_cursor(select_line + 4, start_col); printf(       "│ Point ID (0-%d) │   │", pg->nb_points - 1);
                            move_cursor(select_line + 5, start_col); printf(       "╰────────────────┴───╯");
                            move_cursor(select_line + 4, start_col + 19);
                            if (get_integer(&idx, "", 1, 0, pg->nb_points - 1, false) != INPUT_SUCCESS) break;
                        } else {
                            move_cursor(select_line + 3, start_col); printf(MAGENTA"╭─────────────────┬────╮");
                            move_cursor(select_line + 4, start_col); printf(       "│ Point ID (0-%d) │    │", pg->nb_points - 1);
                            move_cursor(select_line + 5, start_col); printf(       "╰─────────────────┴────╯");
                            move_cursor(select_line + 4, start_col + 20);
                            if (get_integer(&idx, "", 2, 0, 9, false) != INPUT_SUCCESS) break;
                        }

                        move_cursor(select_line + 7, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 8, start_col); printf("│ Set X │      │");
                        move_cursor(select_line + 9, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 8, start_col + 10);
                        if (get_integer(&x, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 10, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 11, start_col); printf("│ Set Y │      │");
                        move_cursor(select_line + 12, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 11, start_col + 10);
                        if (get_integer(&y, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        pg->points[idx].x = x;
                        pg->points[idx].y = y;
                    } break;

                    case 3: {
                        move_cursor(details_lines + 1, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 21);
                        if (get_color(pg->stroke_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 4: {
                        move_cursor(details_lines + 2, 3); printf(MAGENTA"Fill Color");
                        move_cursor(select_line + 3, start_col); printf("╭────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Fill Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 19);
                        if (get_color(pg->fill_color, "") != INPUT_SUCCESS) break; 
                    } break;

                    case 5: {
                        move_cursor(details_lines + 3, 3); printf(MAGENTA"Thickness");
                        move_cursor(select_line + 3, start_col); printf("╭───────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Thickness │    │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&pg->thickness, "", 2, 0, 9, false);
                    } break;

                    case 6: {
                        int sx, sy;
                        move_cursor(details_lines + 4, 3); printf(MAGENTA"Scale");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Scale X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 16);
                        if (get_integer(&sx, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 23); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 23); printf("│ Set Scale Y │     │");
                        move_cursor(select_line + 5, start_col + 23); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 39);
                        if (get_integer(&sy, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        pg->scalex = sx;
                        pg->scaley = sy;
                    } break;
                    default: return;
                }
            } break;
            case MULTILINE: {
                Multiline* ml = node->data.multiline;

                move_cursor(4, start_col);  printf("╭──────────────────────────────────╮\n");
                move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Move entire shape            "MAGENTA"│\n");
                move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Move single point            "MAGENTA"│\n");
                move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Edit Stroke Color            "MAGENTA"│\n");
                move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Edit Thickness               "MAGENTA"│\n");
                move_cursor(9, start_col);  printf("│ "MAGENTA"5 ➤ "RESET"Edit Scale                   "MAGENTA"│\n");
                move_cursor(10, start_col); printf("╰──────────────────────────────────╯\n");

                int select_line = 11;
                InputStatus status;
                int choice;

                move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
                move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->5) "MAGENTA"│->│   │\n");
                move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);

                move_cursor(select_line + 1, start_col + 31);
                fflush(stdout);

                status = get_integer(&choice, "", 1, 1, 5, false);
                if (status == INPUT_CANCEL) return;

                move_cursor(6 + details_lines, 1);
                printf(MAGENTA"╭────────────────────────────────────────╮\n"
                              "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
                              "╰────────────────────────────────────────╯\n");
                switch (choice) {
                    case 1: {
                        int dx, dy;
                        move_cursor(6, 3); printf(MAGENTA"Points:");
                        move_cursor(select_line + 3, start_col); printf("╭────────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Move X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰────────┴──────╯");
                        move_cursor(select_line + 4, start_col + 11);
                        if (get_integer(&dx, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭────────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Move Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰────────┴──────╯");
                        move_cursor(select_line + 4, start_col + 29);
                        if (get_integer(&dy, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        for (int i = 0; i < ml->nb_points; i++) {
                            ml->points[i].x += dx;
                            ml->points[i].y += dy;
                        }
                    } break;

                    case 2: {
                        int idx, x, y;
                        move_cursor(6, 3); printf(MAGENTA"Points");
                        if (ml->nb_points -1 < 10) {
                            move_cursor(select_line + 3, start_col); printf(MAGENTA"╭────────────────┬───╮");
                            move_cursor(select_line + 4, start_col); printf(       "│ Point ID (0-%d) │   │", ml->nb_points - 1);
                            move_cursor(select_line + 5, start_col); printf(       "╰────────────────┴───╯");
                            move_cursor(select_line + 4, start_col + 19);
                            if (get_integer(&idx, "", 1, 0, ml->nb_points - 1, false) != INPUT_SUCCESS) break;
                        } else {
                            move_cursor(select_line + 3, start_col); printf(MAGENTA"╭─────────────────┬────╮");
                            move_cursor(select_line + 4, start_col); printf(       "│ Point ID (0-%d) │    │", ml->nb_points - 1);
                            move_cursor(select_line + 5, start_col); printf(       "╰─────────────────┴────╯");
                            move_cursor(select_line + 4, start_col + 20);
                            if (get_integer(&idx, "", 2, 0, 9, false) != INPUT_SUCCESS) break;
                        }

                        move_cursor(select_line + 7, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 8, start_col); printf("│ Set X │      │");
                        move_cursor(select_line + 9, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 8, start_col + 10);
                        if (get_integer(&x, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 10, start_col); printf("╭───────┬──────╮");
                        move_cursor(select_line + 11, start_col); printf("│ Set Y │      │");
                        move_cursor(select_line + 12, start_col); printf("╰───────┴──────╯");
                        move_cursor(select_line + 11, start_col + 10);
                        if (get_integer(&y, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        ml->points[idx].x = x;
                        ml->points[idx].y = y;
                    } break;

                    case 3: {
                        move_cursor(details_lines + 1, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 21);
                        if (get_color(ml->stroke_color, "") != INPUT_SUCCESS) break; 
                    } break;

                    case 4: {
                        move_cursor(details_lines + 2, 3); printf(MAGENTA"Thickness");
                        move_cursor(select_line + 3, start_col); printf("╭───────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Thickness │    │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&ml->thickness, "", 2, 0, 9, false);
                    } break;

                    case 5: {
                        int sx, sy;
                        move_cursor(details_lines + 3, 3); printf(MAGENTA"Scale");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Scale X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 16);
                        if (get_integer(&sx, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 23); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 23); printf("│ Set Scale Y │     │");
                        move_cursor(select_line + 5, start_col + 23); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 39);
                        if (get_integer(&sy, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        ml->scalex = sx;
                        ml->scaley = sy;
                    } break;
                    default: return;
                }
            } break;
            case PATH: {
                Path* p = node->data.path;

                move_cursor(4, start_col);  printf("╭──────────────────────────────────╮\n");
                move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Toggle Closed                "MAGENTA"│\n");
                move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Edit Position                "MAGENTA"│\n");
                move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Edit Stroke Color            "MAGENTA"│\n");
                move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Edit Fill Color              "MAGENTA"│\n");
                move_cursor(9, start_col);  printf("│ "MAGENTA"5 ➤ "RESET"Edit Thickness               "MAGENTA"│\n");
                move_cursor(10, start_col); printf("│ "MAGENTA"6 ➤ "RESET"Edit Scale                   "MAGENTA"│\n");
                move_cursor(11, start_col); printf("╰──────────────────────────────────╯\n");

                int select_line = 12;
                InputStatus status;
                int choice;

                move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
                move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->6) "MAGENTA"│->│   │\n");
                move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);

                move_cursor(select_line + 1, start_col + 31);
                fflush(stdout);

                status = get_integer(&choice, "", 1, 1, 6, false);
                if (status == INPUT_CANCEL) return;

                move_cursor(6 + details_lines, 1);
                printf(MAGENTA"╭────────────────────────────────────────╮\n"
                              "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
                              "╰────────────────────────────────────────╯\n");
                switch (choice) {
                    case 1: {
                        move_cursor(6, 3); printf(MAGENTA"Closed:");
                        temp_message("", 0, 1);
                        p->closed = !p->closed;
                    } break;

                    case 2: {
                    int dx, dy;
                        move_cursor(12, 3); printf(MAGENTA"Segments:");
                        move_cursor(select_line + 3, start_col); printf("╭────────┬──────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Move X │      │");
                        move_cursor(select_line + 5, start_col); printf("╰────────┴──────╯");
                        move_cursor(select_line + 4, start_col + 11);
                        if (get_integer(&dx, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        move_cursor(select_line + 3, start_col + 18); printf("╭────────┬──────╮");
                        move_cursor(select_line + 4, start_col + 18); printf("│ Move Y │      │");
                        move_cursor(select_line + 5, start_col + 18); printf("╰────────┴──────╯");
                        move_cursor(select_line + 4, start_col + 18 + 11);
                        if (get_integer(&dy, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

                        for (int i = 0; i < p->nb_points; i++) {
                            p->points[i].x += dx;
                            p->points[i].y += dy;
                        }
                    } break;

                    case 3: {
                        move_cursor(7, 3); printf(MAGENTA"Stroke Color");
                        move_cursor(select_line + 3, start_col); printf("╭──────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Stroke Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰──────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 21);
                        if (get_color(p->stroke_color, "") != INPUT_SUCCESS) break; 
                    } break;
                    case 4: {
                        move_cursor(8, 3); printf(MAGENTA"Fill Color");
                        move_cursor(select_line + 3, start_col); printf("╭────────────────┬─────────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Fill Color │         │");
                        move_cursor(select_line + 5, start_col); printf("╰────────────────┴─────────╯");
                        move_cursor(select_line + 4, start_col + 19);
                        if (get_color(p->fill_color, "") != INPUT_SUCCESS) break; 
                    } break;

                    case 5: {
                        move_cursor(9, 3); printf(MAGENTA"Thickness");
                        move_cursor(select_line + 3, start_col); printf("╭───────────────┬────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Thickness │    │");
                        move_cursor(select_line + 5, start_col); printf("╰───────────────┴────╯");
                        move_cursor(select_line + 4, start_col + 18);
                        get_integer(&p->thickness, "", 2, 0, 9, false); 
                    } break;

                    case 6: {
                        int sx, sy;
                        move_cursor(10, 3); printf(MAGENTA"Scale");
                        move_cursor(select_line + 3, start_col); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col); printf("│ Set Scale X │     │");
                        move_cursor(select_line + 5, start_col); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 16);
                        if (get_integer(&sx, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 
                        
                        move_cursor(select_line + 3, start_col + 23); printf("╭─────────────┬─────╮");
                        move_cursor(select_line + 4, start_col + 23); printf("│ Set Scale Y │     │");
                        move_cursor(select_line + 5, start_col + 23); printf("╰─────────────┴─────╯");
                        move_cursor(select_line + 4, start_col + 23 + 16);
                        if (get_integer(&sy, "", 2, 0, 9, true) != INPUT_SUCCESS) break; 
                        p->scalex = sx;
                        p->scaley = sy;
                    } break;
                    default: return;
                }
            } break;
			case GROUP: {
				Group* g = node->data.group;

				move_cursor(4, start_col);  printf("╭──────────────────────────────────╮\n");
				move_cursor(5, start_col);  printf("│ "MAGENTA"1 ➤ "RESET"Rename the Group             "MAGENTA"│\n");
				move_cursor(6, start_col);  printf("│ "MAGENTA"2 ➤ "RESET"Add a Shape to the Group     "MAGENTA"│\n");
				move_cursor(7, start_col);  printf("│ "MAGENTA"3 ➤ "RESET"Remove a Shape from Group    "MAGENTA"│\n");
				move_cursor(8, start_col);  printf("│ "MAGENTA"4 ➤ "RESET"Move (all Shapes in Group)   "MAGENTA"│\n");
				move_cursor(9, start_col);  printf("╰──────────────────────────────────╯\n");

				int select_line = 10;
				InputStatus status;
				int choice;

				move_cursor(select_line, start_col);     printf(MAGENTA"╭─────────────────────────╮  ╭───╮\n");
				move_cursor(select_line + 1, start_col); printf(       "│ "RESET"Select an option (1->4) "MAGENTA"│->│   │\n");
				move_cursor(select_line + 2, start_col); printf(MAGENTA"╰─────────────────────────╯  ╰───╯"RESET);

				move_cursor(select_line + 1, start_col + 31);
				fflush(stdout);

				status = get_integer(&choice, "", 1, 1, 4, false);
				if (status == INPUT_CANCEL) return;

				move_cursor(6 + details_lines, 1);
				printf(MAGENTA"╭────────────────────────────────────────╮\n"
							  "│ "GRAY BOLD"Enter ↵  "RESET"Cancel current Edit           "MAGENTA"│\n"
							  "╰────────────────────────────────────────╯\n");

				switch (choice) {
					case 1: {
						char newname[13] = {0}; // 12 chars max + '\0'
						move_cursor(5, 9); printf(MAGENTA BOLD"%s", g->name);
						move_cursor(select_line + 3, start_col); printf("╭────────────────┬──────────────╮");
						move_cursor(select_line + 4, start_col); printf("│ Set Group Name │              │");
						move_cursor(select_line + 5, start_col); printf("╰────────────────┴──────────────╯");
						move_cursor(select_line + 4, start_col + 19);

						block_enter();
						for (int i = 0; i < 12; i++) {
							int c = getchar();
							if (c == '\n') {
								newname[i] = '\0';
								break;
							}
							newname[i] = (char)c;
							putchar(c);
						}
						unblock_enter();

						newname[12] = '\0';
						if (g->name) free(g->name);
						g->name = strdup(newname);
					} break;

					case 2: {
						ShapeNode* n = list->head;
						int idx = 0, display_idx = 0;
						int index_map[list->size];
						int available_count = 0;

						move_cursor(select_line + 3, start_col);
						printf("╭─────── Elements Available ───────╮\n");

						while (n) {
							bool already_in_group = false;
							ShapeNode* s = g->shapes ? g->shapes->head : NULL;
							while (s) {
								if (s == n) { already_in_group = true; break; }
								s = s->next;
							}

							if (!already_in_group) {
								move_cursor(select_line + 4 + display_idx, start_col);
								printf("│ "RESET"[%d] ", display_idx);

								switch (n->type) {
									case SQUARE: printf("Square"); break;
									case CIRCLE: printf("Circle"); break;
									case RECTANGLE: printf("Rectangle"); break;
									case LINE: printf("Line"); break;
									case ELLIPSE: printf("Ellipse"); break;
									case POLYGON: printf("Polygon"); break;
									case MULTILINE: printf("Multiline"); break;
									case PATH: printf("Path"); break;
									case GROUP: printf("Group: %s", n->data.group->name); break;
								}

								move_cursor(select_line + 4 + display_idx, start_col + 35);
								printf(MAGENTA"│\n");
								index_map[display_idx] = idx;
								display_idx++;
								available_count++;
							}

							n = n->next;
							idx++;
						}

						if (available_count == 0) {
							move_cursor(select_line + 4, start_col); printf(ORANGE"╭─────────────────────────────╮");
							move_cursor(select_line + 5, start_col); printf(      "│ No available element to add │");
							move_cursor(select_line + 6, start_col); printf(      "╰─────────────────────────────╯"RESET);
							temp_message("", 0, 2);
							break;
						}

						move_cursor(select_line + 4 + display_idx, start_col);
						printf("╰──────────────────────────────────╯");

						int choice_add;

						if (available_count < 11) {
							move_cursor(select_line + 5 + display_idx, start_col); printf("╭─────────────────────────┬───╮\n");
							move_cursor(select_line + 6 + display_idx, start_col); printf("│ Index of element to add │   │\n");
							move_cursor(select_line + 7 + display_idx, start_col); printf("╰─────────────────────────┴───╯");
							move_cursor(select_line + 6 + display_idx, start_col + 28);
							if (get_integer(&choice_add, "", 1, 0, available_count - 1, false) != INPUT_SUCCESS) break;
						} else {
							move_cursor(select_line + 5 + display_idx, start_col); printf("╭─────────────────────────┬────╮");
							move_cursor(select_line + 6 + display_idx, start_col); printf("│ Index of element to add │    │");
							move_cursor(select_line + 7 + display_idx, start_col); printf("╰─────────────────────────┴────╯");
							move_cursor(select_line + 6 + display_idx, start_col + 28);
							if (get_integer(&choice_add, "", 2, 0, 9, false) != INPUT_SUCCESS) break;
						}

						ShapeNode* node_to_add = get_node_at(list, index_map[choice_add]);
						if (!node_to_add) break;

						add_to_group(g, node_to_add);
					} break;

					case 3: {
						if (!g->shapes || g->shapes->size == 0) {
							move_cursor(select_line + 3, start_col); printf(ORANGE"╭───────────────────────────╮");
							move_cursor(select_line + 4, start_col); printf(      "│ No element in this group! │");
							move_cursor(select_line + 5, start_col); printf(      "╰───────────────────────────╯"RESET);
							temp_message("", 0, 2);
							break;
						}

						int sub_index;

						if (g->shapes->size < 11) {
							move_cursor(select_line + 3, start_col); printf("╭──────────────────────────┬───╮\n");
							move_cursor(select_line + 4, start_col); printf("│ Index of shape to remove │   │\n");
							move_cursor(select_line + 5, start_col); printf("╰──────────────────────────┴───╯");
							move_cursor(select_line + 4, start_col + 29);
							if (get_integer(&sub_index, "", 1, 0, g->shapes->size - 1, false) != INPUT_SUCCESS) break;
						} else {
							printf("╭──────────────────────────┬────╮\n");
							printf("│ Index of shape to remove │    │\n");
							printf("╰──────────────────────────┴────╯");
							move_cursor(select_line + 4, start_col + 29);
							if (get_integer(&sub_index, "", 2, 0, g->shapes->size - 1, false) != INPUT_SUCCESS) break;
						}

						if (!remove_shape_from_group(g, sub_index)) {
							move_cursor(select_line + 6, start_col); printf(RED"╭────────────────────────────╮");
							move_cursor(select_line + 7, start_col); printf(   "│ Error removing this shape! │");
							move_cursor(select_line + 8, start_col); printf(   "╰────────────────────────────╯"RESET);
							temp_message("", 0, 2);
							break;
						}

						move_cursor(select_line + 6, start_col); printf(GREEN"╭─────────────────────────────╮");
						move_cursor(select_line + 7, start_col); printf(     "│ Shape successfully removed! │");
						move_cursor(select_line + 8, start_col); printf(     "╰─────────────────────────────╯"RESET);
						temp_message("", 0, 2);
					} break;

					case 4: {
						int dx, dy;
						for (int i = 0; i < details_lines - 2; i++) {
							move_cursor(7 + i, 2);
							printf(MAGENTA"➤");
						}
						move_cursor(select_line + 3, start_col); printf("╭────────┬──────╮");
						move_cursor(select_line + 4, start_col); printf("│ Move X │      │");
						move_cursor(select_line + 5, start_col); printf("╰────────┴──────╯");
						move_cursor(select_line + 4, start_col + 11);
						if (get_integer(&dx, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

						move_cursor(select_line + 3, start_col + 18); printf("╭────────┬──────╮");
						move_cursor(select_line + 4, start_col + 18); printf("│ Move Y │      │");
						move_cursor(select_line + 5, start_col + 18); printf("╰────────┴──────╯");
						move_cursor(select_line + 4, start_col + 18 + 11);
						if (get_integer(&dy, "", 3, 0, 9, true) != INPUT_SUCCESS) break;

						move_group(g, dx, dy);

						move_cursor(select_line + 7, start_col); printf(GREEN"╭───────────────────────────╮");
						move_cursor(select_line + 8, start_col); printf(     "│ Group successfully moved! │");
						move_cursor(select_line + 9, start_col); printf(     "╰───────────────────────────╯"RESET);
						temp_message("", 0, 2);
					} break;

					default: return;
				}
			} break;
        }
    }
}

void edit_shape_or_group_menu(ShapeList* list) {
    if (!list || list->size == 0) {
        move_cursor(19, 37); printf(ORANGE"╭─────────────────────────────╮");
        move_cursor(20, 35); printf(    "->│ No shapes or groups to edit │");
        move_cursor(21, 37); printf(      "╰─────────────────────────────╯");
        move_cursor(20 + list->size, 32);
        temp_message(ORANGE"3", 0, 2);
        return;
    }
    int max_idx = list->size - 1;
    int idx;
    InputStatus status;
    do {
        display_list(list);

        if (list->size < 10) {
            printf(MAGENTA"╭───────────────────────────────╮  ╭───╮\n"
                          "│ "RESET"Select an item (0->%d) to edit "MAGENTA"│->│   │\n"
                   MAGENTA"╰───────────────────────────────╯  ╰───╯", max_idx);
            move_cursor(11 + list->size , 38); 
            fflush(stdout);
            status = get_integer(&idx, "", 1, 0, max_idx, false);
        }
        else {
            printf(MAGENTA"╭────────────────────────────────╮  ╭────╮\n"
                          "│ "RESET"Select an item (0->%d) to edit "MAGENTA"│->│    │\n"
                   MAGENTA"╰────────────────────────────────╯  ╰────╯", max_idx);
            move_cursor(11 + list->size , 38); 
            fflush(stdout);
            status = get_integer(&idx, "", 2, 0, max_idx, false);
        }

        if (status == INPUT_SUCCESS) break;
        if (status == INPUT_CANCEL) return;
        
    } while (status != INPUT_SUCCESS);

    ShapeNode* node = get_node_at(list, idx);

    edit_node(node, list, 44, idx, false);
}

void move_group(Group* group, int dx, int dy) {
    if (!group || !group->shapes) return;

    for (ShapeNode* node = group->shapes->head; node; node = node->next) {
        switch(node->type) {
            case CIRCLE:
                node->data.circle->x += dx;
                node->data.circle->y += dy;
                break;

            case RECTANGLE:
                node->data.rectangle->x += dx;
                node->data.rectangle->y += dy;
                break;

            case SQUARE:
                node->data.square->x += dx;
                node->data.square->y += dy;
                break;

            case LINE:
                node->data.line->x1 += dx;
                node->data.line->y1 += dy;
                node->data.line->x2 += dx;
                node->data.line->y2 += dy;
                break;

            case ELLIPSE:
                node->data.ellipse->x += dx;
                node->data.ellipse->y += dy;
                break;

            case POLYGON: {
                Polygon* pg = node->data.polygon;
                for (int i = 0; i < pg->nb_points; i++) {
                    pg->points[i].x += dx;
                    pg->points[i].y += dy;
                }
                break;
            }

            case MULTILINE: {
                Multiline* ml = node->data.multiline;
                for (int i = 0; i < ml->nb_points; i++) {
                    ml->points[i].x += dx;
                    ml->points[i].y += dy;
                }
                break;
            }
            case PATH: {
                Path* p = node->data.path;
                for (int i = 0; i < p->nb_points; i++) {
                    // Moving end point (x, y)
                    p->points[i].x += dx;
                    p->points[i].y += dy;

                    // Moving control points
                    p->points[i].control1.x += dx;
                    p->points[i].control1.y += dy;
                    
                    p->points[i].control2.x += dx;
                    p->points[i].control2.y += dy;
                }
                break;
            }
            case GROUP:
                // 🔁 Appel récursif sur le sous-groupe
                move_group(node->data.group, dx, dy);
                break;

            default:
                printf(RED"Movement not supported for this type.\n"RESET);
                break;
        }
    }
}