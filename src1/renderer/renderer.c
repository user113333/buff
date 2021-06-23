#include "renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "core.h"
#include "math/ortho.h"

typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y, z, w;
} vec4;

typedef struct {
    vec3 position;
    vec4 color;
    vec2 tex_coords;
    float tex_index;
} Vertex;

void GLAPIENTRY opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void quad_insert_into_buffer(float pos_x, float pos_y, float size_x, float size_y, float x0, float y0, float x1, float y1, float color_r, float color_g, float color_b, float color_a, int tex_index);

static const int max_quad_count = 10;
static const int max_vertex_count = max_quad_count * 4;
static const int max_index_count = max_quad_count * 6;
static const int max_textures = 32;

int renderer_framebuffer_width = 0;
int renderer_framebuffer_height = 0;

static unsigned int VBO = 0;
static unsigned int EBO = 0;
static unsigned int VAO = 0;
static unsigned int program = 0;
static unsigned int index_count = 0;
static unsigned int texture_slot_index = 1;

static unsigned int texture_white = 0;
static Vertex* quad_buffer = NULL;
static Vertex* quad_buffer_ptr = NULL;
static unsigned int* texture_slots = NULL;
static float* mvp = NULL;

unsigned int renderer_create_texture() {
    unsigned int id = 0;
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return id;
}

void renderer_init() {
    quad_buffer = malloc(sizeof(Vertex) * max_vertex_count);
    texture_slots = malloc(sizeof(unsigned int) * max_textures);
    mvp = malloc(sizeof(float) * 16);

    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);
    
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(opengl_error_callback, 0);

    program = shader_load_from_file("res/shaders/main.glsl");
    glUseProgram(program);
    int samplers[max_textures];
    for (int i = 0; i < max_textures; i++) {
        samplers[i] = i;
    }
    glUniform1iv(glGetUniformLocation(program, "u_textures"), max_textures, samplers);

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, max_vertex_count * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexArrayAttrib(VAO, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexArrayAttrib(VAO, 1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glEnableVertexArrayAttrib(VAO, 2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glEnableVertexArrayAttrib(VAO, 3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_index));
    
    unsigned int indices[max_index_count];
    unsigned int offset = 0;
    for (int i = 0; i < max_index_count; i += 6) {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    texture_white = renderer_create_texture();
    unsigned int color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    for (int i = 0; i < max_textures; i++) {
        texture_slots[i] = texture_white;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void renderer_begin() {
    quad_buffer_ptr = quad_buffer;
}

void renderer_end() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);

    glUseProgram(program);

    mat4_ortho(mvp, 0, renderer_framebuffer_width, renderer_framebuffer_height, 0, -1, 1);
    glUniformMatrix4fv(glGetUniformLocation(program, "u_mvp"), 1, GL_FALSE, mvp);

    GLsizeiptr size = (void*)quad_buffer_ptr - (void*)quad_buffer;
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, quad_buffer);

    for (int i = 0; i < texture_slot_index; i++) {
        glBindTextureUnit(i, texture_slots[i]);
    }

    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, NULL);

    index_count = 0;
    texture_slot_index = 1;
}

void renderer_draw_quad(float pos_x, float pos_y, float size_x, float size_y, float color_r, float color_g, float color_b, float color_a) {
    renderer_draw_texture(texture_white, pos_x, pos_y, size_x, size_y, 0, 0, 1, 1, color_r, color_g, color_b, color_a);
}

void renderer_draw_texture(unsigned int texture_id, float pos_x, float pos_y, float size_x, float size_y, float s0, float t0, float s1, float t1, float color_r, float color_g, float color_b, float color_a) {
    if (index_count >= max_index_count) {
        renderer_end();
        renderer_begin();
    }

    int id = texture_slots_find(texture_id);
    if (id == -1) {
        if (texture_slot_index >= max_textures) {
            renderer_end();
            renderer_begin();
            renderer_draw_texture(texture_id, pos_x, pos_y, size_x, size_y, s0, t0, s1, t1, color_r, color_g, color_b, color_a);
            return;
        }

        id = texture_slot_index;
        texture_slots[texture_slot_index] = texture_id;
        texture_slot_index++;
    }

    quad_insert_into_buffer(pos_x, pos_y, size_x, size_y, s0, t0, s1, t1, color_r, color_g, color_b, color_a, id);
    index_count += 6;
}

void quad_insert_into_buffer(float pos_x, float pos_y, float size_x, float size_y, float s0, float t0, float s1, float t1, float color_r, float color_g, float color_b, float color_a, int tex_index) {
    quad_buffer_ptr->position.x = pos_x;
    quad_buffer_ptr->position.y = pos_y;
    quad_buffer_ptr->position.z = 0;
    quad_buffer_ptr->tex_coords.x = s0;
    quad_buffer_ptr->tex_coords.y = t0;
    quad_buffer_ptr->color.x = color_r;
    quad_buffer_ptr->color.y = color_g;
    quad_buffer_ptr->color.z = color_b;
    quad_buffer_ptr->color.w = color_a;
    quad_buffer_ptr->tex_index = tex_index;
    quad_buffer_ptr++;

    quad_buffer_ptr->position.x = pos_x + size_x;
    quad_buffer_ptr->position.y = pos_y;
    quad_buffer_ptr->position.z = 0;
    quad_buffer_ptr->tex_coords.x = s1;
    quad_buffer_ptr->tex_coords.y = t0;
    quad_buffer_ptr->color.x = color_r;
    quad_buffer_ptr->color.y = color_g;
    quad_buffer_ptr->color.z = color_b;
    quad_buffer_ptr->color.w = color_a;
    quad_buffer_ptr->tex_index = tex_index;
    quad_buffer_ptr++;

    quad_buffer_ptr->position.x = pos_x + size_x;
    quad_buffer_ptr->position.y = pos_y + size_y;
    quad_buffer_ptr->position.z = 0;
    quad_buffer_ptr->tex_coords.x = s1;
    quad_buffer_ptr->tex_coords.y = t1;
    quad_buffer_ptr->color.x = color_r;
    quad_buffer_ptr->color.y = color_g;
    quad_buffer_ptr->color.z = color_b;
    quad_buffer_ptr->color.w = color_a;
    quad_buffer_ptr->tex_index = tex_index;
    quad_buffer_ptr++;

    quad_buffer_ptr->position.x = pos_x;
    quad_buffer_ptr->position.y = pos_y + size_y;
    quad_buffer_ptr->position.z = 0;
    quad_buffer_ptr->tex_coords.x = s0;
    quad_buffer_ptr->tex_coords.y = t1;
    quad_buffer_ptr->color.x = color_r;
    quad_buffer_ptr->color.y = color_g;
    quad_buffer_ptr->color.z = color_b;
    quad_buffer_ptr->color.w = color_a;
    quad_buffer_ptr->tex_index = tex_index;
    quad_buffer_ptr++;
}

void GLAPIENTRY opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        ERROR_C(severity, message);
    }
}

int texture_slots_find(unsigned int id) {
    for (int i = 0; i < texture_slot_index; i++) {
        if (texture_slots[i] == id) {
            return i;
        }
    }

    return -1;
}
