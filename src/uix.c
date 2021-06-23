#include "uix.h"

#include <stdlib.h>
#include <glad/include/glad/glad.h> // TODO: delete

#include <glfw/include/GLFW/glfw3.h>
#include <fontstash/src/fontstash.h>

#include "glx.h"
#include "zxc.h"

#define SETEQ(TARGET, VAL) if (VAL != 0) TARGET = VAL
#define SETGT(TARGET, VAL) if (VAL > 0) TARGET = VAL
#define SETLT(TARGET, VAL) if (VAL < 0) TARGET = VAL

#define SCISSORS(X, Y, W, H) glx_scissors(X, backend.framebuffer_height - Y - H, W, H)

struct uix_style_t {
    float padding;
    float font_padding_x;
    float font_padding_y;
    float font_size;

    struct glx_color_t color_bg_primary;
    struct glx_color_t color_bg_secondary;
    struct glx_color_t color_bg_tertiary;
} style = { 5, 10, 5, 16, GLX_COLOR(0.3, 0.3, 0.3, 1), GLX_COLOR(0.2, 0.2, 0.2, 1), GLX_COLOR(0.1, 0.1, 0.1, 1) };

struct uix_mouse_t {
    struct v2_t pos;
    struct v2_t pos_delta;
    int left_pressed;
    int right_pressed;

    int left_state, left_state_last;
    int right_state, right_state_last;
    struct v2_t pos_last;
};

struct uix_keyboard_t {
    int key_pressed[UIX_KEY_LAST];
    int key_state[UIX_KEY_LAST];
    int key_state_last[UIX_KEY_LAST];
};

struct uix_backend_t {
    int framebuffer_width;
    int framebuffer_height;
};

struct window_t {
    float x, y, width, height;
};

struct uix_context_t {
    struct FONScontext* fs;
    struct hashmap_t* windows;

    float font_height;

    struct v2_t c;
    struct window_t* window;
    struct window_t window_next;
    struct window_t* selected_window;
    int selected_window_mode;

    int frames;
    int cond;

    int fbcursor_x;
    int fbcursor_y;
    int nav_id;
};

static struct uix_mouse_t mouse;
static struct uix_keyboard_t keyboard;
static struct uix_backend_t backend;

static struct uix_context_t context;

