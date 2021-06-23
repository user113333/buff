#ifndef SHADER_H
#define SHADER_H

unsigned int shader_load_from_source(const char* vert_source, const char* frag_source);
unsigned int shader_load_from_file(const char* filename);

#endif // SHADER_H
