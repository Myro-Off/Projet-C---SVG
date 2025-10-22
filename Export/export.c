#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>
#include <time.h>
#include <unistd.h>

#include "export.h"
#include "structures.h"
#include "utils.h"

static void calculate_viewport(const ShapeList* list, int* xmin, int* ymin, int* xmax, int* ymax) {
    if (!list || list->size == 0) {
        *xmin = 0;
        *ymin = 0;
        *xmax = 800;
        *ymax = 600;
        return;
    }

    *xmin = INT_MAX;
    *ymin = INT_MAX;
    *xmax = INT_MIN;
    *ymax = INT_MIN;

    ShapeNode* current = list->head;
    while (current) {
        switch (current->type) {
            case CIRCLE: {
                int x1 = current->data.circle->x - current->data.circle->radius;
                int y1 = current->data.circle->y - current->data.circle->radius;
                int x2 = current->data.circle->x + current->data.circle->radius;
                int y2 = current->data.circle->y + current->data.circle->radius;
                if (x1 < *xmin) *xmin = x1;
                if (y1 < *ymin) *ymin = y1;
                if (x2 > *xmax) *xmax = x2;
                if (y2 > *ymax) *ymax = y2;
                break;
            }
            case SQUARE: {
                int x1 = current->data.square->x;
                int y1 = current->data.square->y;
                int x2 = x1 + current->data.square->side;
                int y2 = y1 + current->data.square->side;
                if (x1 < *xmin) *xmin = x1;
                if (y1 < *ymin) *ymin = y1;
                if (x2 > *xmax) *xmax = x2;
                if (y2 > *ymax) *ymax = y2;
                break;
            }
            case RECTANGLE: {
                int x1 = current->data.rectangle->x;
                int y1 = current->data.rectangle->y;
                int x2 = x1 + current->data.rectangle->width;
                int y2 = y1 + current->data.rectangle->height;
                if (x1 < *xmin) *xmin = x1;
                if (y1 < *ymin) *ymin = y1;
                if (x2 > *xmax) *xmax = x2;
                if (y2 > *ymax) *ymax = y2;
                break;
            }
            case LINE: {
                int x1 = current->data.line->x1;
                int y1 = current->data.line->y1;
                int x2 = current->data.line->x2;
                int y2 = current->data.line->y2;
                if (x1 < *xmin) *xmin = x1;
                if (y1 < *ymin) *ymin = y1;
                if (x2 > *xmax) *xmax = x2;
                if (y2 > *ymax) *ymax = y2;
                break;
            }
            case ELLIPSE: {
                int x1 = current->data.ellipse->x - current->data.ellipse->radiusx;
                int y1 = current->data.ellipse->y - current->data.ellipse->radiusy;
                int x2 = current->data.ellipse->x + current->data.ellipse->radiusx;
                int y2 = current->data.ellipse->y + current->data.ellipse->radiusy;
                if (x1 < *xmin) *xmin = x1;
                if (y1 < *ymin) *ymin = y1;
                if (x2 > *xmax) *xmax = x2;
                if (y2 > *ymax) *ymax = y2;
                break;
            }
            case POLYGON: {
                for (int i = 0; i < current->data.polygon->nb_points; i++) {
                    int x = current->data.polygon->points[i].x;
                    int y = current->data.polygon->points[i].y;
                    if (x < *xmin) *xmin = x;
                    if (y < *ymin) *ymin = y;
                    if (x > *xmax) *xmax = x;
                    if (y > *ymax) *ymax = y;
                }
                break;
            }
            case MULTILINE: {
                for (int i = 0; i < current->data.multiline->nb_points; i++) {
                    int x = current->data.multiline->points[i].x;
                    int y = current->data.multiline->points[i].y;
                    if (x < *xmin) *xmin = x;
                    if (y < *ymin) *ymin = y;
                    if (x > *xmax) *xmax = x;
                    if (y > *ymax) *ymax = y;
                }
                break;
            }
            case PATH: {
                for (int i = 0; i < current->data.path->nb_points; i++) {
                    PathPoint p = current->data.path->points[i];
                    
                    if (p.type != SEGMENT_CLOSE && p.type != SEGMENT_HORIZONTAL && p.type != SEGMENT_VERTICAL) {
                        int x = p.x;
                        int y = p.y;
                        
                        if (x < *xmin) *xmin = x;
                        if (y < *ymin) *ymin = y;
                        if (x > *xmax) *xmax = x;
                        if (y > *ymax) *ymax = y;
                        
                        if (p.type == SEGMENT_CUBIC_BEZIER || p.type == SEGMENT_SMOOTH_CUBIC) {
                            if (p.control1.x < *xmin) *xmin = p.control1.x;
                            if (p.control1.y < *ymin) *ymin = p.control1.y;
                            if (p.control1.x > *xmax) *xmax = p.control1.x;
                            if (p.control1.y > *ymax) *ymax = p.control1.y;
                            
                            if (p.control2.x < *xmin) *xmin = p.control2.x;
                            if (p.control2.y < *ymin) *ymin = p.control2.y;
                            if (p.control2.x > *xmax) *xmax = p.control2.x;
                            if (p.control2.y > *ymax) *ymax = p.control2.y;
                        }
                    } 
                    else if (p.type == SEGMENT_HORIZONTAL) {
                        if (p.x < *xmin) *xmin = p.x;
                        if (p.x > *xmax) *xmax = p.x;
                    }
                    else if (p.type == SEGMENT_VERTICAL) {
                        if (p.y < *ymin) *ymin = p.y;
                        if (p.y > *ymax) *ymax = p.y;
                    }
                }
                break;
            }
            default: break;
        }
        current = current->next;
    }

    // Ajouter une marge
    *xmin -= 10;
    *ymin -= 10;
    *xmax += 10;
    *ymax += 10;

    // Assurer la taille minimum
    if (*xmin > 0) *xmin = 0;
    if (*ymin > 0) *ymin = 0;
    if (*xmax < 800) *xmax = 800;
    if (*ymax < 600) *ymax = 600;
}

