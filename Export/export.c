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
                int x2 = current->data.square->x + current->data.square->side;
                int y2 = current->data.square->y + current->data.square->side;
                if (current->data.square->x < *xmin) *xmin = current->data.square->x;
                if (current->data.square->y < *ymin) *ymin = current->data.square->y;
                if (x2 > *xmax) *xmax = x2;
                if (y2 > *ymax) *ymax = y2;
                break;
            }

            case RECTANGLE: {
                int x2 = current->data.rectangle->x + current->data.rectangle->width;
                int y2 = current->data.rectangle->y + current->data.rectangle->height;
                if (current->data.rectangle->x < *xmin) *xmin = current->data.rectangle->x;
                if (current->data.rectangle->y < *ymin) *ymin = current->data.rectangle->y;
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

                    // Coordonnées principales
                    if (p.type != SEGMENT_CLOSE) {
                        if (p.x < *xmin) *xmin = p.x;
                        if (p.y < *ymin) *ymin = p.y;
                        if (p.x > *xmax) *xmax = p.x;
                        if (p.y > *ymax) *ymax = p.y;
                    }

                    // Contrôles pour les Béziers
                    if (p.type == SEGMENT_QUADRATIC_BEZIER || p.type == SEGMENT_SMOOTH_QUADRATIC) {
                        if (p.control1.x < *xmin) *xmin = p.control1.x;
                        if (p.control1.y < *ymin) *ymin = p.control1.y;
                        if (p.control1.x > *xmax) *xmax = p.control1.x;
                        if (p.control1.y > *ymax) *ymax = p.control1.y;
                    }

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

                    // Pour les arcs : rayon + rotation
                    if (p.type == SEGMENT_ARC) {
                        int rx = abs(p.radiusX);
                        int ry = abs(p.radiusY);
                        int x1 = p.x - rx;
                        int y1 = p.y - ry;
                        int x2 = p.x + rx;
                        int y2 = p.y + ry;
                        if (x1 < *xmin) *xmin = x1;
                        if (y1 < *ymin) *ymin = y1;
                        if (x2 > *xmax) *xmax = x2;
                        if (y2 > *ymax) *ymax = y2;
                    }
                }
                break;
            }

            default: break;
        }

        current = current->next;
    }

    // Marges pour éviter le clipping
    *xmin -= 10;
    *ymin -= 10;
    *xmax += 10;
    *ymax += 10;

    // Taille minimale du canvas
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
    if (!p || p->nb_points < 1) return;

    fprintf(f, "    <path d='");

    int current_x = 0, current_y = 0;
    int has_started = 0;

    for (int i = 0; i < p->nb_points; i++) {
        PathPoint cp = p->points[i];
        char command;

        switch (cp.type) {
            case SEGMENT_MOVE:
                command = (cp.coord_type == COORD_RELATIVE) ? 'm' : 'M';
                fprintf(f, "%c%d %d", command, cp.x, cp.y);
                current_x = (cp.coord_type == COORD_RELATIVE) ? current_x + cp.x : cp.x;
                current_y = (cp.coord_type == COORD_RELATIVE) ? current_y + cp.y : cp.y;
                has_started = 1;
                break;

            case SEGMENT_LINE:
                if (!has_started) {
                    fprintf(f, "M%d %d", cp.x, cp.y);
                    current_x = cp.x;
                    current_y = cp.y;
                    has_started = 1;
                } else {
                    command = (cp.coord_type == COORD_RELATIVE) ? 'l' : 'L';
                    fprintf(f, "%c%d %d", command, cp.x, cp.y);
                    current_x = (cp.coord_type == COORD_RELATIVE) ? current_x + cp.x : cp.x;
                    current_y = (cp.coord_type == COORD_RELATIVE) ? current_y + cp.y : cp.y;
                }
                break;

            case SEGMENT_HORIZONTAL:
                command = (cp.coord_type == COORD_RELATIVE) ? 'h' : 'H';
                fprintf(f, "%c%d", command, cp.x);
                current_x = (cp.coord_type == COORD_RELATIVE) ? current_x + cp.x : cp.x;
                break;

            case SEGMENT_VERTICAL:
                command = (cp.coord_type == COORD_RELATIVE) ? 'v' : 'V';
                fprintf(f, "%c%d", command, cp.y);
                current_y = (cp.coord_type == COORD_RELATIVE) ? current_y + cp.y : cp.y;
                break;

            case SEGMENT_QUADRATIC_BEZIER:
                command = (cp.coord_type == COORD_RELATIVE) ? 'q' : 'Q';
                fprintf(f, "%c%d %d, %d %d", command,
                        cp.control1.x, cp.control1.y,
                        cp.x, cp.y);
                current_x = (cp.coord_type == COORD_RELATIVE) ? current_x + cp.x : cp.x;
                current_y = (cp.coord_type == COORD_RELATIVE) ? current_y + cp.y : cp.y;
                break;

            case SEGMENT_CUBIC_BEZIER:
                command = (cp.coord_type == COORD_RELATIVE) ? 'c' : 'C';
                fprintf(f, "%c%d %d, %d %d, %d %d", command,
                        cp.control1.x, cp.control1.y,
                        cp.control2.x, cp.control2.y,
                        cp.x, cp.y);
                current_x = (cp.coord_type == COORD_RELATIVE) ? current_x + cp.x : cp.x;
                current_y = (cp.coord_type == COORD_RELATIVE) ? current_y + cp.y : cp.y;
                break;

            case SEGMENT_SMOOTH_QUADRATIC:
                command = (cp.coord_type == COORD_RELATIVE) ? 't' : 'T';
                fprintf(f, "%c%d %d", command, cp.x, cp.y);
                current_x = (cp.coord_type == COORD_RELATIVE) ? current_x + cp.x : cp.x;
                current_y = (cp.coord_type == COORD_RELATIVE) ? current_y + cp.y : cp.y;
                break;

            case SEGMENT_SMOOTH_CUBIC:
                command = (cp.coord_type == COORD_RELATIVE) ? 's' : 'S';
                fprintf(f, "%c%d %d, %d %d", command,
                        cp.control2.x, cp.control2.y,
                        cp.x, cp.y);
                current_x = (cp.coord_type == COORD_RELATIVE) ? current_x + cp.x : cp.x;
                current_y = (cp.coord_type == COORD_RELATIVE) ? current_y + cp.y : cp.y;
                break;

            case SEGMENT_ARC:
                command = (cp.coord_type == COORD_RELATIVE) ? 'a' : 'A';
                fprintf(f, "%c%d %d %d %d %d %d %d", command,
                        cp.radiusX, cp.radiusY,
                        cp.xAxisRotation,
                        cp.largeArcFlag ? 1 : 0,
                        cp.sweepFlag ? 1 : 0,
                        cp.x, cp.y);
                current_x = (cp.coord_type == COORD_RELATIVE) ? current_x + cp.x : cp.x;
                current_y = (cp.coord_type == COORD_RELATIVE) ? current_y + cp.y : cp.y;
                break;

            case SEGMENT_CLOSE:
                command = (cp.coord_type == COORD_RELATIVE) ? 'z' : 'Z';
                fprintf(f, "%c", command);
                break;
        }
    }

    fprintf(f, "' fill='%s' stroke='%s' stroke-width='%d' transform='scale(%d, %d)'/>\n",
            p->fill_color, p->stroke_color, p->thickness, p->scalex, p->scaley);
}

