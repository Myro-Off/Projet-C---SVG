#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"
#include "memory.h"
#include "utils.h"
#include "structures.h"
#include "delete.h"
#include "edit.h"
#include "group.h"

void init_shape_list(ShapeList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void add_shape(ShapeList* list, ShapeType type, ShapeData data) {
    ShapeNode* node = malloc(sizeof(ShapeNode));
    if (!node) return;

    node->type = type;
    node->data = data;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail)
        list->tail->next = node;
    else
        list->head = node;

    list->tail = node;
    list->size++;
}

void delete_node(ShapeList* list, ShapeNode* node) {
    if (!node || !list->head) return;

    remove_shape_from_all_groups(list, node);
    free_shape_node(node);

    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    free(node);
    list->size--;
}

void free_shape_list(ShapeList* list) {
    ShapeNode* current = list->head;
    while (current) {
        ShapeNode* next = current->next;
        free_shape_node(current);
        free(current);
        current = next;
    }
    list->head = list->tail = NULL;
    list->size = 0;
}

ShapeNode* get_node_at(ShapeList* list, int idx) {
    if (!list || idx < 0 || idx >= list->size) return NULL;
    ShapeNode* current = list->head;
    for (int i = 0; i < idx; i++)
        current = current->next;
    return current;
}

const char* get_group_name_for_shape(const ShapeList* list, const ShapeNode* node) {
    if (!list || !node) return NULL;

    for (ShapeNode* current = list->head; current; current = current->next) {
        if (current->type == GROUP && current->data.group) {
            for (ShapeNode* sub = current->data.group->shapes->head; sub; sub = sub->next) {
                if (sub->data.ptr == node->data.ptr)
                    return current->data.group->name;
            }
        }
    }
    return NULL;
}

void display_list(const ShapeList* list) {
    if (!list || list->size == 0) {
        move_cursor(19, 37); printf(ORANGE"╭────────────────────────────────╮");
        move_cursor(20, 35); printf(    "->│ No shapes or groups to display │");
        move_cursor(21, 37); printf(      "╰────────────────────────────────╯"RESET);
        move_cursor(20, 32);
        temp_message(ORANGE"5", 0, 2);
        return;
    }

    system("clear");
    printf(SEA"╔════════════════════════════════════════╗\n"
              "║"RESET BOLD"              GLOBAL  LIST              "RESET SEA"║\n"
              "╚════════════════════════════════════════╝\n");
    printf("╭────────────────────────────────────────╮\n");

    int idx = 0;
    for (ShapeNode* current = list->head; current; current = current->next, idx++) {
        printf(SEA"│"RESET" [%d] ", idx);
        switch (current->type) {
            case CIRCLE:    printf("Circle"); break;
            case SQUARE:    printf("Square"); break;
            case RECTANGLE: printf("Rectangle"); break;
            case LINE:      printf("Line"); break;
            case ELLIPSE:   printf("Ellipse"); break;
            case POLYGON:   printf("Polygon"); break;
            case MULTILINE: printf("Multiline"); break;
            case PATH:      printf("Path"); break;
            case GROUP:
                printf("Group %s (×%d item%s)", 
                    current->data.group->name,
                    current->data.group->shapes ? current->data.group->shapes->size : 0,
                    (current->data.group->shapes && current->data.group->shapes->size > 1) ? "s" : "");
                break;
        }

        if (current->type != GROUP) {
            const char* group_name = get_group_name_for_shape(list, current);
            if (group_name) printf(" (-> %s)", group_name);
        }
        printf("\n");
    }

    for (int i = 0; i < list->size; i++) {
        move_cursor(5 + i, 42);
        printf(SEA"│\n");
    }

    printf(SEA"╰────────────────────────────────────────╯\n"
           CYAN"╭────────────────────────────────────────╮\n"
               "│ "GRAY BOLD"Enter ↵  "RESET"Return to Main Menu           "CYAN"│\n"
               "╰────────────────────────────────────────╯\n\n");
}

