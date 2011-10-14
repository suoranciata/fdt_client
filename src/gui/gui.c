#include <ncurses.h>
#include <glib.h>

#include "gui.h"
#include "gui_tabs.h"
#include "gui_panel.h"

void gui_init(void) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();
    refresh();
    gui_tabs_init();
    gui_panel_init();
}

void gui_end(void) {
    gui_panel_end();
    gui_tabs_end();
    endwin();
}

typedef struct k_h {
    int key;
    void (*handler)(void);
} k_h;
static GList *keys = NULL;
static GList *global_keys = NULL;

#define regkey(ks,k,h) \
    k_h *kh = malloc(sizeof(k_h));\
    kh->key = k;\
    kh->handler = h;\
    ks = g_list_append(ks, kh);

void gui_register_key(int k, void handler(void)) {
    regkey(keys,k,handler);
}
void gui_register_global_key(int k, void handler(void)) {
    regkey(global_keys, k, handler);
}

#define unregkey(ks,k) \
    GList *node = ks;\
    while (node != NULL) {\
        k_h *kh = (k_h*) node->data;\
        if (kh->key == k) {\
            free(kh);\
            ks = g_list_remove_link(ks, node);\
            break;\
        }\
        node = node->next;\
    }

void gui_unreg_key(int k) {
    unregkey(keys, k);
}
void gui_unreg_global_key(int k) {
    unregkey(global_keys, k);
}

void gui_clean_local_keys() {
    GList *node = keys;
    while (node != NULL) {
        node = g_list_remove_link(keys, node);
    }
}

void gui_loop(void) {
    int c;
    while ((c=getch()) != EOF) {
        GList *node = global_keys;
        while (node != NULL) {
            k_h *kh = (k_h*) node->data;
            if (kh->key == c) {
                kh->handler();
                break;
            }
            node = node->next;
        }
        node = keys;
        while (node != NULL) {
            k_h *kh = (k_h*) node->data;
            if (kh->key == c) {
                kh->handler();
                break;
            }
            node = node->next;
        }
    }
}


