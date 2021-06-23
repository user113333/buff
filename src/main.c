#include <glad/glad.h>				// Or any other GL header of your choice.

#include <windows.h>
#include <stdio.h>					// malloc, free, fopen, fclose, ftell, fseek, fread
#include <string.h>					// memset
#define FONTSTASH_IMPLEMENTATION	// Expands implementation
#include "fontstash.h"


#include <GLFW/glfw3.h>
#include <stdlib.h>

#define GLFONTSTASH_IMPLEMENTATION	// Expands implementation
#include "glfontstash.h"
///////

#include "uix.h"
#include "buf.h"

void* window = NULL;

static int window_init_width = 1600;
static int window_init_height = 900;
struct FONScontext* fs;
struct buf_t* buf;

void render();

static void error_callback(int error, const char* description) {
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

void character_callback(GLFWwindow* window, unsigned int codepoint) {
    buf_insert(buf, codepoint);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    printf("%d", key);
}


// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//     if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
//         buf_delete(buf);
//     }

//     if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
//         buf_left(buf);
//     }

//     if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
//         buf_right(buf);
//     }
// }

int main(void) {
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    
    glfwSetErrorCallback(error_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    gladLoadGL();
    
    glfwSetTime(0);

    buf = buf_create();
    fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);

    // Add font to stash.
    int fontNormal = fonsAddFont(fs, "sans", "res/fonts/JetBrainsMono-Regular.ttf");
    fonsSetFont(fs, fontNormal);

    uix_context_init(fs);

    while (!glfwWindowShouldClose(window)) {
        render();

        glfwSwapBuffers(window);
        //glfwWaitEvents();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void render() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    // Update and render
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width,height,0,-1,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glColor4ub(255,255,255,255);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    float dx = 0, dy = 0;
    unsigned int white = glfonsRGBA(255,255,255,255);
    unsigned int brown = glfonsRGBA(192,128,0,128);

    fonsSetSize(fs, 124.0f);
    fonsSetColor(fs, white);
    fonsDrawText(fs, dx,dy,"The big ", NULL);

    fonsSetSize(fs, 24.0f);
    fonsSetAlign(fs, FONS_ALIGN_TOP);
    fonsSetColor(fs, brown);
    
    fonsDrawText(fs, dx,dy,"brown fox", NULL);
    fonsDrawText(fs, dx,dy,"brown fox", NULL);

    uix_newframe(window);

    uix_navbar_begin();
    uix_navbar_item("File");
    uix_navbar_item("Download");
    uix_navbar_end();

    uix_next_window_size(100, 100, UIX_COND_ONCE);
    uix_begin("treeview");
    uix_end();

    uix_next_window_size(100, 100, UIX_COND_ONCE);
    uix_begin("code");
    uix_button("ASDjJžžžćč");
    uix_button("2");
    uix_end();

    fonsDrawText(fs, 100, 100, buf->buf, buf->gap);
    fonsDrawText(fs, 100, 120, buf->gap + buf->gap_size, buf->buf + buf->buf_size);
}
