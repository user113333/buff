#ifndef FONT_H
#define FONT_H

#include "core.h"

typedef struct Font;

struct Font* font_load_from_file(const char* path, unsigned int size);
void font_render_atlas(struct Font* font, float pos_x, float pos_y, float scale);
int font_line_height(struct Font* font);

int font_render_char(struct Font* font, char_t* c, float pos_x, float pos_y, float scale);

#endif // FONT_H
