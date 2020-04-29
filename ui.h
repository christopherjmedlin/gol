#ifndef GOL_UI
#define GOL_UI

#include <ncurses.h>
#include "gol.h"

#define RED 1
#define WHITE 2

#define DEFAULT_FILE "rip.gol"

typedef struct {
    bool insert;
    bool playing;
    int cursorx;
    int cursory;
} UIState;

void init_ncurses(); 
void draw_cells(CellBoard* cells);
void run();

#endif
