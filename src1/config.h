#ifndef IO_H
#define IO_H

int config_tab_spaces = 4;
int config_render_font_atlas = 0;

Key keys[] = {
    { KEY_SPECIAL_CTRL,                     GLFW_KEY_N, editor_create, {NULL} },
    { KEY_SPECIAL_CTRL,                     GLFW_KEY_V, editor_ctrl_v, {0} },
    { KEY_SPECIAL_CTRL,                     GLFW_KEY_O, NULL, {0} },
    { KEY_SPECIAL_CTRL,                     GLFW_KEY_K, NULL, {0} },
    { KEY_SPECIAL_CTRL,                     GLFW_KEY_S, NULL, {0} },
    { KEY_SPECIAL_CTRL | KEY_SPECIAL_SHIFT, GLFW_KEY_S, NULL, {0} },
};

#endif // IO_H