void uix_backend_newframe(void* window) {
    // mouse
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    mouse.pos_last = mouse.pos;
    mouse.pos.x = mx;
    mouse.pos.y = my;
    mouse.pos_delta.x = mouse.pos.x - mouse.pos_last.x;
    mouse.pos_delta.y = mouse.pos.y - mouse.pos_last.y;
    mouse.left_state_last = mouse.left_state;
    mouse.left_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    mouse.left_pressed = mouse.left_state && mouse.left_state_last == 0;
    mouse.right_state_last = mouse.right_state;
    mouse.right_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    mouse.right_pressed = mouse.right_state && mouse.right_state_last == 0;

    // keyboard
    for (int i = UIX_KEY_SPACE; i < UIX_KEY_LAST; i++) {
        keyboard.key_state_last[i] = keyboard.key_state[i];
        keyboard.key_state[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }

    for (int i = 0; i < UIX_KEY_LAST; i++) {
        keyboard.key_pressed[i] = !keyboard.key_state_last[i] && keyboard.key_state[i];
    }

    // backend
    glfwGetFramebufferSize(window, &backend.framebuffer_width, &backend.framebuffer_height);
}

int uix_cond_check(int cond) {
    return cond & context.cond;
}

void uix_cursor_y(int y) {
    context.c.y += y;
    context.c.y += style.padding;
}

// ====== INITS ======

void uix_context_init(struct FONScontext* fs) {
    context.fbcursor_x = 0;
    context.fbcursor_y = 0;
    context.nav_id = -1;

    context.fs = fs;
    context.windows = hashmap_create(sizeof(struct window_t));

    int font = fonsAddFont(context.fs, "sans", "res/fonts/JetBrainsMono-Regular.ttf");
    fonsSetFont(context.fs, font);
    fonsLineBounds(fs, style.font_size, &context.font_height, &context.font_height);

    context.cond = UIX_COND_ALL;
}

void uix_newframe(void* window) {
    context.fbcursor_x = 0;
    context.fbcursor_y = 0;

    // update cond
    if (context.cond & UIX_COND_ONCE && context.frames) {
        context.cond = UIX_COND_ALWAYS;
    }

    // update window sizes
    if (context.selected_window != NULL) {
        if (context.selected_window_mode == 0b10) {
            context.selected_window->x += mouse.pos_delta.x;
            context.selected_window->y += mouse.pos_delta.y;
        } else if (context.selected_window_mode == 0b01) {
            context.selected_window->width += mouse.pos_delta.x;
            context.selected_window->height += mouse.pos_delta.y;
        }

        if ((mouse.left_state == 0 && context.selected_window_mode == 0b10) || (mouse.right_state == 0 && context.selected_window_mode == 0b01)) {
            context.selected_window = NULL;
        }
    }

    uix_backend_newframe(window);
    context.frames++;
}

// ====== HELPER ======

void uix_next_window_size(float width, float height, enum UIX_COND_ cond) {
    if (!uix_cond_check(cond)) {
        return;
    }

    context.window_next.width = width;
    context.window_next.height = height;
}

// ====== WINDOW ======

void uix_begin(const char* name) {
    struct window_t* w = hashmap_get(context.windows, zxc_hash(name));
    context.window = w;

    SETGT(w->x, context.window_next.x);
    SETGT(w->y, context.window_next.y);
    SETGT(w->width, context.window_next.width);
    SETGT(w->height, context.window_next.height);

    if ((mouse.left_pressed || mouse.right_pressed) && keyboard.key_state[UIX_KEY_LEFT_CONTROL] && zxc_rect_v2((struct rect_t){w->x, w->y, w->width, w->height}, mouse.pos)) {
        context.selected_window = w;
        context.selected_window_mode = mouse.left_pressed * 0b10 | mouse.right_pressed;
    }

    glx_rect(w->x, w->y, w->width, w->height, style.color_bg_tertiary);
    context.c = (struct v2_t){ w->x + style.padding, w->y + style.padding };
    // glx_scissors(context.c.x, context.c.y, w->width - style.padding * 2, w->height - style.padding * 2);
    
    float width = w->width - style.padding * 2;
    float height = w->height - style.padding * 2;
    SCISSORS(context.c.x, context.c.y, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}

void uix_end() {
    glx_scissors(0, 0, backend.framebuffer_width, backend.framebuffer_height);

    context.window_next.x = -1;
    context.window_next.y = -1;
    context.window_next.width = -1;
    context.window_next.height = -1;

    if (context.window->width < 0) context.window->width = 0;
    if (context.window->height < 0) context.window->height = 0;
}

// ====== NAVBAR ======

void uix_navbar_begin() {
    glx_rect(0, context.fbcursor_y, backend.framebuffer_width, context.font_height + style.font_padding_y * 2, style.color_bg_primary);
}

void uix_navbar_end() {
    context.fbcursor_y += context.font_height + style.font_size * 2;
}

int uix_navbar_item(const char* name) {
    int focus = 0;
    float x = context.fbcursor_x;
    float y = context.fbcursor_y;
    float width = fonsTextBounds(context.fs, 0, 0, name, NULL, NULL) + style.font_padding_x * 2;
    float height = context.font_height + style.font_padding_y * 2;

    if (zxc_rect_v2(RECT(x, y, width, height), V2(mouse.pos.x, mouse.pos.y))) {
        glx_rect(x, y, width, height, style.color_bg_secondary);

        if (mouse.left_pressed) {
            glx_rect(x, y, width, height, style.color_bg_tertiary);
        }

        focus = 1;
    }

    fonsDrawText(context.fs, x + style.font_padding_x, y + style.font_padding_y, name, NULL);

    context.fbcursor_x += width;

    return focus && mouse.left_pressed;
}

// ====== ITEMS ======

int uix_button(const char* name) {
    float bounds = fonsTextBounds(context.fs, 0, 0, name, NULL, NULL);

    glx_rect(context.c.x, context.c.y, bounds + 2 * style.font_padding_x, context.font_height + 2 * style.font_padding_y, style.color_bg_primary);
    fonsDrawText(context.fs, context.c.x + style.font_padding_x, context.c.y + style.font_padding_y, name, NULL);

    uix_cursor_y(context.font_height + 2 * style.font_padding_y);
}
