#include <stdio.h>
#include <string.h>

#include <util/xstring.h>
#include <util/re.h>
#include <http/client.h>
#include <types/post.h>
#include <types/thread.h>
#include <types/forum.h>
#include <parsers/panel.h>

void util_strings(void) {
    printf("-- testing util/strings.h\n");
    char *s = xstrdup("abc");
    printf("%s\n", s);
    char *s2 = xstrdup("def");
    printf("%s\n", s2);
    s = xstrcat(s, s2);
    printf("%s\n", s);
    char *s3 = xstrsub(s + 2, 1);
    free(s);
    free(s2);
    printf("%s\n", s3);
    free(s3);
}

void util_re(void) {
    printf("-- testing util/re.h\n");
    pcre *re = re_compile("ci(a\\d)o");
    char* s = xstrdup("cia3o");
    char** res = re_exec(re, s, 1);
    printf("res[0] = %s\n", res[0]);
    printf("res[1] = %s\n", res[1]);
    re_free_matches(res, 1);
    free(re);
    free(s);
}

void http_client(void) {
    printf("-- testing http/client.c\n");
    char *page = http_get("/disclaimer.aspx");
    printf("expected 2545 bytes.\n");
    printf("loaded   %d bytes.\n", strlen(page));
    free(page);
}

void types(void) {
    printf("-- testing types/post.h\n");
    post *p = new_post();
    p->m_id = xstrdup("1233412");
    p->titolo = xstrdup("ciao");
    delete_post(p);
}

int main(int argc, char **argv) {
    util_strings();
    util_re();
    http_client();
    types();
/*
    // types
    forum *f = new_forum(PANEL);
    delete_forum(f);

    parse_panel_init();
    char *page = http_get("/");
    //printf("----- page -----\n%s\n----- page -----", page);
    forum *f = parse_panel(page);
    free(page);
    delete_forum(f);
    parse_panel_end();*/
    printf("-- end\n");
    return 0;
}
