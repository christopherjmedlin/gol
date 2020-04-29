#include "gol.h"
#include <stdio.h>
#include <string.h>

int wraparound_index(int index, int size) {
    int new_index = index % size;
    if (new_index < 0) {
        return new_index + size;
    }
    return new_index;
}

bool get_cell(CellBoard* board, int x, int y) {
    int newx, newy;
    newx = wraparound_index(x, board->size_x);
    newy = wraparound_index(y, board->size_y);
    return *(board->cells + (newy*board->size_x) + newx);
}

void set_cell(CellBoard* board, int x, int y, bool val) {
    int newx, newy;
    newx = wraparound_index(x, board->size_x);
    newy = wraparound_index(y, board->size_y);
    *(board->cells + (newy*board->size_x) + newx) = val;
    // also set the next generation
    *(board->next_cells + (newy*board->size_x) + newx) = val;
}

void toggle_cell(CellBoard* board, int x, int y) {
    set_cell(board, x, y, !get_cell(board, x, y));
}

void set_next_generation(CellBoard* board, int x, int y, bool val) {
    int newx, newy;
    newx = wraparound_index(x, board->size_x);
    newy = wraparound_index(y, board->size_y);
    *(board->next_cells + (newy*board->size_x) + newx) = val;
}

CellBoard* init_board(int size_x, int size_y) {
    CellBoard* board = malloc(sizeof(CellBoard));
    board->cells = malloc(sizeof(char)*size_x*size_y);
    board->next_cells = malloc(sizeof(char)*size_x*size_y);
    board->size_x = size_x;
    board->size_y = size_y;

    for (int i = 0; i < size_x; i++) {
        for (int j = 0; j < size_y; j++) {
            set_cell(board, i, j, false);
            set_next_generation(board, i, j, false);
        }
    }

    return board;
}

int num_neighbors(CellBoard* board, int x, int y) {
    int neighbors = 0;

    if (get_cell(board, x+1, y)) neighbors++;
    if (get_cell(board, x, y+1)) neighbors++;
    if (get_cell(board, x-1, y)) neighbors++;
    if (get_cell(board, x, y-1)) neighbors++;
    if (get_cell(board, x+1, y+1)) neighbors++;
    if (get_cell(board, x-1, y-1)) neighbors++;
    if (get_cell(board, x+1, y-1)) neighbors++;
    if (get_cell(board, x-1, y+1)) neighbors++;

    return neighbors;
}

void process_cell(CellBoard* board, int x, int y) {
    int neighbors = num_neighbors(board, x, y); 
    // if cell alive
    if (get_cell(board, x, y)) {
        if (neighbors < 2 || neighbors > 3) {
            set_next_generation(board, x, y, false);
        }
    } else {
        if (neighbors == 3) {
            set_next_generation(board, x, y, true);
        }
    }
}

void next_generation(CellBoard* board) {
    int i, j;
    for (i = 0; i < board->size_x; i++) {
        for (j = 0; j < board->size_y; j++) {
            process_cell(board, i, j);
        }
    }
    memcpy(board->cells, board->next_cells,
           sizeof(bool)*board->size_x*board->size_y);
}

void free_board(CellBoard* board) {
    free(board->cells);
    free(board->next_cells);
    free(board);
}
