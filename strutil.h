#ifndef STRING_UTILS_H__
#define STRING_UTILS_H__

#include <sys/types.h>

typedef struct string_t {
    u_char *data;
    size_t  len;
    size_t  size;
} string_t;

#endif
