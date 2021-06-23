#include "buffer.h"

#include "gap.h"
#include "math/utf32.h"

#define GROW_SIZE 10

#define CURSOR_PTR buffer_c_pos_to_ptr(buffer, buffer->c)
#define CURSOR_POS(PTR) buffer_ptr_to_c_pos(buffer, PTR)

// funcs

int buffer_ptr_to_c_pos(buffer_t* buffer, char_t* c_ptr) {
    if (c_ptr > buffer->gap) {
        c_ptr -= buffer->gapsize;
    }

    return c_ptr - buffer->b;
}

char_t* buffer_c_pos_to_ptr(buffer_t* buffer, int c_pos) {
    char_t* c_ptr = buffer->b + c_pos;
    
    if (c_ptr > buffer->gap) {
        c_ptr += buffer->gapsize;
    }

    return c_ptr;
}

void buffer_grow(buffer_t* buffer, int size) {
    buffer->gapsize += size;
    buffer->bsize = buffer->bsize + size;
    buffer->b = realloc(buffer->b, buffer->bsize * sizeof(char_t));
    buffer->gap = buffer->b + (buffer->bsize - buffer->gapsize);
}

void buffer_grow_if(buffer_t* buffer, int space_needed, int size) {
    if (buffer->gapsize >= space_needed) {
        return;
    }

    buffer_grow(buffer, size);
}

void buffer_delete(buffer_t* buffer) {
    if (buffer->c == buffer->bsize - buffer->gapsize) {
        return;
    }

    buffer->gapsize++;
}

char_t* buffer_next_target(buffer_t* buffer, char_t* start, char_t target, int step) {
    while (start[0] != target) {
        start += step;

        if (start <= buffer->b) {
            return buffer->b;
        } else if (start >= buffer->b + buffer->bsize) {
            return buffer->b + buffer->bsize;
        }

        // TODO: delete?
        //if (gap_is_ptr_gap(buffer, start)) {
        //    continue;
        //}
    }

    return start;
}

// TODO: delete?
int buffer_next_target_count(buffer_t* buffer, char_t* start, char_t target, int step) {
    int c = 0;

    while (start[0] != target) {
        start += step;

        if (start <= buffer->b) {
            return buffer->b;
        } else if (start >= buffer->b + buffer->bsize) {
            return buffer->b + buffer->bsize;
        }

        if (gap_is_ptr_gap(buffer, start)) {
            continue;
        }

        c++;
    }

    return c;
}

// defs

void buffer_init(buffer_t* buffer, char_t* str) {
    int strsize = strlen(str);
    buffer->bsize = GROW_SIZE + strsize;
    buffer->b = malloc(buffer->bsize * sizeof(char_t));
    buffer->c = 0;
    buffer->c_x = 0;
    buffer->gap = buffer->b + strsize;
    buffer->gapsize = GROW_SIZE;
}

void buffer_save(buffer_t* buffer, char_t* str) {
    // TODO:
}

void buffer_put(buffer_t* buffer, char_t ch) {
    buffer_grow_if(buffer, 1, GROW_SIZE);
    gap_reposition(buffer);

    buffer->b[buffer->c] = ch;
    buffer->gap += 1;
    buffer->gapsize -= 1;
    buffer->c += 1;
}

void buffer_paste(buffer_t* buffer, utf32* str_utf32) {
    int size = utf32_len(str_utf32);

    buffer_grow(buffer, size);
    gap_reposition(buffer);
    
    memcpy(buffer->b + buffer->c, str_utf32, size * sizeof(utf32));
    buffer->gap += size;
    buffer->gapsize -= size;
    buffer->c += size;
}

void buffer_move_x(buffer_t* buffer, int offset) {
    int next = buffer->c + offset;
    if (next >= 0 && next <= buffer->bsize - buffer->gapsize) {
        buffer->c = next;
    }
}

void buffer_move_y(buffer_t* buffer, int offset) {
    char_t* c_ptr = buffer_next_target(buffer, (offset > 0 ? CURSOR_PTR : CURSOR_PTR + offset), '\n', offset);
    
    if (offset > 0 && c_ptr != buffer->b + buffer->bsize) {
        c_ptr++;
    }

    buffer->c = CURSOR_POS(c_ptr);
}

void buffer_move_end(buffer_t* buffer) {
    char_t* c_ptr = buffer_next_target(buffer, CURSOR_PTR, '\n', 1);
    buffer->c = CURSOR_POS(c_ptr);
}

void buffer_move_home(buffer_t* buffer) {
    char_t* c_ptr = buffer_next_target(buffer, CURSOR_PTR - 1, '\n', -1);

    if (c_ptr != buffer->b + buffer->bsize && c_ptr != buffer->b) {
        c_ptr++;
    }

    buffer->c = CURSOR_POS(c_ptr);
}

void buffer_delete_left(buffer_t* buffer, int len) {
    gap_reposition(buffer);
    for (int i = 0; i < len; i++) {
        if (buffer->c == 0) {
            return;
        }

        buffer->c--;
        buffer->gap--;
        buffer_delete(buffer);
    }
}

void buffer_delete_right(buffer_t* buffer, int len) {
    gap_reposition(buffer);
    for (int i = 0; i < len; i++) {
        buffer_delete(buffer);
    }
}
