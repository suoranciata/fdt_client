#include <stdio.h>

#include "parsers/p_panel.h"
#include "gui/gui.h"
#include "handlers/h_panel.h"

void handle_shutdown(void) {
    gui_end();
    p_panel_end();
    exit(0);
}

int main(int argc, char **argv) {
    gui_init();
    p_panel_init();
    gui_register_global_key('q', handle_shutdown);
    gui_register_global_key('p', h_panel_goto_panel);
    h_panel_goto_panel();
    gui_loop();
    return 0;
}
