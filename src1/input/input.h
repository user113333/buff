#ifndef INPUT_H
#define INPUT_H

// TODO: REWRITE

#define KCE(GLFW_KEY, ACTION) case GLFW_KEY: if (action == GLFW_PRESS || action == GLFW_REPEAT) { ACTION; } break

static int input_keys[500];
static int input_keys_shift = 0;
static int input_keys_control = 0;
static int input_keys_alt = 0;

static void input_keys_reset();
static int special_key_check(int special);

void character_callback(GLFWwindow* window, unsigned int codepoint) {
    editor_insert((char_t)codepoint);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key) {
        // case GLFW_KEY_ENTER: if (action == GLFW_PRESS || action == GLFW_REPEAT) { editor_enter(); } break;
        KCE(GLFW_KEY_ENTER, editor_enter());
        KCE(GLFW_KEY_TAB, editor_tab());
        KCE(GLFW_KEY_BACKSPACE, editor_backspace());
        KCE(GLFW_KEY_DELETE, editor_delete());
        KCE(GLFW_KEY_LEFT, editor_dir(DIR_LEFT));
        KCE(GLFW_KEY_RIGHT, editor_dir(DIR_RIGHT));
        KCE(GLFW_KEY_UP, editor_dir(DIR_UP));
        KCE(GLFW_KEY_DOWN, editor_dir(DIR_DOWN));
        KCE(GLFW_KEY_END, editor_end());
        KCE(GLFW_KEY_HOME, editor_home(DIR_DOWN));
        
        case GLFW_KEY_LEFT_SHIFT: input_keys_shift = action; break;
        case GLFW_KEY_LEFT_CONTROL: input_keys_control = action; break;
        case GLFW_KEY_LEFT_ALT: input_keys_alt = action; break;
    }

    input_keys[key] = action;
}

void input_init() {
    input_keys_reset();
}

void input_handle() {
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        Key* k = &keys[i];

        if (input_keys[k->key] == GLFW_PRESS && special_key_check(k->special_key)) {
            k->callback(k->arg);
        }
    }

    input_keys_reset();
}

void input_keys_reset() {
    for (int i = 0; i < sizeof(input_keys) / sizeof(input_keys[0]); i++) {
        input_keys[i] = 0;
    }
}

int special_key_check(int special) {
    int out = 1;
    
    out &= !(special & KEY_SPECIAL_SHIFT) || input_keys_shift;
    out &= !(special & KEY_SPECIAL_CTRL) || input_keys_control;
    out &= !(special & KEY_SPECIAL_ALT) || input_keys_alt;

    return out;
}

#endif // INPUT_H
