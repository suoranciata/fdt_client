#include <stdlib.h>
#include <curses.h>
#include <ncurses.h>
#include <string.h>

#include "gui_tabs.h"
#include "gui.h"

#define MAX_TABS 20
static gui_tab *tabs[MAX_TABS];
static int tabs_index = 0;

WINDOW *tabs_win = NULL;

void gui_tabs_init(void) {
    int cols = getmaxx(stdscr);
    tabs_win = newwin(1, cols, 0, 0);
    wrefresh(tabs_win);
}
void gui_tabs_end(void) {
    delwin(tabs_win);
    refresh();
}

void gui_tabs_refresh(void) {
    mvwprintw(tabs_win, 0, 0, "");
    for (int i=0;i<tabs_index;++i) {
        gui_tab *tab = tabs[i];
        if (tab->selected) {
            wprintw(tabs_win, "[ ");
            wattron(tabs_win, A_BOLD);
            wprintw(tabs_win, "%s", tab->name);
            wattroff(tabs_win, A_BOLD);
            wprintw(tabs_win, " ] ");
        } else if (tab->changedUnseen) {
            wprintw(tabs_win, "[+");
            wprintw(tabs_win, "%s", tab->name);
            wprintw(tabs_win, " ] ");
        } else {
            wprintw(tabs_win, "[ ");
            wprintw(tabs_win, "%s", tab->name);
            wprintw(tabs_win, " ] ");
        }
    }
    //pulisci fino a fine linea
    int x;
    getyx(tabs_win, x, x);//funziona solo perché y viene valorizzato prima dalla macro, quindi scrive due volte su x, la seconda col valore giusto
    int cols = getmaxx(stdscr);
    for (;x<cols;++x) {
        wprintw(tabs_win, " ");
    }
    wrefresh(tabs_win);
}

void gui_tabs_addtab(char *name) {
    gui_tab *tab = malloc(sizeof(gui_tab));
    tab->name = malloc(strlen(name)+1);
    memcpy(tab->name, name, strlen(name)+1);
    tab->changedUnseen = FALSE;
    tab->selected = tabs == NULL; //autoselect if first
    tabs[tabs_index++] = tab;
    gui_tabs_refresh();
}

gui_tab *gui_tabs_findtab(char *name) {
    for (int i=0;i<tabs_index;++i) {
        gui_tab *tab = tabs[i];
        char *tabname = tab->name;
        if (strcmp(tabname, name) == 0)
            return tab;
    }
    return NULL;
}

int gui_tabs_exists(char *name) {
    return gui_tabs_findtab(name) != NULL;
}

void gui_tabs_delete(char *name) {
    for (int i=0;i<tabs_index;++i) {
        gui_tab *tab = tabs[i];
        char *tabname = tab->name;
        if (strcmp(tabname, name) == 0) {
            if (tab->selected) {
                if (i<tabs_index-1) {
                    tabs[i+1]->selected = TRUE;
                } else if (i>0) {
                    tabs[i+1]->selected = TRUE;
                } else {
                    // nessun tab rimasto
                }
            }
            free(tab);
            // scala tutti i tab successivi di uno
            for (;i<tabs_index;++i) {
                tabs[i] = tabs[i-1];
            }
            tabs[i-1] = NULL;
            tabs_index--;
            gui_tabs_refresh();
            return;
        }
    }
}

void gui_tabs_select(char *name) {
    for (int i=0;i<tabs_index;++i) {
        gui_tab *tab = tabs[i];
        char *tabname = tab->name;
        if (strcmp(tabname, name) == 0) {
            tab->selected = TRUE;
            tab->changedUnseen = FALSE;
        } else {
            tab->selected = FALSE;
        }
    }
    gui_tabs_refresh();
}

void gui_tabs_notifyChange(char *name) {
    gui_tabs_findtab(name)->changedUnseen = TRUE;
    gui_tabs_refresh();
}

