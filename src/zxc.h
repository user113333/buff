#define HASHMAP_MAX_COUNT 1000

#define V2(X, Y) (struct v2_t){ X, Y }
#define RECT(X, Y, WIDTH, HEIGHT) (struct rect_t){ V2(X, Y), V2(WIDTH, HEIGHT) }

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct v2_t {
    float x, y;
};

struct rect_t {
    struct v2_t pos;
    struct v2_t size;
};

struct hashmap_t {
    void* data;
    int data_size;
    int data_count;
};

// util
unsigned long zxc_hash(const char*);

// collision
int zxc_rect_v2(struct rect_t rect, struct v2_t v2);

// hashmap
struct hashmap_t* hashmap_create(int val_size);
void hashmap_free(struct hashmap_t* m);
void* hashmap_get(struct hashmap_t* m, unsigned long key);

#ifdef ZXC_IMPLEMENTATION

#include <stdlib.h>

unsigned long zxc_hash(const char* str) {
    unsigned long hash = 5381;
    int c;

    while (c = (unsigned char)*str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int zxc_rect_v2(struct rect_t rect, struct v2_t v2) {
    v2.x -= rect.pos.x;
    v2.y -= rect.pos.y;

    return v2.x < rect.size.x && v2.x > 0 && v2.y < rect.size.y && v2.y > 9;
}

struct hashmap_t* hashmap_create(int val_size) {
    struct hashmap_t* m = malloc(sizeof(struct hashmap_t));
    m->data = calloc(HASHMAP_MAX_COUNT, val_size);
    m->data_size = val_size;
    m->data_count = HASHMAP_MAX_COUNT;
}

void hashmap_free(struct hashmap_t* m) {
    free(m->data);
    free(m);
}

void* hashmap_get(struct hashmap_t* m, unsigned long key) {
    key %= m->data_count;

    return m->data + key * m->data_size;
}

#endif
