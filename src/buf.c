#include "buf.h"

#include <stdlib.h>
#include <string.h>

struct buf_t* buf_create() {
    struct buf_t* b = malloc(sizeof(struct buf_t));
    
    b->buf_size = 100;
    b->gap_size = 100;
    b->buf = calloc(100, sizeof(char));
    b->gap = b->buf;
    
    return b;
}

void buf_insert(struct buf_t* b, unsigned int ch) {
    char b1 = (ch & 0b11111111);
    *b->gap = b1;
    b->gap++;
    b->gap_size--;
}

void buf_delete(struct buf_t* b) {
    b->gap--;
    b->gap_size++;
}

void buf_left(struct buf_t* b) {
    b->gap--;

    memcpy(b->gap + b->gap_size, b->gap, sizeof(char));
}

void buf_right(struct buf_t* b) {
    memcpy(b->gap, b->gap + b->gap_size, sizeof(char));

    b->gap++;
}
