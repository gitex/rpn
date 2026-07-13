#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdio.h>

#include "string.h"

#define ESC "\033"
#define CSI "["
#define OSC "]"

#define FIRST_COL 1
#define FIRST_ROW 1

typedef enum {
    RESET   = 0,
    FG_BLACK   = 30,
    FG_RED     = 31,
    FG_GREEN   = 32,
    FG_YELLOW  = 33,
    FG_BLUE    = 34,
    FG_MAGENTA = 35,
    FG_CYAN    = 36,
    FG_WHITE   = 37,
} Color;

#define colorize(text, color)  (ESC (color) "m" (text) ESC "0m")
#define color_start(color)     (ESC (color) "m")
#define color_end()            (ESC "0m")

#define UPPER_LEFT_CORNER   (ESC CSI "H")
#define CLEAR_CURRENT_ROW   (ESC CSI "2K")
#define CLEAR_ALL           (ESC CSI "2J")

#define term_clear_all()            printf(CLEAR_ALL)
#define term_move_to(row, col)      printf(ESC CSI "%lu;%luH", (row), (col))
#define term_clear(row)             printf(CLEAR_CURRENT_ROW)
#define term_print(s)               printf("%.*s", (int)(s).len, (s).chars)
#define term_print_row(s)           printf("%.*s\n", (int)(s).len, (s).chars)
#define term_print_colorized(s, c)  printf("%.*s", (int)(s).len, (s).chars)

#endif
