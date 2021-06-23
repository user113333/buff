#include "glx.h"

#include <glad/include/glad/glad.h>

#include "zxc.h"

void glx_viewport(float x, float y, float width, float height) {
    width = MAX(0, width);
    height = MAX(0, height);

    glViewport(x, y, width, height);
}

void glx_scissors(float x, float y, float width, float height) {
    glEnable(GL_SCISSOR_TEST);
    
    width = MAX(0, width);
    height = MAX(0, height);
    
    glScissor(x, y, width, height);
}

void glx_rect(float x, float y, float width, float height, struct glx_color_t color) {
    glBegin(GL_QUADS);
    
        glColor3d(color.r, color.g, color.b);
        glVertex3f(x, y, 0);
        glColor3d(color.r, color.g, color.b);
        glVertex3f(x, y + height, 0);
        glColor3d(color.r, color.g, color.b);
        glVertex3f(x + width, y + height, 0);
        glColor3d(color.r, color.g, color.b);
        glVertex3f(x + width, y, 0);
    
    glEnd();
}
