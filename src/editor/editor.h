#ifndef EDITOR_H
#define EDITOR_H

#include "core.h"

#define EDITOR_NULL -1

typedef struct Font;

extern int config_tab_spaces;
extern int config_render_font_atlas;

void editor_create(const char* path);
void editor_render(struct Font* font);

void editor_enter();
void editor_tab();
void editor_backspace();
void editor_delete();
void editor_dir(int dir);
void editor_end();
void editor_home();

void editor_select(int i);
void editor_insert(char_t ch);
void editor_ctrl_v();
int editor_len();

#endif // EDITOR_H
