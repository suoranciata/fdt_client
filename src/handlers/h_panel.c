#include <stdio.h>
#include "../gui/gui.h"
#include "../gui/gui_tabs.h"
#include "../gui/gui_panel.h"
#include "../net/http_client.h"
#include "../parsers/p_panel.h"
#include "../core/types.h"


int pagina = 1;
int n_pagine = 0;

void nextpage(void);
void prevpage(void);
void loadpage(void);

void h_panel_goto_panel() {
    gui_clean_local_keys();
    if (!gui_tabs_exists("panel")) {
        gui_tabs_addtab("panel");
        gui_tabs_select("panel");
    }
    gui_panel_show();
    gui_register_key('r', loadpage);
    gui_register_key(260, prevpage);
    gui_register_key(261, nextpage);
    loadpage();
}

void nextpage() {
    if (pagina == n_pagine) return;
    pagina++;
    loadpage();
}

void prevpage() {
    if (pagina == 1) return;
    pagina--;
    loadpage();
}

void loadpage() {
    char url[35];
    sprintf(url,"/hp.aspx?f_id=130&type=2&Page=%d",pagina);
    panel *dati = p_panel_parse( http_get( url ) );
    gui_panel_update(dati);
    pagina = dati->pagina;
    n_pagine = dati->n_pagine;
    delete_panel(dati);
}

