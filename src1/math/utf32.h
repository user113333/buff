#ifndef UTF32_H
#define UTF32_H

typedef unsigned int utf32; // uint32_t

utf32* to_utf32(char* str);
int utf32_len(utf32* str_utf32);

#endif // UTF32_H
