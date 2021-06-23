#define GLX_COLOR(R, G, B, A) (struct glx_color_t){ R, G, B, A }

struct glx_color_t {
    float r, g, b, a;
};

void glx_viewport(float x, float y, float width, float height);
void glx_scissors(float x, float y, float width, float height);
void glx_rect(float x, float y, float width, float height, struct glx_color_t color);
