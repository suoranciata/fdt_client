#ifndef types_thread_h_guard
#define types_thread_h_guard

#include <stdlib.h>
#include <types/post.h>

typedef struct thread {
    post *op;
    post *ultimo;
    int n_risposte;
    char *subforum;
} thread;

static inline thread *new_thread() {
    thread *t = (thread*) malloc (sizeof(thread));
    t->op = NULL;
    t->ultimo = NULL;
    t->n_risposte = 0;
    t->subforum = NULL;
    return t;
}

static inline void delete_thread(thread *t) {
    if (t->op != NULL)
        delete_post(t->op);
    if (t->ultimo != NULL)
        delete_post(t->ultimo);
    if (t->subforum != NULL)
        free(t->subforum);
    free(t);
}

#endif
