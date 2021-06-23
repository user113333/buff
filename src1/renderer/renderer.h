#ifndef RENDERER_H
#define RENDERER_H

#include "texture.h"

#define DRAW_TEXTURE_SOURCE(texture, pos_x, pos_y, size_x, size_y, s0, t0, s1, t1) renderer_draw_texture(texture, pos_x, pos_y, size_x, size_y, s0, t0, s1, t1, 1, 1, 1, 1)
#define DRAW_TEXTURE(texture, pos_x, pos_y, size_x, size_y) renderer_draw_texture(texture, pos_x, pos_y, size_x, size_y, 0, 0, 1, 1, 1, 1, 1, 1)

typedef struct GLFWwindow;

extern int renderer_framebuffer_width;
extern int renderer_framebuffer_height;

unsigned int renderer_create_texture();

void renderer_init();
void renderer_begin();
void renderer_end();

void renderer_draw_quad(float pos_x, float pos_y,
    float size_x, float size_y,
    float color_r, float color_g, float color_b, float color_a);

void renderer_draw_texture(
    unsigned int texture_id,
    float pos_x, float pos_y,
    float size_x, float size_y,
    float x0, float y0,
    float x1, float y1,
    float color_r, float color_g, float color_b, float color_a);

#endif // RENDERER_H
