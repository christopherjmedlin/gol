#include <stdio.h>
#include <time.h>
#include "ui.h"

int main() {
    init_ncurses();
    run();
    endwin();
    return 0;
}
