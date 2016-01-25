#ifndef UTIL_DEBUG_H__
#define UTIL_DEBUG_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/syscall.h>

#ifndef _DEBUG
#define _DEBUG 0
#endif

#define FMT_R(msg)  "\x1b[31m\x1b[1m" msg "\x1b[0m\n"
#define FMT_G(msg)  "\x1b[32m\x1b[1m" msg "\x1b[0m\n"
#define FMT_B(msg)  "\x1b[34m\x1b[1m" msg "\x1b[0m\n"
#define FMT_Y(msg)  "\x1b[33m\x1b[1m" msg "\x1b[0m\n"
#define FMT_M(msg)  "\x1b[35m\x1b[1m" msg "\x1b[0m\n"
#define TCKL        "\033[0G\033[0K"

#if _DEBUG > 0
#include <assert.h>
#define __FN__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define debug_print(msg,...)                                            \
    fprintf(stdout, "%s:%d(%s): "msg,__FN__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#else
#define debug_print(msg, ...) ((void)0)
#endif

#define ASSERT(x)                                                       \
    if (!(x)) {                                                         \
        debug_print(FMT_R("FAILED: %s"), #x);                           \
        exit(1);                                                        \
    }

#define thread_id() syscall(__NR_gettid)



#ifdef __cplusplus
extern "C" {
#endif

static inline void print_string(void *data, int length)
{
    int i;
    char *str = (char*) data;
    for (i = 0; i < length; i++) {
        if (str[i] >= 32 && str[i] <= 126)
            printf("%c", str[i]);
        else
            printf("\\x%02x", (uint8_t) str[i]);
    }
}

static inline void print_hex_string(void *data, int length, int max_size)
{
    int i;
    uint8_t *pstr = (uint8_t *)data;
    if (max_size == 0) max_size = 32;
    length = length > max_size ? max_size : length;
    for (i = 0; i < length; i++)
        printf("%02x ", pstr[i]);
    if (length > 32)
        printf("...");
}

static inline const char *hexstr(void *in, size_t len, char *out)
{
    size_t i, off;
    unsigned char *p = (unsigned char *)in;
    for(i=0, off=0; i<len; ++i) {
        off += sprintf(out+off, "%02x", *p++);
    }
    out[off] = 0;

    return out;
}


#ifdef __cplusplus
}
#endif

#endif
