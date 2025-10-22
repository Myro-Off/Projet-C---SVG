#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "create.h"
#include "list.h"
#include "group.h"
#include "utils.h"
#include "structures.h"
#include "shapes.h"
#include "menu.h"

// ----------------------- Boîtes de messages -----------------------
void box_message(const char* color, const char* title, const char* msg) {
    printf(    "%s╔════════════════════════════════════════════════╗\n", color);
    printf(      "║ %-46s ║\n", title);
    printf(      "║ %-46s ║\n", msg);
    temp_message("╚════════════════════════════════════════════════╝\n"RESET, 0, 2);
}

void box_success(const char* msg) { box_message(GREEN, "                   SUCCESS                   ", msg); }
void box_error(const char* msg)   { box_message(ORANGE, "                    ERROR                    ", msg); }

// ----------------------- Messages création réussie -----------------------
void circle_created(void)    { system("clear"); box_success("         Circle created successfully!        "); }
void square_created(void)    { system("clear"); box_success("        Square created successfully!         "); }
void rectangle_created(void) { system("clear"); box_success("       Rectangle created successfully!       "); }
void line_created(void)      { system("clear"); box_success("         Line created successfully!          "); }
void ellipse_created(void)   { system("clear"); box_success("        Ellipse created successfully!        "); }
void polygon_created(void)   { system("clear"); box_success("        Polygon created successfully!        "); }
void multiline_created(void) { system("clear"); box_success("       Polyline created successfully!        "); }
void path_created(void)      { system("clear"); box_success("         Path created successfully!          "); }

// ----------------------- Messages erreur points -----------------------
void polygon_points_error(void)   { box_error("     A polygon must have 3 to 15 points      "); }
void multiline_points_error(void) { box_error("     A polyline must have 3 to 15 points     "); }
void path_points_error(void)      { box_error("  A path must have between 1 and 15 segments  "); }

// ----------------------- Boîtes de création -----------------------
void box_creation(const char* type) {
    system("clear");
    printf(SEA"╔══════════════════════════════════════════════╗\n");
    printf(   "║ %-44s ║\n", type);
    printf(   "║ Press "RESET BOLD"Enter ↵"RESET SEA" at any time to cancel creation ║\n");
    printf(   "╚══════════════════════════════════════════════╝\n"RESET);
}

void circle_creation(void)    { box_creation("               CREATE CIRCLE               "); }
void square_creation(void)    { box_creation("               CREATE SQUARE               "); }
void rectangle_creation(void) { box_creation("             CREATE  RECTANGLE             "); }
void ellipse_creation(void)   { box_creation("              CREATE  ELLIPSE              "); }
void line_creation(void)      { box_creation("                CREATE LINE                "); }
void polygon_creation(void)   { box_creation("              CREATE  POLYGON              "); }
void multiline_creation(void) { box_creation("              CREATE POLYLINE              "); }
void path_creation(void)      { box_creation("                CREATE PATH                "); }

// ----------------------- Fonctions utilitaires -----------------------
static bool ask_integer(int* value, const char* prompt, int width, int min, int max, bool allow_negative) {
    InputStatus status;
    do {
        status = get_integer(value, prompt, width, min, max, allow_negative);
        if (status == INPUT_SUCCESS) return true;
        if (creation_error(status)) return false;  // <-- arrêt complet si annulé
    } while(true);
}

static bool ask_color(char* buffer, const char* prompt) {
    InputStatus status;
    do {
        status = get_color(buffer, prompt);
        if (status == INPUT_SUCCESS) return true;
        if (creation_error(status)) return false;
    } while(true);
}

static bool ask_scale(int* scalex, int* scaley) {
    printf(WHITE"╭────────────────────────────────────────╮\n"
                "│ "RESET"negative = inverted                    "WHITE"│\n"
                "│ "RESET"number = scale multiplication          "WHITE"│\n"
                "╰────────────────────────────────────────╯\n"RESET);
    if (!ask_integer(scalex, "• Scale X: ", 2, 0, 9, true)) return false;
    if (!ask_integer(scaley, "• Scale Y: ", 2, 0, 9, true)) return false;
    return true;
}