void ask_list(const ShapeList* list) {
    if (!list || list->size == 0) {
        move_cursor(19, 37); printf(ORANGE"╭────────────────────────────────╮");
        move_cursor(20, 35); printf(    "->│ No shapes or groups to display │");
        move_cursor(21, 37); printf(      "╰────────────────────────────────╯"RESET);
        move_cursor(20, 32);
        temp_message(ORANGE"5", 0, 2);
        return;
    }

    display_list(list);
    int lines;
    lines = list->size + 8;
    int idx;
    if (list->size < 11) {
        move_cursor(lines + 1, 1); printf(SEA"╭─────────────────────────────────╮  ╭───╮\n");
	    move_cursor(lines + 2, 1); printf(   "│ "RESET"Index of element to see details "SEA"│->│   │\n");
	    move_cursor(lines + 3, 1); printf(SEA"╰─────────────────────────────────╯  ╰───╯");
	    move_cursor(lines + 2, 40);
        if (get_integer(&idx, "", 1, 0, list->size - 1, false)) return;
    } else {
        move_cursor(lines + 1, 1); printf(SEA"╭─────────────────────────────────╮  ╭────╮\n");
	    move_cursor(lines + 2, 1); printf(   "│ "RESET"Index of element to see details "SEA"│->│    │\n");
	    move_cursor(lines + 3, 1); printf(SEA"╰─────────────────────────────────╯  ╰────╯");
	    move_cursor(lines + 2, 40);
        if (get_integer(&idx, "", 2, 0, 9, false)) return;
    }

    ShapeNode* node = get_node_at((ShapeList*)list, idx);
    if (!node) return; 

    if (node->type == GROUP)
        display_group_details(node, node->data.group, idx, (ShapeList*)list);
    else
        display_shape_details(node, idx, (ShapeList*)list);
    
    ask_list(list);
}

int display_group_info(const Group* group, int idx) {
    if (!group || !group->shapes) return 0;

    if (idx < 10) {
        printf(SEA"╔════════════════════════════════════════╗\n"
                  "║           "RESET BOLD"[%d] GROUP  DETAILS"SEA"           ║\n"
                  "╚════════════════════════════════════════╝\n", idx);
    } else {
        printf(SEA"╔════════════════════════════════════════╗\n"
                  "║           "RESET BOLD"[%d] GROUP DETAILS"SEA"           ║\n"
                  "╚════════════════════════════════════════╝\n", idx);
    }

    printf(SEA"╭────────────────────┬───────────────────╮\n"
              "│ "PINK"Group %-12s "SEA"│ "RESET"(×%d item%s)\n"
           SEA"├────────────────────┴───────────────────┤\n",
       group->name,
       group->shapes ? group->shapes->size : 0,
       (group->shapes && group->shapes->size > 1) ? "s" : "");

    int sub_idx = 0;
    for (ShapeNode* sub = group->shapes->head; sub; sub = sub->next, sub_idx++)
        printf(SEA"│ "RESET"[%d] %-10s\n", sub_idx, get_shape_type(sub->type));

    printf(SEA"╰────────────────────────────────────────╯\n");
    move_cursor(5, 42); printf(SEA"│");
    for (int i = 0; i < group->shapes->size; i++) {
        move_cursor(7 + i, 42);
        printf(SEA"│");
    }
    return 2 + group->shapes->size;
}

