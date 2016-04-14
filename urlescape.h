#ifndef URL_ESCAPE_H___
#define URL_ESCAPE_H___

#ifdef __cplusplus
extern "C" {
#endif

typedef struct escape_string_s {
    char  *data;
    size_t len;
    size_t size;
} escape_string;

//char *url_escape(const char *string, size_t length);
int url_escape_ex(const char *string, size_t length, escape_string *s);
//int url_unescape(const char *string, size_t length, char **ostring, size_t *olen);
int url_unescape_ex(const char *string, size_t length, escape_string *s);

#ifdef __cplusplus
}
#endif

#endif
