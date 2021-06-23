#include "utf32.h"

#include <utf8-utils.h>

utf32* to_utf32(char* str) {
    int len = utf8_strlen(str);
    utf32* str_utf32 = malloc(sizeof(utf32) * (len + 1));

    int i = 0;

    while (*str) {
        str_utf32[i] = utf8_to_utf32(str);
        str += utf8_surrogate_len(str);
        i++;
    }

    str_utf32[i] = 0;
    return str_utf32;
}

int utf32_len(utf32* str_utf32) {
    int len = 0;
    
    while (*str_utf32 != '\0') {
        str_utf32++;
        len++;
    }
    
    return len;
}
