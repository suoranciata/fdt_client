#ifndef types_forum_h_guard
#define types_forum_h_guard

#include <stdlib.h>
#include <types/thread.h>

static int FORUM = 0;
static int PANEL = 1;

typedef struct forum {
    char *nome;
    thread** threads;
    int n_threads;
    int pagina;
    int n_pagine;
    int tipo;
} forum;

static inline forum *new_forum(int tipo) {
    forum *f = malloc(sizeof(forum));
    f->nome = NULL;
    f->n_threads = 0;
    f->pagina = 0;
    f->n_pagine = 0;
    f->tipo = tipo;
    if (tipo == FORUM)
        f->threads = (thread**) malloc (sizeof(thread*) * 15);
    else if (tipo == PANEL)
        f->threads = (thread**) malloc (sizeof(thread*) * 10);
    return f;
}

static inline void delete_forum(forum *f) {
    if (f->nome != NULL)
        free(f->nome);
    for (int i=0;i<f->n_threads;++i)
        free(f->threads[i]);
    free(f->threads);
    free(f);
}

#endif