void export_svg_circle(FILE* f, const Circle* c) {
    fprintf(f,
        "    <circle cx='%d' cy='%d' r='%d' fill='%s' stroke='%s' stroke-width='%d' transform='scale(%d, %d)'/>\n",
        c->x, c->y, c->radius, c->fill_color, c->stroke_color, c->thickness, c-> scalex, c-> scaley
    );
}

void export_svg_square(FILE* f, const Square* s) {
    fprintf(f,
        "    <rect x='%d' y='%d' width='%d' height='%d' fill='%s' stroke='%s' stroke-width='%d' rx='%d' ry='%d' transform='scale(%d, %d)'/>\n",
        s->x, s->y, s->side, s->side, s->fill_color, s->stroke_color, s->thickness, s->rounded, s->rounded, s-> scalex, s-> scaley
    );
}

void export_svg_rectangle(FILE* f, const Rectangle* r) {
    fprintf(f,
        "    <rect x='%d' y='%d' width='%d' height='%d' fill='%s' stroke='%s' stroke-width='%d' rx='%d' ry='%d' transform='scale(%d, %d)'/>\n",
        r->x, r->y, r->width, r->height, r->fill_color, r->stroke_color, r->thickness, r->rounded, r->rounded, r-> scalex, r-> scaley
    );
}

void export_svg_line(FILE* f, const Line* l) {
    fprintf(f,
        "    <line x1='%d' y1='%d' x2='%d' y2='%d' stroke='%s' stroke-width='%d' transform='scale(%d, %d)'/>\n",
        l->x1, l->y1, l->x2, l->y2, l->stroke_color, l->thickness, l-> scalex, l-> scaley
    );
}