void display_group_details(ShapeNode* node, Group* group, int idx, ShapeList* list) {
    if (!group || !group->shapes) return;
    system("clear");

    display_group_info(group, idx);

    printf(SEA"\n├────────────────────────────────────────┤\n"
                "│ "RED BOLD"1 ➤ "RESET BOLD"Delete"RESET" this Group                  "SEA"│\n"
                "│ "MAGENTA BOLD"2 ➤ "RESET BOLD"Edit"RESET" this Group                    "SEA"│\n"
                "│ "SEA BOLD"3 ➤ "RESET BOLD"Details"RESET" of a Shape/Group (soon)    "SEA"│\n"
                "│ "GRAY BOLD"Enter ↵  "RESET"Return"RESET" to Previous Menu       "SEA"│\n"
                "╰────────────────────────────────────────╯");

    InputStatus status;
    int choice;
    int lines;
    lines = group->shapes->size + 12;
    move_cursor(lines + 1, 1); printf(SEA"╭─────────────────────────╮  ╭───╮\n");
	move_cursor(lines + 2, 1); printf(   "│ "RESET"Select an option (1->3) "SEA"│->│   │\n");
	move_cursor(lines + 3, 1); printf(SEA"╰─────────────────────────╯  ╰───╯");
	move_cursor(lines + 2, 32);

	fflush(stdout);

	status = get_integer(&choice, "", 1, 1, 3, false);

    if (status == INPUT_CANCEL) {
        system("clear");
        return;
    }

    switch (choice) {
        case 1:
            delete_shape_or_group(list, get_node_at(list, idx), idx, group->shapes->size, true);
            break;
        case 2:
            edit_node(node, list, 44, idx, true);
            display_group_details(node, group, idx, list);
            break;
        case 3: {
            if (group->shapes->size == 0) {
                move_cursor(lines + 1, 37); printf(ORANGE"╭────────────────────────────────╮");
                move_cursor(lines + 2, 35); printf(    "->│ No shapes or groups to display │");
                move_cursor(lines + 3, 37); printf(      "╰────────────────────────────────╯"RESET);
                move_cursor(lines + 2, 32);
                temp_message(ORANGE"3", 0, 2);
                return;
            }

            printf(GRAY"\nEnter ↵ to cancel\n"RESET);
            int selected;
            if (group->shapes->size <11) {
                move_cursor(lines + 4, 1); printf(SEA"╭─────────────────────────────────╮  ╭───╮\n");
	            move_cursor(lines + 5, 1); printf(   "│ "RESET"Index of element to see details "SEA"│->│   │\n");
	            move_cursor(lines + 6, 1); printf(SEA"╰─────────────────────────────────╯  ╰───╯");
			    move_cursor(lines + 5, 40);
			    status = get_integer(&selected, "", 1, 0, group->shapes->size - 1, false);
            } else {
				move_cursor(lines + 4, 1); printf(SEA"╭─────────────────────────────────╮  ╭────╮\n");
	            move_cursor(lines + 5, 1); printf(   "│ "RESET"Index of element to see details "SEA"│->│    │\n");
	            move_cursor(lines + 6, 1); printf(SEA"╰─────────────────────────────────╯  ╰────╯");
			    move_cursor(lines + 5, 40);
				status = get_integer(&selected, "", 2, 0, 9, false);
                }
            if (status == INPUT_CANCEL) {
                system("clear");
                display_group_details(node, group, idx, list);
                return;
            }

            ShapeNode* selected_node = get_node_at(group->shapes, selected);
            if (!selected_node) {
                move_cursor(lines + 1, 46); printf(ORANGE"╭──────────────╮");
                move_cursor(lines + 2, 44); printf(    "->│ Invaid Index │");
                move_cursor(lines + 3, 46); printf(      "╰──────────────╯"RESET);
                move_cursor(lines + 2, 40);
                printf("%d", selected);
                temp_message("", 0, 2);
                display_group_details(node, group, idx, list);
                return;
            }

            if (selected_node->type == GROUP)
                display_group_details(selected_node, selected_node->data.group, selected, group->shapes);
            else
                display_shape_details(selected_node, selected, group->shapes);

            display_group_details(node, group, idx, list);
            break;
        }
    }
}

void display_shape_details(ShapeNode* node, int idx, ShapeList* list) {
    if (!node) return;
    system("clear");

    int details_lines = display_shape_info(node, list, idx); 

    printf(SEA"\n├────────────────────────────────────────┤\n"
                "│ "RED BOLD"1 ➤ "RESET BOLD"Delete"RESET" this Shape                  "SEA"│\n"
                "│ "MAGENTA BOLD"2 ➤ "RESET BOLD"Edit"RESET" this Shape                    "SEA"│\n"
                "│ "GRAY BOLD"Enter ↵  "RESET BOLD"Return"RESET" to Previous Menu       "SEA"│\n"
                "╰────────────────────────────────────────╯\n");

    InputStatus status;
    int choice;
    int lines;
    lines = details_lines + 9;
    move_cursor(lines + 1, 1); printf(SEA"╭─────────────────────────╮  ╭───╮\n");
	move_cursor(lines + 2, 1); printf(   "│ "RESET"Select an option (1->2) "SEA"│->│   │\n");
	move_cursor(lines + 3, 1); printf(SEA"╰─────────────────────────╯  ╰───╯");
	move_cursor(lines + 2, 32);
    status = get_integer(&choice, "", 1, 1, 2, false);

    if (status == INPUT_CANCEL) {
        system("clear");
        return;
    }

    if (choice == 1) {
        delete_shape_or_group(list, node, idx, details_lines, true);
    } else if (choice == 2) {
        edit_node(node, list, 44, idx, true);
        display_shape_details(node, idx, list);
    }
}

