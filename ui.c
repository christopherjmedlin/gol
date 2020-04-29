#include "ui.h"

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
                mvaddch(LINES-2-y, x*2, ' ');
                addch(' ');
            }
        }
    }
    attroff(COLOR_PAIR(RED));
}

void interpret_input_insert(int in, CellBoard* gol, UIState* state) {
    switch (in) {
        // escape
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

void interpret_input(int in, CellBoard* gol, UIState* state) {
    if (state->insert) {
        interpret_input_insert(in, gol, state);
        return;
    }
    if (state->playing) {
        // the only operation permitted while playing is to pause
        if (in == 27 || in == 'p') {
            // turn off half delay
            cbreak();
            state->playing = false;
        }
        return;
    }

    switch (in) {
        case 'i': 
            state->insert = true; 
            break;
        case 'p': 
            // causes getchar to stop blocking if user hasn't inputed anything
            // in .5 seconds. so this will cause the automaton to continuously
            // run while still allowing for input
            halfdelay(1);
            state->playing = true;
            break;
        case '\n':
            next_generation(gol);
            break;
    }
}

void run() {
    int input;
    // minus 2 is for the text at the bottom
    // division by 2 is because each cell takes 2 columns
    CellBoard* gol = init_board(COLS/2, LINES-2);
    
    UIState* state = malloc(sizeof(UIState));
    state->insert = false;
    state->playing = false;
    // place cursor near center of terminal
    state->cursorx = COLS/4;
    state->cursory = LINES/2;

    while (true) {
        refresh();
        draw_cells(gol);
        input = getch();
        clear();
        if (input == 'q') {
            break;
        }
        interpret_input(input, gol, state);

        // draw cursor
        if (state->insert) {
            attron(COLOR_PAIR(WHITE));
            mvaddch(LINES-2-state->cursory, state->cursorx*2, ' ');
            addch(' ');
            attroff(COLOR_PAIR(WHITE));
            mvprintw(LINES-1, 0, "(insert mode)");
            mvprintw(LINES-1, COLS-20, "x: %d, y: %d",
                     state->cursorx, state->cursory);
        }

        if (state->playing) {
            next_generation(gol);
            mvprintw(LINES-1, 0, "playing...");
        }
    }

    free_board(gol);
    free(state);
}
