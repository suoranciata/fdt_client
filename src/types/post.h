#ifndef types_post_h_guard
#define types_post_h_guard

#include <stdio.h>
#include <stdlib.h>

typedef struct post {
    char *titolo;
    char *autore;
    char *data_unparsed;
    char *m_id;
} post;

static inline post *new_post() {
    post *p = (post*) malloc (sizeof(post));
    p->titolo = NULL;
    p->autore = NULL;
    p->data_unparsed = NULL;
    p->m_id = NULL;
    return p;
}

static inline void delete_post(post *p) {
    if (p->titolo != NULL)
        free(p->titolo);
    if (p->autore != NULL)
        free(p->autore);
    if (p->data_unparsed != NULL)
        free(p->data_unparsed);
    if (p->m_id != NULL)
        free(p->m_id);
    free(p);
}

#endif
