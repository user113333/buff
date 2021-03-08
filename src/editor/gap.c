#include "gap.h"

#include <string.h>

void gap_reposition(buffer_t* buffer) {
    int gap_pos = buffer->gap - buffer->b;
    if (buffer->c == gap_pos) {
        return;
    }

    char_t* c_ptr = NULL;
    char_t* dest = NULL;
    char_t* src = NULL;
    int size = NULL;
    if (buffer->c < gap_pos) {
        c_ptr = buffer->b + buffer->c;
        src = c_ptr;
        dest = src + buffer->gapsize;
        size = buffer->gap - c_ptr;
    } else if (gap_pos < buffer->c) {
        c_ptr = buffer->b + buffer->c + buffer->gapsize;
        src = buffer->gap + buffer->gapsize;
        dest = src - buffer->gapsize;
        size = c_ptr - src;
    }

    memcpy(dest, src, size);
    buffer->gap = c_ptr;
}

int gap_is_ptr_gap(buffer_t* buffer, char_t* ptr) {
    return ptr >= buffer->gap && ptr < buffer->gap + buffer->gapsize;
}
