#include "editor.h"

#include <freetype-gl.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include "gap.h"
#include "math/utf32.h"
#include "buffer.h"
#include "renderer/font.h"
#include "renderer/renderer.h"

#define BFS_SELECTED &bfs[selected]

static char_t glyph_space = ' ';
static char_t glyph_underscore = '_';

static buffer_t bfs[10];
static int selected = EDITOR_NULL;

static int gbs_next_empty();
void cursor_render(float x, float y, float h);

void editor_init_renderer() {
    // TODO: fb_main = framebuffer_create();
}

void editor_create(const char* path) {
    int i = gbs_next_empty();
    editor_select(i);
    
    if (path == NULL) {
        buffer_init(BFS_SELECTED, "");
    } else {
        // TODO
    }
}

void editor_render(struct Font* font) {
    if (selected == EDITOR_NULL) {
        return;
    }

    buffer_t* buf = BFS_SELECTED;
    
    int pen_x = 0;
    int pen_y = 0;

    int i = 0;
    for (char_t* p = buf->b; p < buf->b + buf->bsize; p++) {
        if (gap_is_ptr_gap(buf, p)) {
            pen_x += font_render_char(font, &glyph_underscore, pen_x, pen_y, 1);
            continue;
        }

        if (buf->c == i) {
            cursor_render(pen_x, pen_y, font_line_height(font));
        }

        i++;

        switch (*p) {
            case '\n':
                pen_y += font_line_height(font);
                pen_x = 0;
            break;
            
            case '\t':
                for (int i = 0; i < config_tab_spaces; i++) {
                    pen_x += font_render_char(font, &glyph_space, pen_x, pen_y, 1);
                }
            break;

            default:
                pen_x += font_render_char(font, p, pen_x, pen_y, 1);
            break;
        }
    }

    if (buf->c == i) {
        cursor_render(pen_x, pen_y, font_line_height(font));
    }

    if (config_render_font_atlas) {
        font_render_atlas(font, 0, 0, 1);
    }
}

void editor_enter() {
    editor_insert('\n');
}

void editor_tab() {
    editor_insert('\t');
}

void editor_backspace() {
    buffer_delete_left(BFS_SELECTED, 1);
}

void editor_delete() {
    buffer_delete_right(BFS_SELECTED, 1);
}

void editor_dir(int dir) {
    switch (dir) {
        case DIR_LEFT: buffer_move_x(BFS_SELECTED, -1); break;
        case DIR_RIGHT: buffer_move_x(BFS_SELECTED, 1); break;
        case DIR_UP: buffer_move_y(BFS_SELECTED, -1); break;
        case DIR_DOWN: buffer_move_y(BFS_SELECTED, 1); break;
    }
}

void editor_end() {
    buffer_move_end(BFS_SELECTED);
}

void editor_home() {
    buffer_move_home(BFS_SELECTED);
}

void editor_select(int i) {
    selected = i;
}

void editor_insert(char_t ch) {
    if (selected == EDITOR_NULL) {
        return;
    }

    buffer_put(BFS_SELECTED, ch);
}

void editor_ctrl_v() {
    char* str = glfwGetClipboardString(window);
    utf32* str_utf32 = to_utf32(str);
    buffer_paste(BFS_SELECTED, str_utf32);
}

int editor_len() {
    return vector_size(bfs);
}

int gbs_next_empty() {
    return 0; // TODO
}

void cursor_render(float x, float y, float h) {
    renderer_draw_quad(x + 1, y, 1, h, 1, 1, 1, 1);
}
