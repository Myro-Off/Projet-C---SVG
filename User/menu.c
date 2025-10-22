#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

#include "create.h"
#include "edit.h"
#include "delete.h"
#include "export.h"
#include "utils.h"
#include "structures.h"
#include "memory.h"
#include "list.h"
#include "group.h"
#include "menu.h"

void display_introduction(bool first_launch) {
    system("clear");
    if (first_launch) {

        block_enter();
        
        printf(MAGENTA"╔════════════════════════════════════════╗\n"
                      "║                                        "YELLOW"║\n"
                      "╚════════════════════════════════════════╝\n");
        move_cursor(2, 15); 
        printf(WHITE BOLD); slow_print("CREATOR STUDIO", 25000); 
        printf(RESET);

        move_cursor(4, 1);
        printf(  RESET"╔════════════════════════════════════════╗\n"
                      "║                                        ║\n"
                      "║                                        ║\n"
                      "╠════════════════════════════════════════╣\n"
                      "║                                        ║\n"
                      "╚════════════════════════════════════════╝\n");
        move_cursor(5, 10);  slow_print("Welcome to Creator Studio", 10000);
        move_cursor(6, 4);   slow_print("your terminal graphic creation tool", 10000);
        move_cursor(8, 5);   slow_print("Coded and Designed  by ", 10000);
        printf(BOLD YELLOW); slow_print("Adam ", 10000);
        printf(PURPLE);      slow_print("Valsan", 10000);

        move_cursor(10, 1);
        printf(  GREEN"╭────────────────────────────────────────╮\n"
                      "│                                        "GREEN"│\n"
                      "╰────────────────────────────────────────╯\n"RESET);
        printf(GREEN);
        move_cursor(11, 4);  slow_print("Press ", 10000);
        printf(RESET BOLD);  slow_print("Enter ↵ ", 10000);
        printf(RESET GREEN); slow_print("to begin the program !", 10000);
        printf(GREEN);

        move_cursor(13, 1);
        empty_buffer(); 
        
        int c;
        do {
            c = getchar();
        } while (c != '\n' && c != EOF);
        
        unblock_enter();
        empty_buffer(); 
    }

    system("clear");
    printf(CYAN"╔════════════════════════════════════════╗\n"
           CYAN"║"RESET"                "WHITE BOLD"MAIN MENU               "RESET CYAN"║\n"
           CYAN"╚════════════════════════════════════════╝\n"RESET
           CYAN"╭────────────────────────────────────────╮\n"
           CYAN"│"RESET" "BOLD SEA"1 ➤ "RESET BOLD"✏️  Create"RESET" a Shape"CYAN"                  │\n"
           CYAN"│"RESET" "BOLD PINK"2 ➤ "RESET BOLD"🧩 Create"RESET" a Group of Shapes"CYAN"        │\n"
           CYAN"│"RESET" "BOLD MAGENTA"3 ➤ "RESET BOLD"⚙️  Edit"RESET" a Shape or a Group"CYAN"         │\n"
           CYAN"│"RESET" "BOLD RED"4 ➤ "RESET BOLD"❌ Delete"RESET" a Shape or a Group"CYAN"       │\n"
           CYAN"├────────────────────────────────────────┤\n"
           CYAN"│"RESET" "BOLD BLUE"5 ➤ "RESET"📋 Open the "BOLD"List"RESET CYAN"                   │\n"
           CYAN"├────────────────────────────────────────┤\n"
           CYAN"│"RESET" "BOLD CYAN"6 ➤ "RESET BOLD"💾 Export"RESET" to SVG"CYAN"                   │\n"
           CYAN"│"RESET" "BOLD CYAN"7 ➤ "RESET BOLD"⬇️  Import"RESET" from SVG (soon)"CYAN"          │\n"
           CYAN"╰────────────────────────────────────────╯\n"
           RED"╭────────────────────────────────────────╮\n"
           RED"│"RESET" "BOLD GRAY"Enter ↵"RESET BOLD"  Quit"RESET" program"RED"                  │\n"
           RED"╰────────────────────────────────────────╯\n\n"

           CYAN"╭─────────────────────────╮  ╭───╮\n"
               "│ "RESET"Select an option (1->7) "CYAN"│->│   │\n"
           CYAN"╰─────────────────────────╯  ╰───╯");
}

