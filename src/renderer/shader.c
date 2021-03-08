#include "shader.h"

#include <string.h>
#include <GLFW/glfw3.h>

#include "editor/io.h"
#include "core.h"

static const char* frag_identifier = "#shader frag\n";
static const char* vert_identifier = "#shader vert\n";

GLuint shader_compile(const char* source, const GLenum type) {
    GLint compile_status;
    GLuint handle = glCreateShader(type);
    glShaderSource(handle, 1, &source, 0);
    glCompileShader(handle);

    glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_status);
    if(compile_status == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(handle, sizeof(messages), 0, &messages[0]);
        ERROR(messages);
    }
    return handle;
}

GLuint shader_load_from_source(const char* vert_source, const char* frag_source) {
    GLuint handle = glCreateProgram();
    GLint link_status;

    GLuint vert_shader = shader_compile(vert_source, GL_VERTEX_SHADER);
    GLuint frag_shader = shader_compile(frag_source, GL_FRAGMENT_SHADER);

    glAttachShader(handle, vert_shader);
    glAttachShader(handle, frag_shader);
    glLinkProgram(handle);
    

    glGetProgramiv(handle, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        GLchar messages[256];
        glGetProgramInfoLog(handle, sizeof(messages), 0, &messages[0]);
        ERROR(messages);
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    
    return handle;
}

GLuint shader_load_from_file(const char* filename) {
    const char* buffer = read_file(filename);
    const char* frag_pos = strstr(buffer, frag_identifier);
    const char* vert_pos = strstr(buffer, vert_identifier);
    
    const char* frag_end = (frag_pos < vert_pos ? vert_pos : buffer + strlen(buffer));
    const char* vert_end = (vert_pos < frag_pos ? frag_pos : buffer + strlen(buffer));
    const char* frag_start = frag_pos + (strlen(frag_identifier));
    const char* vert_start = vert_pos + (strlen(vert_identifier));
    
    int frag_size = strlen(frag_start) - strlen(frag_end);
    int vert_size = strlen(vert_start) - strlen(vert_end);

    char frag_source[frag_size + 1];
    memcpy(frag_source, frag_start, frag_size);
    frag_source[frag_size] = '\0';
    
    char vert_source[vert_size + 1];
    memcpy(vert_source, vert_start, vert_size);
    vert_source[vert_size] = '\0';
    
    return shader_load_from_source(vert_source, frag_source);
}
