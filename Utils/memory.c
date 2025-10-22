#include <stdlib.h>
#include "memory.h"
#include "group.h"

void free_shape_node(ShapeNode* node) {
    if (!node) return;

    switch (node->type) {
        case CIRCLE:    
            free(node->data.circle);
            break;
        case SQUARE:    
            free(node->data.square);
            break;
        case RECTANGLE: 
            free(node->data.rectangle);
            break;
        case LINE:      
            free(node->data.line);
            break;
        case ELLIPSE:   
            free(node->data.ellipse);
            break;
        case POLYGON:
            if (node->data.polygon) {
                free(node->data.polygon->points);
                free(node->data.polygon);
            }
            break;
        case MULTILINE:
            if (node->data.multiline) {
                free(node->data.multiline->points);
                free(node->data.multiline);
            }
            break;
        case PATH:
            if (node->data.path) {
                free(node->data.path->points);
                free(node->data.path);
            }
            break;
        case GROUP:
            if (node->data.group) {
                free(node->data.group->name);
                if (node->data.group->shapes)
                    free_group_shape_list(node->data.group->shapes);
                free(node->data.group);
            }
            break;
        default:
            break;
    }
}

void free_all_shapes(ShapeList* list) {
    if (!list) return;

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