int display_shape_info(const ShapeNode* node, const ShapeList* list, int idx) {
    if (!node) return 0;
    int details_lines = 0; 
    const char* title = ""; 

    switch (node->type) {
        case CIRCLE:    
            title = "CIRCLE"; 
            details_lines = 7; 
            break;
        case SQUARE:    
            title = "SQUARE"; 
            details_lines = 8; 
            break;
        case RECTANGLE: 
            title = "RECTANGLE"; 
            details_lines = 8; 
            break;
        case LINE:      
            title = "LINE"; 
            details_lines = 6; 
            break;
        case ELLIPSE:   
            title = "ELLIPSE"; 
            details_lines = 7; 
            break;
        case POLYGON:
            title = "POLYGON";
            details_lines = (node->data.polygon->nb_points + 2) / 3 + 7; 
           break;
        case MULTILINE:
            title = "MULTILINE";
            details_lines = (node->data.multiline->nb_points + 2) / 3 + 7;
           break;
        case PATH:
            title = "PATH";
            details_lines = node->data.path->nb_points + 8;
            for (int i = 1; i < node->data.path->nb_points; i++) {
                PathPoint p = node->data.path->points[i];
                if (p.type == SEGMENT_QUADRATIC_BEZIER) details_lines += 1; // C1
                if (p.type == SEGMENT_CUBIC_BEZIER) details_lines += 1;     // C1 + C2
                if (p.type == SEGMENT_SMOOTH_QUADRATIC) details_lines += 1;
                if (p.type == SEGMENT_SMOOTH_CUBIC) details_lines += 1;
                if (p.type == SEGMENT_ARC) details_lines += 1;
            }
            break;
        case GROUP:
                break;
    }

    printf(SEA"╔════════════════════════════════════════╗\n"); 

    char dynamic_text[60];
    int dynamic_len = snprintf(dynamic_text, sizeof(dynamic_text), "[%d] %s DETAILS", idx, title);
    const int INNER_WIDTH = 40; 
    int total_padding = INNER_WIDTH - dynamic_len;
    int padding_left = total_padding / 2;
    int padding_right = total_padding - padding_left;

    printf(SEA"║%*s"RESET BOLD"%s"RESET SEA"%*s║\n", 
           padding_left, "", dynamic_text, padding_right, ""); 

    printf(SEA"╚════════════════════════════════════════╝\n");
    printf(SEA"╭────────────────────────────────────────╮\n");

    const char* group_name = get_group_name_for_shape(list, node);
    
    switch (node->type) {
        case CIRCLE:
            if (group_name) printf(SEA"│"RESET" In Group : %s\n", group_name);
            else printf(SEA"│"RESET" Not in Group\n");
            printf(SEA"│"RESET" Center = (%d, %d)\n", node->data.circle->x, node->data.circle->y);
            printf(SEA"│"RESET" Radius = %d\n", node->data.circle->radius);
            printf(SEA"│"RESET" Stroke Color = %s\n", node->data.circle->stroke_color);
            printf(SEA"│"RESET" Fill Color = %s\n", node->data.circle->fill_color);
            printf(SEA"│"RESET" Thickness = %d\n", node->data.circle->thickness);
            printf(SEA"│"RESET" Scale = x=%d y=%d\n", node->data.circle->scalex, node->data.circle->scaley);
            break;

        case SQUARE:
            if (group_name) printf(SEA"│"RESET" In Group : %s\n", group_name);
            else printf(SEA"│"RESET" Not in Group\n");
            printf(SEA"│"RESET" Starting Point = (%d, %d)\n", node->data.square->x, node->data.square->y);
            printf(SEA"│"RESET" Side Length = %d\n", node->data.square->side);
            printf(SEA"│"RESET" Stroke Color = %s\n", node->data.square->stroke_color);
            printf(SEA"│"RESET" Fill Color = %s\n", node->data.square->fill_color);
            printf(SEA"│"RESET" Thickness = %d\n", node->data.square->thickness);
            printf(SEA"│"RESET" Rounded = %d\n", node->data.square->rounded);
            printf(SEA"│"RESET" Scale = x=%d y=%d\n", node->data.square->scalex, node->data.square->scaley);
            break;

        case RECTANGLE:
            if (group_name) printf(SEA"│"RESET" In Group : %s\n", group_name);
            else printf(SEA"│"RESET" Not in Group\n");
            printf(SEA"│"RESET" Starting Point = (%d, %d)\n", node->data.rectangle->x, node->data.rectangle->y);
            printf(SEA"│"RESET" Size = %dx%d\n", node->data.rectangle->width, node->data.rectangle->height);
            printf(SEA"│"RESET" Stroke Color = %s\n", node->data.rectangle->stroke_color);
            printf(SEA"│"RESET" Fill Color = %s\n", node->data.rectangle->fill_color);
            printf(SEA"│"RESET" Thickness = %d\n", node->data.rectangle->thickness);
            printf(SEA"│"RESET" Rounded = %d\n", node->data.rectangle->rounded);
            printf(SEA"│"RESET" Scale = x=%d y=%d\n", node->data.rectangle->scalex, node->data.rectangle->scaley);
            break;

        case LINE:
            if (group_name) printf(SEA"│"RESET" In Group : %s\n", group_name);
            else printf(SEA"│"RESET" Not in Group\n");
            printf(SEA"│"RESET" Starting Point = (%d, %d)\n", node->data.line->x1, node->data.line->y1);
            printf(SEA"│"RESET" Ending Point = (%d, %d)\n", node->data.line->x2, node->data.line->y2);
            printf(SEA"│"RESET" Stroke Color = %s\n", node->data.line->stroke_color);
            printf(SEA"│"RESET" Thickness = %d\n", node->data.line->thickness);
            printf(SEA"│"RESET" Scale = x=%d y=%d\n", node->data.line->scalex, node->data.line->scaley);
            break;

        case ELLIPSE:
            if (group_name) printf(SEA"│"RESET" In Group : %s\n", group_name);
            else printf(SEA"│"RESET" Not in Group\n");
            printf(SEA"│"RESET" Centre = (%d, %d)\n", node->data.ellipse->x, node->data.ellipse->y);
            printf(SEA"│"RESET" Radius = x=%d y=%d\n", node->data.ellipse->radiusx, node->data.ellipse->radiusy);
            printf(SEA"│"RESET" Stroke Color = %s\n", node->data.ellipse->stroke_color);
            printf(SEA"│"RESET" Fill Color = %s\n", node->data.ellipse->fill_color);
            printf(SEA"│"RESET" Thickness = %d\n", node->data.ellipse->thickness);
            printf(SEA"│"RESET" Scale = x=%d y=%d\n", node->data.ellipse->scalex, node->data.ellipse->scaley);
            break;

        case POLYGON:
            if (group_name) printf(SEA"│"RESET" In Group : %s\n", group_name);
            else printf(SEA"│"RESET" Not in Group\n");
            printf(SEA"│"RESET" Points:\n");
            printf(SEA"│"RESET" ");
            for (int i = 0; i < node->data.polygon->nb_points; i++) {
                printf("(%d, %d) ", node->data.polygon->points[i].x, node->data.polygon->points[i].y);
                bool is_end_of_line = (i + 1) % 3 == 0;
                bool is_last_point = i == node->data.polygon->nb_points - 1;
                if (is_end_of_line && !is_last_point) {
                    printf("\n"SEA"│"RESET" ");
                }
            }
            printf(SEA"\n│\n");
            printf(SEA"│"RESET" Stroke Color = %s\n", node->data.polygon->stroke_color);
            printf(SEA"│"RESET" Fill Color = %s\n", node->data.polygon->fill_color);
            printf(SEA"│"RESET" Thickness = %d\n", node->data.polygon->thickness);
            printf(SEA"│"RESET" Scale = x=%d y=%d\n", node->data.polygon->scalex, node->data.polygon->scaley);
            break;

        case MULTILINE:
            if (group_name) printf(SEA"│"RESET" In Group : %s\n", group_name);
            else printf(SEA"│"RESET" Not in Group\n");
            printf(SEA"│"RESET" Points:\n");
            printf(SEA"│"RESET" ");
            for (int i = 0; i < node->data.multiline->nb_points; i++) {
                printf("(%d, %d) ", node->data.multiline->points[i].x, node->data.multiline->points[i].y);
                bool is_end_of_line = (i + 1) % 3 == 0;
                bool is_last_point = i == node->data.multiline->nb_points - 1;
                if (is_end_of_line && !is_last_point) {
                    printf("\n"SEA"│"RESET" ");
                }
            }
            printf(SEA"\n│\n");
            printf(SEA"│"RESET" Stroke Color = %s\n", node->data.multiline->stroke_color);
            printf(SEA"│"RESET" Thickness = %d\n", node->data.multiline->thickness);
            printf(SEA"│"RESET" Scale = x=%d y=%d\n", node->data.multiline->scalex, node->data.multiline->scaley);
            break;
            
        case PATH:
            if (group_name) printf(SEA"│"RESET" In Group : %s\n", group_name);
            else printf(SEA"│"RESET" Not in Group\n");
            printf(SEA"│"RESET" Closed: %s\n", node->data.path->closed ? "Yes" : "No");
            printf(SEA"│"RESET" Stroke Color = %s\n", node->data.path->stroke_color);
            printf(SEA"│"RESET" Fill Color = %s\n", node->data.path->fill_color);
            printf(SEA"│"RESET" Thickness = %d\n", node->data.path->thickness);
            printf(SEA"│"RESET" Scale = x=%d y=%d\n", node->data.path->scalex, node->data.path->scaley);

            printf(SEA"│"RESET"\n");
            printf(SEA"│"RESET" Segments:\n");

            for (int i = 0; i < node->data.path->nb_points; i++) {
                PathPoint p = node->data.path->points[i];

                if (i == 0) {
                    printf(SEA"│"RESET" [%d] Start (M/m): (%d, %d)\n", i+1, p.x, p.y);
                    continue;
                }

                const char* type_str = "";
                char extra[64] = "";

                switch (p.type) {
                    case SEGMENT_LINE: type_str = "Line"; break;
                    case SEGMENT_HORIZONTAL: type_str = "Horizontal"; break;
                    case SEGMENT_VERTICAL: type_str = "Vertical"; break;
                    case SEGMENT_QUADRATIC_BEZIER: 
                        type_str = "Quadratic Bézier";
                        snprintf(extra, sizeof(extra), SEA"\n│"RESET"     C1=(%d,%d)", p.control1.x, p.control1.y);
                        break;
                    case SEGMENT_CUBIC_BEZIER:
                        type_str = "Cubic Bézier";
                        snprintf(extra, sizeof(extra), SEA"\n│"RESET"     C1=(%d,%d) C2=(%d,%d)", 
                            p.control1.x, p.control1.y, p.control2.x, p.control2.y);
                        break;
                    case SEGMENT_SMOOTH_QUADRATIC: 
                        type_str = "Smooth Quadratic";
                        snprintf(extra, sizeof(extra), SEA"\n│"RESET"     End=(%d,%d)", p.x, p.y);
                    break;
                    case SEGMENT_SMOOTH_CUBIC:
                        type_str = "Smooth Cubic";
                        snprintf(extra, sizeof(extra), SEA"\n│"RESET"     C1=(%d,%d) End=(%d,%d)", p.control1.x, p.control1.y, p.x, p.y);
                        break;
                    case SEGMENT_ARC:
                        type_str = "Arc";
                        snprintf(extra, sizeof(extra), SEA"\n│"RESET"     Rx=%d Ry=%d Rot=%d L=%d S=%d", 
                            p.radiusX, p.radiusY, p.xAxisRotation, p.largeArcFlag, p.sweepFlag);
                        break;
                    case SEGMENT_CLOSE:
                        type_str = "Close Path";
                        break;
                    default:
                        type_str = "Unknown";
                        break;
                }

                if (strlen(extra) > 0) {
                    printf(SEA"│"RESET" [%d] %s → End=(%d,%d) %s\n", i+1, type_str, p.x, p.y, extra);
                } else if (p.type != SEGMENT_CLOSE) {
                    printf(SEA"│"RESET" [%d] %s → End=(%d,%d)\n", i+1, type_str, p.x, p.y);
                } else {
                    printf(SEA"│"RESET" [%d] %s\n", i+1, type_str);
                }
            }
            break;
        case GROUP:
            break;
    }
    
    printf(SEA"╰────────────────────────────────────────╯\n");

    for (int i = 0; i < details_lines; i++) {
        move_cursor(5 + i, 42);
        printf(SEA"│");
    }
    return details_lines;
}