#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>

extern void* window;
typedef unsigned int char_t;

#define SIGN(x) ((0 < x) - (x < 0))

#define PRINT(x) printf("[INFO] %s\n", x)
#define ERROR(x) printf("[ERROR] %s\n", x); exit(EXIT_FAILURE)

#define PRINT_C(x, y) printf("[INFO] (%d) %s\n", x, y)
#define ERROR_C(x, y) printf("[ERROR] (%d) %s\n", x, y); exit(EXIT_FAILURE)

#define PRINT_M(x, y) printf("[INFO] %s %s\n", x, y)
#define ERROR_M(x, y) printf("[ERROR] %s %s\n", x, y); exit(EXIT_FAILURE)

enum {
    KEY_SPECIAL_SHIFT = 1 << 0,
    KEY_SPECIAL_CTRL = 1 << 1,
    KEY_SPECIAL_ALT = 1 << 2,
};

enum {
    UIFWindow_None = 0,
    UIFWindow_NoTitleBar = 1 << 0,
    UIFWindow_NoResize = 1 << 1,
    UIFWindow_NoMove = 1 << 2,
    UIFWindow_NoScrollbar = 1 << 3,
    UIFWindow_NoScrollWithMouse = 1 << 4,
    UIFWindow_NoCollapse = 1 << 5,
};

enum {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
};

#endif // CORE_H
