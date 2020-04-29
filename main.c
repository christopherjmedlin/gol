#include <stdio.h>
#include <time.h>
#include "ui.h"

int main(int argc, char* argv[]) {
    char* filename;

    init_ncurses();
    if (argc == 2) {
        filename = argv[1];
        run(filename);
    } else {
        run(NULL);
    }
    endwin();
    return 0;
}
