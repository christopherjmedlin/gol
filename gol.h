#ifndef GAME_OF_LIFE
#define GAME_OF_LIFE

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int size_x;
    int size_y;
    bool* cells;
    bool* next_cells;
    long generations;
} CellBoard;

CellBoard* init_board(int size_x, int size_y);

// Gets/sets a value in the cell array with wrap-around
bool get_cell(CellBoard* board, int x, int y);
void set_cell(CellBoard* board, int x, int y, bool val);
void toggle_cell(CellBoard* board, int x, int y);

// Processes the next stage
void next_generation(CellBoard* board);

// file i/o
void save_state(CellBoard* board, char* filename);
CellBoard* load_state(char* filename, int size_x, int size_y);

// frees from memory
void free_board(CellBoard* board);

#endif
