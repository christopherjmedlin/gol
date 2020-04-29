#include <assert.h>
#include "gol.h"
#include <stdio.h>

// helper function for doing x generations
void generations(CellBoard* c, int x) {
    for (int i = 0; i < x; i++) {
        next_generation(c);
    }
}

void test_cell_board_basic_ops() {
    CellBoard* c = init_board(5, 10);

    assert(c->size_x == 5);
    assert(c->size_y == 10);
    assert(c->cells);

    set_cell(c, 3, 4, true);
    assert(get_cell(c, 3, 4));

    // insertion near end of array
    set_cell(c, 4, 9, true);
    assert(get_cell(c, 4, 9));

    // wrap around
    set_cell(c, 5, 10, true);
    assert(get_cell(c, 0, 0));

    free_board(c);
}

// 2x2 should stay alive always.
void test_2x2() {
    CellBoard* c = init_board(4, 4);
    
    set_cell(c, 1, 1, true);
    set_cell(c, 1, 2, true);
    set_cell(c, 2, 1, true);
    set_cell(c, 2, 2, true);

    generations(c, 20);

    assert(get_cell(c, 1, 1));
    assert(get_cell(c, 1, 2));
    assert(get_cell(c, 2, 1));
    assert(get_cell(c, 2, 2));
    
    free_board(c);
}

void test_t_shape() {
    CellBoard* c = init_board(30, 30);
    
    set_cell(c, 15, 15, true);
    set_cell(c, 16, 15, true);
    set_cell(c, 17, 15, true);
    set_cell(c, 16, 16, true);
    
    generations(c, 10);

    assert(!get_cell(c, 15, 15));
    assert(get_cell(c, 19, 15));
    assert(get_cell(c, 13, 15));
    
    free_board(c);
}

int main() {
    test_cell_board_basic_ops();
    test_2x2();
    test_t_shape();
    printf("\033[0;32mTests ran without issue!\033[0m\n");
}