// ----------------------- Fonctions ask_* -----------------------
void ask_circle(ShapeList* list) {
    system("clear");
    circle_creation();
    int x, y, radius, thickness, scalex, scaley;
    char stroke_color[16], fill_color[16];

    if (!ask_integer(&x, "• Center X: ", 3, 0, 9, true)) return;
    if (!ask_integer(&y, "• Center Y: ", 3, 0, 9, true)) return;
    if (!ask_integer(&radius, "• Radius: ", 3, 0, 9, false)) return;
    if (!ask_integer(&thickness, "• Thickness: ", 2, 0, 9, false)) return;
    if (!ask_color(stroke_color, "• Stroke "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_color(fill_color, "• Fill "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_scale(&scalex, &scaley)) return;

    Circle* ci = create_circle(x, y, radius, stroke_color, fill_color, thickness, scalex, scaley);
    ShapeData data = {.circle = ci};
    add_shape(list, CIRCLE, data);

    system("clear");
    circle_created();
}

void ask_square(ShapeList* list) {
    system("clear");
    square_creation();
    int x, y, side, thickness, rounded, scalex, scaley;
    char stroke_color[16], fill_color[16];

    if (!ask_integer(&x, "• Top-left X: ", 3, 0, 9, true)) return;
    if (!ask_integer(&y, "• Top-left Y: ", 3, 0, 9, true)) return;
    if (!ask_integer(&side, "• Side length: ", 3, 0, 9, false)) return;
    if (!ask_color(stroke_color, "• Stroke "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_color(fill_color, "• Fill "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_integer(&thickness, "• Thickness: ", 2, 0, 9, false)) return;

    printf(WHITE"╭────────────────────────────────────────╮\n"
                "│ "RESET"0 = not rounded                        "WHITE"│\n"
                "│ "RESET">0 = rounding power                    "WHITE"│\n"
                "╰────────────────────────────────────────╯\n"RESET);
    if (!ask_integer(&rounded, "• Corner rounding: ", 2, 0, 9, false)) return;

    if (!ask_scale(&scalex, &scaley)) return;

    Square* sq = create_square(x, y, side, stroke_color, fill_color, thickness, rounded, scalex, scaley);
    ShapeData data = {.square = sq};
    add_shape(list, SQUARE, data);

    system("clear");
    square_created();
}

void ask_rectangle(ShapeList* list) {
    system("clear");
    rectangle_creation();
    int x, y, width, height, thickness, rounded, scalex, scaley;
    char stroke_color[16], fill_color[16];

    if (!ask_integer(&x, "• Top-left X: ", 3, 0, 9, true)) return;
    if (!ask_integer(&y, "• Top-left Y: ", 3, 0, 9, true)) return;
    if (!ask_integer(&width, "• Width: ", 3, 0, 9, false)) return;
    if (!ask_integer(&height, "• Height: ", 3, 0, 9, false)) return;
    if (!ask_color(stroke_color, "• Stroke "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_color(fill_color, "• Fill "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_integer(&thickness, "• Thickness: ", 2, 0, 9, false)) return;

    printf(WHITE"╭────────────────────────────────────────╮\n"
                "│ "RESET"0 = not rounded                        "WHITE"│\n"
                "│ "RESET">0 = rounding power                    "WHITE"│\n"
                "╰────────────────────────────────────────╯\n"RESET);
    if (!ask_integer(&rounded, "• Corner rounding: ", 2, 0, 9, false)) return;

    if (!ask_scale(&scalex, &scaley)) return;

    Rectangle* rect = create_rectangle(x, y, width, height, stroke_color, fill_color, thickness, rounded, scalex, scaley);
    ShapeData data = {.rectangle = rect};
    add_shape(list, RECTANGLE, data);

    system("clear");
    rectangle_created();
}

void ask_line(ShapeList* list) {
    system("clear");
    line_creation();
    int x1, y1, x2, y2, thickness, scalex, scaley;
    char stroke_color[16];

    if (!ask_integer(&x1, "• Start X: ", 3, 0, 9, true)) return;
    if (!ask_integer(&y1, "• Start Y: ", 3, 0, 9, true)) return;
    if (!ask_integer(&x2, "• End X: ", 3, 0, 9, true)) return;
    if (!ask_integer(&y2, "• End Y: ", 3, 0, 9, true)) return;
    if (!ask_integer(&thickness, "• Thickness: ", 2, 0, 9, false)) return;
    if (!ask_color(stroke_color, "• Stroke "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_scale(&scalex, &scaley)) return;

    Line* line = create_line(x1, y1, x2, y2, stroke_color, thickness, scalex, scaley);
    ShapeData data = {.line = line};
    add_shape(list, LINE, data);

    system("clear");
    line_created();
}

void ask_ellipse(ShapeList* list) {
    system("clear");
    ellipse_creation();
    int x, y, rx, ry, thickness, scalex, scaley;
    char stroke_color[16], fill_color[16];

    if (!ask_integer(&x, "• Center X: ", 3, 0, 9, true)) return;
    if (!ask_integer(&y, "• Center Y: ", 3, 0, 9, true)) return;
    if (!ask_integer(&rx, "• Radius X: ", 3, 0, 9, false)) return;
    if (!ask_integer(&ry, "• Radius Y: ", 3, 0, 9, false)) return;
    if (!ask_integer(&thickness, "• Thickness: ", 2, 0, 9, false)) return;
    if (!ask_color(stroke_color, "• Stroke "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_color(fill_color, "• Fill "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) return;
    if (!ask_scale(&scalex, &scaley)) return;

    Ellipse* e = create_ellipse(x, y, rx, ry, stroke_color, fill_color, thickness, scalex, scaley);
    ShapeData data = {.ellipse = e};
    add_shape(list, ELLIPSE, data);

    system("clear");
    ellipse_created();
}

void ask_polygon(ShapeList* list) {
    system("clear");
    polygon_creation();

    int points_count, thickness, scalex, scaley;
    char stroke_color[16], fill_color[16];

    if (!ask_integer(&points_count, "• Number of points (3→15): ", 2, 0, 9, false)) return;

    if (points_count < 3 || points_count > 15) {
        polygon_points_error();
        return;
    }
    
    Point* points = malloc(sizeof(Point) * points_count);
    if (!points) return;

    for (int i = 0; i < points_count; i++) {
        char prompt_x[32], prompt_y[32];
        sprintf(prompt_x, "• Point %d X: ", i + 1);
        sprintf(prompt_y, "• Point %d Y: ", i + 1);
        if (!ask_integer(&points[i].x, prompt_x, 3, 0, 9, true)) { free(points); return; }
        if (!ask_integer(&points[i].y, prompt_y, 3, 0, 9, true)) { free(points); return; }
    }

    if (!ask_color(stroke_color, "• Stroke "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) { free(points); return; }
    if (!ask_color(fill_color, "• Fill "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) { free(points); return; }
    if (!ask_integer(&thickness, "• Thickness: ", 2, 0, 9, false)) { free(points); return; }
    if (!ask_scale(&scalex, &scaley)) { free(points); return; }

    Polygon* po = create_polygon(points, points_count, stroke_color, fill_color, thickness, scalex, scaley);
    ShapeData data = {.polygon = po};
    add_shape(list, POLYGON, data);

    free(points);
    system("clear");
    polygon_created();
}

void ask_multiline(ShapeList* list) {
    system("clear");
    multiline_creation();

    int points_count, thickness, scalex, scaley;
    char stroke_color[16];

    if (!ask_integer(&points_count, "• Number of points (3→15): ", 2, 0, 9, false)) return;

    if (points_count < 3 || points_count > 15) {
        multiline_points_error();
        return;
    }

    Point* points = malloc(sizeof(Point) * points_count);
    if (!points) return;

    for (int i = 0; i < points_count; i++) {
        char prompt_x[32], prompt_y[32];
        sprintf(prompt_x, "• Point %d X: ", i + 1);
        sprintf(prompt_y, "• Point %d Y: ", i + 1);
        if (!ask_integer(&points[i].x, prompt_x, 3, 0, 9, true)) { free(points); return; }
        if (!ask_integer(&points[i].y, prompt_y, 3, 0, 9, true)) { free(points); return; }
    }

    if (!ask_color(stroke_color, "• Stroke "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) { free(points); return; }
    if (!ask_integer(&thickness, "• Thickness: ", 2, 0, 9, false)) { free(points); return; }
    if (!ask_scale(&scalex, &scaley)) { free(points); return; }

    Multiline* ml = create_multiline(points, points_count, stroke_color, thickness, scalex, scaley);
    ShapeData data = {.multiline = ml};
    add_shape(list, MULTILINE, data);

    free(points);
    system("clear");
    multiline_created();
}

void ask_path(ShapeList* list) {
    system("clear");
    path_creation();

    int nb_segments, thickness, scalex, scaley;
    char stroke_color[16], fill_color[16];
    bool closed;

    if (!ask_integer(&nb_segments, "• Number of segments (1→9): ", 1, 1, 9, false)) return;
    int nb_points = nb_segments + 1;

    PathPoint* points = calloc(nb_points, sizeof(PathPoint));
    if (!points) return;

    printf(BLUE"\n---- Point 1 (Start of Path - M/m) ----\n"RESET);
    printf(WHITE
    "╭────────────────────────────────────────╮\n"
    "│ "RESET"Coordinate Type:                       "WHITE"│\n"
    "│                                        "WHITE"│\n"
    "│ "RESET"1 = Absolute                           "WHITE"│\n"
    "│ "RESET"2 = Relative                           "WHITE"│\n"
    "╰────────────────────────────────────────╯\n"RESET);
    if (!ask_integer((int*)&points[0].coord_type, "• Coordinate Type: ", 1, 1, 2, false)) { free(points); return; }
    points[0].type = SEGMENT_MOVE;
    if (!ask_integer(&points[0].x, "• X: ", 3, 0, 9, true)) { free(points); return; }
    if (!ask_integer(&points[0].y, "• Y: ", 3, 0, 9, true)) { free(points); return; }

    for (int i = 1; i < nb_points; i++) {
        printf(BLUE"\n------------- Segment %d -------------\n"RESET, i);
        printf(WHITE
        "╭────────────────────────────────────────╮\n"
        "│ "RESET"Coordinate Type:                       "WHITE"│\n"
        "│                                        "WHITE"│\n"
        "│ "RESET"1 = Absolute                           "WHITE"│\n"
        "│ "RESET"2 = Relative                           "WHITE"│\n"
        "╰────────────────────────────────────────╯\n"RESET);
        if (!ask_integer((int*)&points[i].coord_type, "• Coordinate Type: ", 1, 1, 2, false)) { free(points); return; }

        printf(WHITE
        "╭────────────────────────────────────────╮\n"
        "│ "RESET"Segment Type:                          "WHITE"│\n"
        "│                                        "WHITE"│\n"
        "│ "RESET"1 = Move (M / m)                       "WHITE"│\n"
        "│ "RESET"2 = Line (L / l)                       "WHITE"│\n"
        "│ "RESET"3 = Horizontal Line (H / h)            "WHITE"│\n"
        "│ "RESET"4 = Vertical Line (V / v)              "WHITE"│\n"
        "│ "RESET"5 = Quadratic Bézier (Q / q)           "WHITE"│\n"
        "│ "RESET"6 = Cubic Bézier (C / c)               "WHITE"│\n"
        "│ "RESET"7 = Smooth Quadratic (T / t)           "WHITE"│\n"
        "│ "RESET"8 = Smooth Cubic (S / s)               "WHITE"│\n"
        "│ "RESET"9 = Arc (A / a)                        "WHITE"│\n"
        "╰────────────────────────────────────────╯\n"RESET);
        if (!ask_integer((int*)&points[i].type, "• Segment Type: ", 1, 1, 9, false)) { free(points); return; }

        switch(points[i].type) {
            case SEGMENT_MOVE:
                if (!ask_integer(&points[i].x, "• X (move to): ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].y, "• Y (move to): ", 3, 0, 9, true)) { free(points); return; }
                break;

            case SEGMENT_LINE:
            case SEGMENT_SMOOTH_QUADRATIC:
                if (!ask_integer(&points[i].x, "• X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].y, "• Y: ", 3, 0, 9, true)) { free(points); return; }
                break;
            case SEGMENT_HORIZONTAL:
                if (!ask_integer(&points[i].x, "• X: ", 3, 0, 9, true)) { free(points); return; }
                break;
            case SEGMENT_VERTICAL:
                if (!ask_integer(&points[i].y, "• Y: ", 3, 0, 9, true)) { free(points); return; }
                break;
            case SEGMENT_QUADRATIC_BEZIER:
                if (!ask_integer(&points[i].control1.x, "• Control X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].control1.y, "• Control Y: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].x, "• End X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].y, "• End Y: ", 3, 0, 9, true)) { free(points); return; }
                break;
            case SEGMENT_CUBIC_BEZIER:
                if (!ask_integer(&points[i].control1.x, "• Control 1 X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].control1.y, "• Control 1 Y: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].control2.x, "• Control 2 X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].control2.y, "• Control 2 Y: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].x, "• End X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].y, "• End Y: ", 3, 0, 9, true)) { free(points); return; }
                break;
            case SEGMENT_SMOOTH_CUBIC:
                printf(WHITE
                "╭──────────────────────────────────────────────╮\n"
                "│ "RESET"Reminder:                                    "WHITE"│\n"
                "│ "RESET"First control point is mirrored automatically"WHITE"│\n"
                "│ "RESET"from previous segment’s second control point "WHITE"│\n"
                "╰──────────────────────────────────────────────╯\n"RESET);

                if (!ask_integer(&points[i].control2.x, "• Control point 2 - X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].control2.y, "• Control point 2 - Y: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].x, "• End point - X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].y, "• End point - Y: ", 3, 0, 9, true)) { free(points); return; }

                break;
            case SEGMENT_ARC:
                if (!ask_integer(&points[i].radiusX, "• Radius X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].radiusY, "• Radius Y: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].xAxisRotation, "• X-axis rotation (degrees): ", 3, 0, 9, true)) { free(points); return; }

                printf(WHITE
                "╭────────────────────────────────────────╮\n"
                "│ "RESET"Large Arc Flag:                        "WHITE"│\n"
                "│ "RESET"0 = Draw smaller arc                   "WHITE"│\n"
                "│ "RESET"1 = Draw larger arc                    "WHITE"│\n"
                "╰────────────────────────────────────────╯\n"RESET);
                int tmp;
                if (!ask_integer(&tmp, "• Large arc flag (0/1): ", 1, 0, 1, false)) { free(points); return; }
                points[i].largeArcFlag = (tmp != 0);

                printf(WHITE
                "╭────────────────────────────────────────╮\n"
                "│ "RESET"Sweep Flag:                            "WHITE"│\n"
                "│ "RESET"0 = Draw counter-clockwise (⟲)         "WHITE"│\n"
                "│ "RESET"1 = Draw clockwise (⟳)                 "WHITE"│\n"
                "╰────────────────────────────────────────╯\n"RESET);
                if (!ask_integer(&tmp, "• Sweep flag (0/1): ", 1, 0, 1, false)) { free(points); return; }
                points[i].sweepFlag = (tmp != 0);       

                if (!ask_integer(&points[i].x, "• End X: ", 3, 0, 9, true)) { free(points); return; }
                if (!ask_integer(&points[i].y, "• End Y: ", 3, 0, 9, true)) { free(points); return; }
                break;
            default:
                break;
        }
    }

    if (!ask_color(stroke_color, "• Stroke "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) { free(points); return; }
    if (!ask_color(fill_color, "• Fill "BOLD RED"c"GREEN"o"BLUE"l"PURPLE"o"YELLOW"r"RESET": ")) { free(points); return; }
    if (!ask_integer(&thickness, "• Thickness: ", 2, 0, 9, false)) { free(points); return; }
    if (!ask_scale(&scalex, &scaley)) { free(points); return; }

    printf(WHITE
    "╭────────────────────────────────────────╮\n"
    "│ "RESET"0 = Open path                          "WHITE"│\n"
    "│ "RESET"1 = Closed path                        "WHITE"│\n"
    "╰────────────────────────────────────────╯\n"RESET);
    int closed_int;
    if (!ask_integer(&closed_int, "• Closed: ", 1, 0, 1, false)) { free(points); return; }
    closed = (closed_int != 0);

    Path* path = create_path(points, nb_points, stroke_color, fill_color, thickness, scalex, scaley, closed);
    ShapeData data = {.path = path};
    add_shape(list, PATH, data);

    free(points);
    system("clear");
    path_created();
}