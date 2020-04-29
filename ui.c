#include "ui.h"
#include <unistd.h>

void init_ncurses() {
    initscr();
    noecho();
    // don't know if i'll ever use F keys, but i'll enable this anyway.
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();

    init_pair(RED, COLOR_RED, COLOR_RED);
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
}

void draw_cells(CellBoard* cells) {
    int x, y;
    attron(COLOR_PAIR(RED));
    for (x = 0; x < cells->size_x; x++) {
        for (y = 0; y < cells->size_y; y++) {
            if (get_cell(cells, x, y)) {
                // space will appear as red block
                mvaddch(LINES-3-y, x*2, ' ');
                addch(' ');
            }
        }
    }
    attroff(COLOR_PAIR(RED));
}

void interpret_input_insert(int in, CellBoard* gol, UIState* state) {
    switch (in) {
        // exit insert
        case 'i':
            state->insert = false;
            break;
        case KEY_LEFT:
            if (state->cursorx > 0)
                state->cursorx -= 1;
            break;
        case KEY_DOWN:
            if (state->cursory > 0)
                state->cursory--;
            break;
        case KEY_UP:
            if (state->cursory < LINES)
                state->cursory += 1;;
            break;
        case KEY_RIGHT:
            if (state->cursorx < COLS/2)
                state->cursorx++;
            break;
        case '\n':
            toggle_cell(gol, state->cursorx, state->cursory);
            break;
    }
}

void interpret_input_playing(int in, CellBoard* gol, UIState* state) {
    switch (in) {
        case 'p':
            state->playing = false;
            // make getch block again
            timeout(-1);
            break;
        case '+':
            if (state->play_speed < 7)
                state->play_speed++;
            break;
        case '-':
            if (state->play_speed > 1)
                state->play_speed--;
            break;
    }
}

void print_help() {
    erase();
    mvprintw(LINES/2-3, COLS/2-10, "Press p to play/pause.");
    mvprintw(LINES/2-1, COLS/2-15, "Press i to toggle insert mode.");
    mvprintw(LINES/2, COLS/2-17, "Press enter to advance a generation.");
    mvprintw(LINES/2+1, COLS/2-16, "Press w to save the current state.");
    mvprintw(LINES/2+2, COLS/2-17, "Press any key to close this message.");
    getch();
}

void interpret_input(int in, CellBoard* gol, UIState* state) {
    char filename[50];

    if (state->insert) {
        interpret_input_insert(in, gol, state);
        return;
    }
    else if (state->playing) {
        interpret_input_playing(in, gol, state);
        return;
    }

    switch (in) {
        case 'i': 
            state->insert = true; 
            break;
        case 'p': 
            state->playing = true;
            // make getch non-blocking
            timeout(1);
            break;
        case 'w':
            echo();
            mvprintw(LINES-1, 0, 
                     "File name (relative to working directory): ");
            getstr(filename);
            noecho();
            
            save_state(gol, filename);
            // clear the bottom line
            move(LINES-1, 0);
            clrtobot();
            mvprintw(LINES-1, 0, "State saved as %s.", filename);
            break;
        case 'h':
            print_help();
            break;
        case '\n':
            next_generation(gol);
            break;
    }
}

useconds_t sleep_time(int speed) {
    // these values are rather arbitrarily defined for now
    switch (speed) {
        case 1: return 100000;
        case 2: return 50000;
        case 3: return 25000;
        case 4: return 10000;
        case 5: return 5000;
        case 6: return 3000;
        case 7: return 1000;
    }
    return 10000;
}

void run(char* filename) {
    int input;
    CellBoard* gol;
    
    if (filename == NULL) {
        gol = load_state(DEFAULT_FILE, COLS/2, LINES-3);
    } else {
        gol = load_state(filename, COLS/2, LINES-3);
    }
    
    UIState* state = malloc(sizeof(UIState));
    state->insert = false;
    state->playing = false;
    // place cursor near center of terminal
    state->cursorx = COLS/4;
    state->cursory = LINES/2;
    state->play_speed = 2;

    while (true) {
        draw_cells(gol);
        refresh();
        input = getch();
        if (input == 'q') {
            break;
        }
        interpret_input(input, gol, state);

        erase();
        // draw cursor
        if (state->insert) {
            attron(COLOR_PAIR(WHITE));
            mvaddch(LINES-3-state->cursory, state->cursorx*2, ' ');
            addch(' ');
            attroff(COLOR_PAIR(WHITE));
            mvprintw(LINES-1, 0, "(insert mode)");
            mvprintw(LINES-1, COLS-20, "x: %d, y: %d",
                     state->cursorx, state->cursory);
        }

        if (state->playing) {
            next_generation(gol);
            // this function is technically deprecated, but nanosleep is far
            // more complicated
            usleep(sleep_time(state->play_speed));
            mvprintw(LINES-1, 0, "playing...");
            mvprintw(LINES-1, COLS-25, "total generations: %d", gol->generations);
        }
    }

    free_board(gol);
    free(state);
}
