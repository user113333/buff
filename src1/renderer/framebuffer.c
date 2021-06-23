#include "framebuffer.h"

#include <GL/glew.h>

#include "renderer.h"

const int framebuffer_width = 1920;
const int framebuffer_height = 1080;

FrameBuffer* framebuffer_create() {
    FrameBuffer* fb = malloc(sizeof(FrameBuffer));
    
    glGenFramebuffers(1, &fb->id);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->id);

    glGenTextures(1, &fb->texture);
    glBindTexture(GL_TEXTURE_2D, fb->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer_width, framebuffer_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb->texture, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // ?
    
    return fb;
}

void framebuffer_begin(FrameBuffer* framebuffer) {
    glViewport(0, 0, framebuffer_width, framebuffer_height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void framebuffer_end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, renderer_framebuffer_width, renderer_framebuffer_height);
}
