#ifndef types_h_guard
#define types_h_guard

#include <stdlib.h>

typedef struct post {
    char *titolo;
    char *autore;
    char *data_unparsed;
    char *contenuto;
    char *m_id;
} post;
static inline post *new_post() {
    post *p = malloc(sizeof(post));
    p->titolo = NULL;
    p->autore = NULL;
    p->data_unparsed = NULL;
    p->contenuto = NULL;
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
    if (p->contenuto != NULL)
        free(p->contenuto);
    if (p->m_id != NULL)
        free(p->m_id);
    free(p);
}

typedef struct thread {
    post *capothread;
    post *ultimo;
    int n_risposte;
    char *subforum;
} thread;
static inline thread *new_thread() {
    thread *t = malloc(sizeof(thread));
    t->capothread = NULL;
    t->ultimo = NULL;
    t->n_risposte = 0;
    t->subforum = NULL;
    return t;
}
static inline void delete_thread(thread *t) {
    if (t->capothread != NULL)
        delete_post(t->capothread);
    if (t->ultimo != NULL)
        delete_post(t->ultimo);
    if (t->subforum != NULL)
        free(t->subforum);
    free(t);
}

typedef struct forum {
    char *nome;
    thread *threads[15];
    int n_threads;
    int pagina;
    int n_pagine;
} forum;
static inline forum *new_forum() {
    forum *f = malloc(sizeof(forum));
    f->nome = NULL;
    f->n_threads = 0;
    f->pagina = 0;
    f->n_pagine = 0;
    return f;
}
static inline void delete_forum(forum *f) {
    if (f->nome != NULL)
        free(f->nome);
    if (f->n_threads > 0)
        while (f->n_threads --> 0)
            delete_thread( f->threads[f->n_threads] );
    free(f);
}

typedef struct panel {
    int pagina;
    int n_pagine;
    thread *threads[10];
    int n_threads;
} panel;
static inline panel *new_panel() {
    panel *p = malloc(sizeof(panel));
    p->pagina = 0;
    p->n_pagine = 0;
    p->n_threads = 0;
    return p;
}
static inline void delete_panel(panel *p) {
    if (p->n_threads > 0)
        while (p->n_threads --> 0)
            delete_thread( p->threads[p->n_threads] );
    free(p);
}

#endif