void export_svg_ellipse(FILE* f, const Ellipse* e) {
    fprintf(f,
        "    <ellipse cx='%d' cy='%d' rx='%d' ry='%d' fill='%s' stroke='%s' stroke-width='%d' transform='scale(%d, %d)'/>\n",
        e->x, e->y, e->radiusx, e->radiusy, e->fill_color, e->stroke_color, e->thickness, e-> scalex, e-> scaley
    );
}

void export_svg_polygon(FILE* f, const Polygon* p) {
    fprintf(f, "    <polygon points='");
    for (int i = 0; i < p->nb_points; i++) {
        fprintf(f, "%d,%d ", p->points[i].x, p->points[i].y);
    }
    fprintf(f, "' fill='%s' stroke='%s' stroke-width='%d' transform='scale(%d, %d)'/>\n",
        p->fill_color, p->stroke_color, p->thickness, p-> scalex, p-> scaley
    );
}

void export_svg_multiline(FILE* f, const Multiline* m) {
    fprintf(f, "    <multiline points='");
    for (int i = 0; i < m->nb_points; i++) {
        fprintf(f, "%d,%d ", m->points[i].x, m->points[i].y);
    }
    fprintf(f, "' fill='none' stroke='%s' stroke-width='%d' transform='scale(%d, %d)'/>\n",
        m->stroke_color, m->thickness, m-> scalex, m-> scaley
    );
}

void export_svg_path(FILE* f, const Path* p) {
    if (p->nb_points < 1) return;

    fprintf(f, "    <path d='");

    for (int i = 0; i < p->nb_points; i++) {
        PathPoint current_point = p->points[i];
        char command;
        char format_str[16];
        if (current_point.coord_type == COORD_ABSOLUTE) {
             strcpy(format_str, " %c");
        } else {
             strcpy(format_str, " %c");
        }

        switch (current_point.type) {
            case SEGMENT_MOVE:
                command = 'M';
                if (current_point.coord_type == COORD_RELATIVE) command = 'm';
                fprintf(f, format_str, command);
                fprintf(f, "%d %d", current_point.x, current_point.y);
                break;
                
            case SEGMENT_LINE:
                command = 'L';
                if (current_point.coord_type == COORD_RELATIVE) command = 'l';
                fprintf(f, format_str, command);
                fprintf(f, "%d %d", current_point.x, current_point.y);
                break;
                
            case SEGMENT_HORIZONTAL:
                command = 'H';
                if (current_point.coord_type == COORD_RELATIVE) command = 'h';
                fprintf(f, format_str, command);
                fprintf(f, "%d", current_point.x);
                break;

            case SEGMENT_VERTICAL:
                command = 'V';
                if (current_point.coord_type == COORD_RELATIVE) command = 'v';
                fprintf(f, format_str, command);
                fprintf(f, "%d", current_point.y);
                break;

            case SEGMENT_QUADRATIC_BEZIER:
                command = 'Q';
                if (current_point.coord_type == COORD_RELATIVE) command = 'q';
                fprintf(f, format_str, command);
                // Q cx cy, x y
                fprintf(f, "%d %d, %d %d", 
                    current_point.control1.x, current_point.control1.y,
                    current_point.x, current_point.y);
                break;
                
            case SEGMENT_CUBIC_BEZIER:
                command = 'C';
                if (current_point.coord_type == COORD_RELATIVE) command = 'c';
                fprintf(f, format_str, command);
                fprintf(f, "%d %d, %d %d, %d %d", 
                    current_point.control1.x, current_point.control1.y,
                    current_point.control2.x, current_point.control2.y,
                    current_point.x, current_point.y);
                break;
                
            case SEGMENT_SMOOTH_QUADRATIC:
                command = 'T';
                if (current_point.coord_type == COORD_RELATIVE) command = 't';
                fprintf(f, format_str, command);
                // T x y
                fprintf(f, "%d %d", current_point.x, current_point.y);
                break;
                
            case SEGMENT_SMOOTH_CUBIC:
                command = 'S';
                if (current_point.coord_type == COORD_RELATIVE) command = 's';
                fprintf(f, format_str, command);
                fprintf(f, "%d %d, %d %d", 
                    current_point.control2.x, current_point.control2.y,
                    current_point.x, current_point.y);
                break;
                
            case SEGMENT_ARC:
                command = 'A';
                if (current_point.coord_type == COORD_RELATIVE) command = 'a';
                fprintf(f, format_str, command);
                fprintf(f, "%d %d %d %d %d %d %d", 
                    current_point.radiusX, current_point.radiusY, 
                    current_point.xAxisRotation, 
                    current_point.largeArcFlag ? 1 : 0, 
                    current_point.sweepFlag ? 1 : 0, 
                    current_point.x, current_point.y);
                break;
                
            case SEGMENT_CLOSE:
                command = 'Z';
                if (current_point.coord_type == COORD_RELATIVE) command = 'z';
                fprintf(f, format_str, command);
                break;
        }
    }
    
    fprintf(f, 
        "' fill='%s' stroke='%s' stroke-width='%d' transform='scale(%d, %d)'/>\n",
        p->fill_color, p->stroke_color, p->thickness, p->scalex, p->scaley
    );
}

