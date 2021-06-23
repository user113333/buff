#ifndef BUFFER_H
#define BUFFER_H

#include "core.h"

typedef struct {
    char_t* b;
    int bsize;

    char_t* gap;
    int gapsize;

    int c;
    int c_x;
} buffer_t;

void buffer_init(buffer_t* buffer, char_t* str);
void buffer_save(buffer_t* buffer, char_t* str);
void buffer_put(buffer_t* buffer, char_t ch);
void buffer_paste(buffer_t* buffer, unsigned int* str_utf32);
void buffer_move_x(buffer_t* buffer, int offset);
void buffer_move_y(buffer_t* buffer, int offset);
void buffer_move_end(buffer_t* buffer);
void buffer_move_home(buffer_t* buffer);
void buffer_delete_left(buffer_t* buffer, int len);
void buffer_delete_right(buffer_t* buffer, int len);

#endif // BUFFER_H