void export_svg_all(const char* filename, ShapeList* list) {
    if (!list || list->size == 0) {
        move_cursor(19, 37); printf(ORANGE"╭────────────────────────────────────────╮");
        move_cursor(20, 35); printf(    "->│ Please create a shape before exporting │");
        move_cursor(21, 37); printf(      "╰────────────────────────────────────────╯");
        temp_message("", 0, 2);
            return;
        }
    system("rm Export/export.svg");

    move_cursor(19, 37); printf(ORANGE"╭─────────────────────────────────────────╮");
    move_cursor(20, 35); printf(    "->│                                         │");
    move_cursor(21, 37); printf(      "╰─────────────────────────────────────────╯");

    FILE* f = fopen(filename, "w");
    if (!f) {
        move_cursor(19, 37); printf(ORANGE"╭─────────────────────────╮");
        move_cursor(20, 35); printf(    "->│ Error creating SVG file │");
        move_cursor(21, 37); printf(      "╰─────────────────────────╯");
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
    move_cursor(19, 37); printf(GREEN"╭─────────────────────────────────────────────╮");
    move_cursor(20, 35); printf(   "->│ SVG export completed -> %s │", filename);
    move_cursor(21, 37); printf(     "╰─────────────────────────────────────────────╯");
    temp_message("", 0, 3);
}