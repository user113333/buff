#include "io.h"

#include "core.h"

char* read_file(const char* filename) {
    FILE* file;
    char* buffer;
    size_t size;

    file = fopen(filename, "rb");
    if(!file) {
        ERROR_C(filename, "Unable to open file");
    }
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = (char*) malloc((size+1) * sizeof(char*));
    fread(buffer, sizeof(char), size, file);
    buffer[size] = 0;
    fclose(file);

    return buffer;
}
