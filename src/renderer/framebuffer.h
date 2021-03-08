#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef struct {
    unsigned int id;
    unsigned int texture;
} FrameBuffer;

FrameBuffer* framebuffer_create();
void framebuffer_begin(FrameBuffer* framebuffer);
void framebuffer_end();

#endif // FRAMEBUFFER_H
