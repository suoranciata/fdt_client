#ifndef util_xstring_h_guard
#define util_xstring_h_guard

#include <string.h>
#include <stdlib.h>

static inline char* xstrdup(char *src) {
    int len = strlen(src);
    char* dest = (char*) malloc (len+1);
    strcpy(dest, src);
    return dest;
}

static inline char* xstrcat(char *dest, char *src) {
    int len_src = strlen(src);
    int len_dest = strlen(dest);
    dest = (char*) realloc (dest, len_dest + len_src + 1);
    memcpy(dest + len_dest, src, len_src);
    dest[len_dest + len_src ] = '\0';
    return dest;
}

static inline char* xstrsub(char *src, int len) {
    char *dest = (char*) malloc (len+1);
    memcpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}

#endif
