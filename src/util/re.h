#ifndef util_re_h_guard
#define util_re_h_guard

#include <string.h>
#include <util/xstring.h>
#include <pcre.h>

static inline pcre *re_compile(char *re_str) {
    const char *errorptr;
    int erroroffset;
    return pcre_compile(re_str, 0, &errorptr, &erroroffset, NULL);
}

static inline char** re_exec(pcre *re, char *subject, int g_count) {
    int oveccount = 3 * (g_count+1);
    int ovector[oveccount];
    int rc = pcre_exec(re, NULL, subject, strlen(subject), 0, 0, ovector, oveccount);
    // rc = -1 if no match, rc = 0 if ovector is too small
    if (rc <= 0) return NULL;
    char** res = (char**) malloc ((g_count+1)*sizeof(char*));
    for (int i=0;i<g_count+1;++i) {
        int s_start = ovector[i*2];
        int s_end = ovector[i*2+1];
        int s_len = s_end - s_start;
        res[i] = xstrsub(subject + s_start, s_len);
    }
    return res;
}

static inline void re_free_matches(char **matches, int g_count) {
    for (int i=0;i<(g_count+1);++i) {
        free(matches[i]);
    }
    free(matches);
}

#endif
