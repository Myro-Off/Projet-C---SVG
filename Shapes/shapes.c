#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "structures.h"

Square* create_square(int x, int y, int side, const char* stroke_color, const char* fill_color, int thickness, int rounded, int scalex, int scaley) {
    Square* sq = malloc(sizeof(Square));
    if (!sq) return NULL;

    sq->x = x;
    sq->y = y;
    sq->side = side;
    snprintf(sq->stroke_color, sizeof(sq->stroke_color), "%s", stroke_color);
    snprintf(sq->fill_color, sizeof(sq->fill_color), "%s", fill_color);
    sq->thickness = thickness;
    sq->rounded = rounded;
    sq->scalex = scalex;
    sq->scaley = scaley;

    return sq;
}

Circle* create_circle(int x, int y, int radius, const char* stroke_color, const char* fill_color, int thickness, int scalex, int scaley) {
    Circle* ci = malloc(sizeof(Circle));
    if (!ci) return NULL;

    ci->x = x;
    ci->y = y;
    ci->radius = radius;
    snprintf(ci->stroke_color, sizeof(ci->stroke_color), "%s", stroke_color);
    snprintf(ci->fill_color, sizeof(ci->fill_color), "%s", fill_color);
    ci->thickness = thickness;
    ci->scalex = scalex;
    ci->scaley = scaley;

    return ci;
}

Ellipse* create_ellipse(int x, int y, int radiusx, int radiusy, const char* stroke_color, const char* fill_color, int thickness, int scalex, int scaley) {
    Ellipse* el = malloc(sizeof(Ellipse));
    if (!el) return NULL;

    el->x = x;
    el->y = y;
    el->radiusx = radiusx;
    el->radiusy = radiusy;
    snprintf(el->stroke_color, sizeof(el->stroke_color), "%s", stroke_color);
    snprintf(el->fill_color, sizeof(el->fill_color), "%s", fill_color);
    el->thickness = thickness;
    el->scalex = scalex;
    el->scaley = scaley;

    return el;
}

Line* create_line(int x1, int y1, int x2, int y2, const char* stroke_color, int thickness, int scalex, int scaley) {
    Line* li = malloc(sizeof(Line));
    if (!li) return NULL;

    li->x1 = x1;
    li->y1 = y1;
    li->x2 = x2;
    li->y2 = y2;
    snprintf(li->stroke_color, sizeof(li->stroke_color), "%s", stroke_color);
    li->thickness = thickness;
    li->scalex = scalex;
    li->scaley = scaley;

    return li;
}

Rectangle* create_rectangle(int x, int y, int width, int height, const char* stroke_color, const char* fill_color, int thickness, int rounded, int scalex, int scaley) {
    Rectangle* re = malloc(sizeof(Rectangle));
    if (!re) return NULL;

    re->x = x;
    re->y = y;
    re->width = width;
    re->height = height;
    snprintf(re->stroke_color, sizeof(re->stroke_color), "%s", stroke_color);
    snprintf(re->fill_color, sizeof(re->fill_color), "%s", fill_color);
    re->thickness = thickness;
    re->rounded = rounded;
    re->scalex = scalex;
    re->scaley = scaley;

    return re;
}

Polygon* create_polygon(Point* points, int nb_points, const char* stroke_color, const char* fill_color, int thickness, int scalex, int scaley) {
    if (!points || nb_points < 3) return NULL;

    Polygon* pg = malloc(sizeof(Polygon));
    if (!pg) return NULL;

    pg->points = malloc(sizeof(Point) * nb_points);
    if (!pg->points) {
        free(pg);
        return NULL;
    }

    for (int i = 0; i < nb_points; i++)
        pg->points[i] = points[i];

    pg->nb_points = nb_points;
    snprintf(pg->stroke_color, sizeof(pg->stroke_color), "%s", stroke_color);
    snprintf(pg->fill_color, sizeof(pg->fill_color), "%s", fill_color);
    pg->thickness = thickness;
    pg->scalex = scalex;
    pg->scaley = scaley;

    return pg;
}

Multiline* create_multiline(Point* points, int nb_points, const char* stroke_color, int thickness, int scalex, int scaley) {
    if (!points || nb_points < 2) return NULL;

    Multiline* ml = malloc(sizeof(Multiline));
    if (!ml) return NULL;

    ml->points = malloc(sizeof(Point) * nb_points);
    if (!ml->points) {
        free(ml);
        return NULL;
    }

    for (int i = 0; i < nb_points; i++)
        ml->points[i] = points[i];

    ml->nb_points = nb_points;
    snprintf(ml->stroke_color, sizeof(ml->stroke_color), "%s", stroke_color);
    ml->thickness = thickness;
    ml->scalex = scalex;
    ml->scaley = scaley;

    return ml;
}

Path* create_path(PathPoint* points, int nb_points, const char* stroke_color, const char* fill_color, int thickness, int closed, int scalex, int scaley) {
    if (!points || nb_points < 2) return NULL;

    Path* pa = malloc(sizeof(Path));
    if (!pa) return NULL;

    pa->points = calloc(nb_points, sizeof(PathPoint));
    if (!pa->points) {
        free(pa);
        return NULL;
    }

    for (int i = 0; i < nb_points; i++)
        pa->points[i] = points[i];

    pa->nb_points = nb_points;
    snprintf(pa->stroke_color, sizeof(pa->stroke_color), "%s", stroke_color);
    snprintf(pa->fill_color, sizeof(pa->fill_color), "%s", fill_color);
    pa->thickness = thickness;
    pa->closed = closed;
    pa->scalex = scalex;
    pa->scaley = scaley;

    return pa;
}