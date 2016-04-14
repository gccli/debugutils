#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include "utils/urlescape.h"

#ifdef __cplusplus
extern "C" {
#endif

static int url_isunreserved(unsigned char in)
{
  switch (in) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case 'a': case 'b': case 'c': case 'd': case 'e':
    case 'f': case 'g': case 'h': case 'i': case 'j':
    case 'k': case 'l': case 'm': case 'n': case 'o':
    case 'p': case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E':
    case 'F': case 'G': case 'H': case 'I': case 'J':
    case 'K': case 'L': case 'M': case 'N': case 'O':
    case 'P': case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case '-': case '.': case '_': case '~':
      return 1;
    default:
      break;
  }
  return 0;
}


int url_escape_ex(const char *string, size_t length, escape_string *s)
{
    char *ns;
    char *testing_ptr = NULL;
    unsigned char in; /* we need to treat the characters unsigned */
    size_t newlen = length+1;
    size_t strindex=0;

    if (s->data && s->size > 0) {
        if (s->size < length) {
            s->data = realloc(s->data, length+4);
            s->size = length;
        }
    } else {
        s->data = calloc(1, length+4);
        s->size = length;
    }

    ns = s->data;

    if(!ns)
        return ENOMEM;

    //len = alloc-1;
    while(length--) {
        in = *string;

        if(url_isunreserved(in))
            /* just copy this */
            ns[strindex++]=in;
        else {
            /* encode it */
            newlen += 2; /* the size grows with two, since this'll become a %XX */
            if(newlen > s->size) {
                s->size *= 2;
                testing_ptr = realloc(s->data, s->size+4);
                if(!testing_ptr) {
                    free( s->data ); s->data = NULL;
                    return ENOMEM;
                }
                else {
                    s->data = testing_ptr;
                    ns = s->data;
                }
            }

            snprintf(&ns[strindex], 4, "%%%02X", in);
            strindex+=3;
        }
        string++;
    }
    ns[strindex]=0; /* terminate it */
    s->len = strindex;

    return 0;
}

int url_unescape_ex(const char *string, size_t length, escape_string *s)
{
    char *ns;
    size_t alloc;

    unsigned char in;
    size_t strindex=0;
    unsigned long hex;

    if (s->data && s->size > 0) {
        if (s->size < length) {
            s->data = realloc(s->data, length+4);
            s->size = length;
        }
    } else {
        s->data = calloc(1, length+4);
        s->size = length;
    }

    ns = s->data;
    alloc = length+1;


    if(!ns)
        return ENOMEM;

    while(--alloc > 0) {
        in = *string;
        if(('%' == in) && (alloc > 2) && isxdigit(string[1]) && isxdigit(string[2])) {
            /* this is two hexadecimal digits following a '%' */
            char hexstr[3];
            char *ptr;
            hexstr[0] = string[1];
            hexstr[1] = string[2];
            hexstr[2] = 0;

            hex = strtoul(hexstr, &ptr, 16);
            in = hex & 0xff;
            string+=2;
            alloc-=2;
        }

        ns[strindex++] = in;
        string++;
    }
    ns[strindex] = 0;
    s->len = strindex;

    return 0;
}

#ifdef __cplusplus
}
#endif

#ifdef _TEST
#include <unistd.h>
#include "utilfile.h"
#include "utilsha1.h"
int main(int argc, char *argv[])
{
    int fd;
    char name[64], *buf, *out = NULL, *verify;
    size_t len, outlen;
    buf = get_file_buffer(argv[1], &len);
    if (buf) {
        if (argc > 2) {
            if (url_unescape(buf, len, &out, &outlen) == 0) {
                printf("%.*s\n", (int)outlen, out);
                if ((verify = url_escape(out, outlen))) {
                    fd = get_tmpfile(name, 0666, NULL, NULL);
                    if (fd > 0) {
                        printf("verify file:%s\n", name);
                        write(fd, verify, strlen(verify));
                        close(fd);
                    }
                }
            }
        } else {
            if ((out = url_escape(buf, len)))
                printf("%s\n", out);
        }
        free(buf);
        if (out) free(out);
    }
}
#endif
