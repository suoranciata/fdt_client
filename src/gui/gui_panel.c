#include <ncurses.h>
#include "gui_tabs.h"
#include "gui_panel.h"

void gui_panel_init(void) {
}

void gui_panel_end(void) {
}

static WINDOW *pwin = NULL;

void gui_panel_show(void) {
    int x,y;
    getmaxyx(stdscr, y, x);
    pwin = newwin(y, x-1, 1, 0);
    wrefresh(pwin);
}

void gui_panel_hide(void) {
    wrefresh(pwin);
    delwin(pwin);
}


static panel *p;
void gui_panel_redraw() {
    mvwprintw(pwin, 0,0, "Pagina %d di %d\n", p->pagina, p->n_pagine);
    thread *t;
    for (int i=0;i<p->n_threads;++i) {
        t = p->threads[i];
        wprintw(pwin, "(%s) %s",
                t->capothread->m_id,
                t->capothread->titolo);
        if (t->capothread->autore != NULL) {
            wprintw(pwin, " di ");
            wattron(pwin, A_BOLD);
            wprintw(pwin, "%s", t->capothread->autore);
            wattroff(pwin, A_BOLD);
            wprintw(pwin, "\n");
        } else {
            wprintw(pwin, " di non autenticato\n");
        }
        if (t->ultimo != NULL) {
            wprintw(pwin, "\t(%s) %s", t->ultimo->m_id, t->ultimo->titolo);
            if (t->ultimo->autore != NULL) {
                wprintw(pwin, " di ");
                wattron(pwin, A_BOLD);
                wprintw(pwin, "%s", t->ultimo->autore);
                wattroff(pwin, A_BOLD);
                wprintw(pwin, "\n");
            } else {
                wprintw(pwin, " di non autenticato\n");
            }
        } else {
            wprintw(pwin, "\n");
        }
    }
    wrefresh(pwin);
}

void gui_panel_update(panel *p_new) {
    p = p_new;
    gui_panel_redraw();
}