void export_svg_all(const char* filename, ShapeList* list) {
    if (!list || list->size == 0) {
        move_cursor(19, 37);
        printf(ORANGE"╭────────────────────────────────────────╮");
        move_cursor(20, 35);
        printf(    "->│ Please create a shape before exporting │");
        move_cursor(21, 37);
        printf(      "╰────────────────────────────────────────╯");
        temp_message("", 0, 2);
            return;
        }
    system("rm Export/export.svg");

    move_cursor(19, 37);
    printf(ORANGE"╭─────────────────────────────────────────────╮");
    move_cursor(20, 35);
    printf(    "->│                                             │");
    move_cursor(21, 37);
    printf(      "╰─────────────────────────────────────────────╯");

    FILE* f = fopen(filename, "w");
    if (!f) {
        move_cursor(19, 37);
        printf(ORANGE"╭─────────────────────────╮");
        move_cursor(20, 35);
        printf(    "->│ Error creating SVG file │");
        move_cursor(21, 37);
        printf(      "╰─────────────────────────╯");
        temp_message("", 0, 2);
        return;
    }

    int xmin, ymin, xmax, ymax;
    calculate_viewport(list, &xmin, &ymin, &xmax, &ymax);

    int width = xmax - xmin;
    int height = ymax - ymin;

    fprintf(f, "<svg xmlns='http://www.w3.org/2000/svg' width='%d' height='%d' viewBox='%d %d %d %d'>\n",
            width, height, xmin, ymin, width, height);

    ShapeNode* current = list->head;
    while (current) {
        switch (current->type) {
            case CIRCLE:    export_svg_circle(f, current->data.circle); break;
            case SQUARE:    export_svg_square(f, current->data.square); break;
            case RECTANGLE: export_svg_rectangle(f, current->data.rectangle); break;
            case LINE:      export_svg_line(f, current->data.line); break;
            case ELLIPSE:   export_svg_ellipse(f, current->data.ellipse); break;
            case POLYGON:   export_svg_polygon(f, current->data.polygon); break;
            case MULTILINE: export_svg_multiline(f, current->data.multiline); break;
            case PATH:      export_svg_path(f, current->data.path); break;
            default: break;
        }
        current = current->next;
    }
    fprintf(f, "</svg>\n");
    fclose(f);

    const int steps = 100;
    for (int i = 0; i <= steps; ++i) {
        double frac = (double)i / steps;
        draw_progress(frac);
        usleep(60000);
    }
    move_cursor(19, 37);
    printf(GREEN"╭─────────────────────────────────────────────╮");
    move_cursor(20, 35);
    printf(   "->│ SVG export completed -> %s │", filename);
    move_cursor(21, 37);
    printf(     "╰─────────────────────────────────────────────╯");
    temp_message("", 0, 3);
}
