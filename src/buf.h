struct buf_t {
    char* buf;
    char* gap;
    int buf_size;
    int gap_size;
};

struct buf_t* buf_create();
void buf_insert(struct buf_t* b, unsigned int ch);
void buf_delete(struct buf_t* b);
void buf_left(struct buf_t* b);
void buf_right(struct buf_t* b);
