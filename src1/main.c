#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// TODO all includes..
#include "editor/editor.h"
#include "renderer/font.h"
#include "renderer/renderer.h"

typedef union {
	int i;
	unsigned int ui;
	float f;
	const void *v;
} Arg;

typedef struct {
	unsigned int special_key;
    unsigned int key;
    void (*callback)();
    const Arg arg;
} Key;

#include "config.h"
#include "input/input.h"

struct texture_font_t* font_main;

void* window = NULL;

static int window_init_width = 1600;
static int window_init_height = 900;

void render();

static void error_callback(int error, const char* description) {
    PRINT_C(error, description);
}

static void framebuffer_size_callback(struct GLFWwindow* window, int width, int height) {
    renderer_framebuffer_width = width;
    renderer_framebuffer_height = height;
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
    render();
}

void center_window(GLFWwindow *window, GLFWmonitor *monitor) {
    if (!monitor)
        return;

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    if (!mode)
        return;

    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwSetWindowPos(window, monitorX + (mode->width - windowWidth) / 2, monitorY + (mode->height - windowHeight) / 2);
}

int main(void) {
    if (!glfwInit()) {
        ERROR("can't initialize glfw");
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    renderer_framebuffer_width = window_init_width;
    renderer_framebuffer_height = window_init_height;
    window = glfwCreateWindow(window_init_width, window_init_height, "Buff", NULL, NULL);
    // center_window(window, glfwGetPrimaryMonitor());
    if (window == NULL) {
        ERROR("can't create glfw window");
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetErrorCallback(error_callback);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum err = glewInit();
	if (GLEW_OK != err) {
		ERROR_M("can't initialize glew: ", glewGetErrorString(err));
		glfwTerminate();
	}

    PRINT_M("OpenGL", glGetString(GL_VERSION));
    PRINT_M("GLSL", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    glfwSetTime(0);

    renderer_init();
    editor_init_renderer();
    input_init();
    font_main = font_load_from_file("res/fonts/JetBrainsMono-Regular.ttf", 25);

    while (!glfwWindowShouldClose(window)) {
        //glfwWaitEvents();
        glfwPollEvents();
        input_handle();

        render();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void render() {
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    renderer_begin();

    editor_render(font_main);

    renderer_end();
}