int main(void) {
    ShapeList shapes_list;
    init_shape_list(&shapes_list);

    int choice;
    bool first_launch = true;

    do {
        display_introduction(first_launch);
        first_launch = false;

        move_cursor(20, 32);
        fflush(stdout);
        InputStatus status = get_integer(&choice, "", 1, 1, 7, false); 

        if (status == INPUT_CANCEL) break;

        switch (choice) {
            case 1: create_menu(&shapes_list); break;
            case 2: create_group_menu(&shapes_list); break;
            case 3: edit_shape_or_group_menu(&shapes_list); break;
            case 4: delete_shape_or_group_menu(&shapes_list); break;
            case 5: ask_list(&shapes_list); break;
            case 6: export_svg_all("./Export/export.svg", &shapes_list); break;
            case 7: break;
        }

    } while (true);

    free_all_shapes(&shapes_list);

    move_cursor(19, 30); printf(CYAN"╭─────────╮  "GREEN"╭─────────────────────────────╮");
    move_cursor(20, 30); printf(CYAN"│ "GREEN"Enter ↵ "CYAN"│"GREEN"->│ Program ended, memory freed │");
    move_cursor(21, 30); printf(CYAN"╰─────────╯  "GREEN"╰─────────────────────────────╯\n\n");
    return 0;
}

void create_menu(ShapeList* list) {
    int choice;
    InputStatus status;

    while (true) {
        system("clear");
        printf(SEA"╔════════════════════════════════════════╗\n"
                  "║"RESET"             "WHITE"SHAPE CREATION             "SEA"║\n"
                  "╚════════════════════════════════════════╝\n"RESET);
        printf(SEA"╭────────────────────────────────────────╮\n"
               SEA"│"RESET" "SEA"1 ➤ "RESET"●  Circle                          "SEA"│\n"
               SEA"│"RESET" "SEA"2 ➤ "RESET"■  Square                          "SEA"│\n"
               SEA"│"RESET" "SEA"3 ➤ "RESET"▬  Rectangle                       "SEA"│\n"
               SEA"│"RESET" "SEA"4 ➤ "RESET"/  Line                            "SEA"│\n"
               SEA"│"RESET" "SEA"5 ➤ "RESET"⬭  Ellipse                         "SEA"│\n"
               SEA"│"RESET" "SEA"6 ➤ "RESET"⬟  Polygon                         "SEA"│\n"
               SEA"│"RESET" "SEA"7 ➤ "RESET"╱╲ Polyline                        "SEA"│\n"
               SEA"│"RESET" "SEA"8 ➤ "RESET"┼  Path                            "SEA"│\n"
               SEA"╰────────────────────────────────────────╯\n"
               CYAN"╭────────────────────────────────────────╮\n"
               CYAN"│"RESET" "BOLD GRAY"Enter ↵"RESET"  Return to main menu"CYAN"           │\n"
               CYAN"╰────────────────────────────────────────╯\n\n");
        printf(SEA"╭─────────────────────────────────╮  ╭───╮\n"
                  "│ "RESET"Select a shape to create (1->8) "SEA"│->│   │\n"
               SEA"╰─────────────────────────────────╯  ╰───╯");

        move_cursor(19, 40);
        fflush(stdout);
        status = get_integer(&choice, "", 1, 1, 8, false); 

        if (status == INPUT_CANCEL) return; 

        bool choice_is_valid_action = true;
        switch (choice) {
            case 1: ask_circle(list); break;
            case 2: ask_square(list); break;
            case 3: ask_rectangle(list); break;
            case 4: ask_line(list); break;
            case 5: ask_ellipse(list); break;
            case 6: ask_polygon(list); break;
            case 7: ask_multiline(list); break;
            case 8: ask_path(list); break;
        }

        if (choice_is_valid_action) return;
    }
}