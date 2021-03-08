#ifndef GAP_H
#define GAP_H

#include "buffer.h"

void gap_reposition(buffer_t* buffer);
int gap_is_ptr_gap(buffer_t* buffer, char_t* ptr);

#endif // GAP_H
