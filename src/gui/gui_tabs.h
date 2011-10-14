#ifndef gui_tabs_h_guard
#define gui_tabs_h_guard

typedef struct gui_tab {
    char *name;
    int changedUnseen;
    int selected;
} gui_tab;

void gui_tabs_init(void);
void gui_tabs_end(void);
void gui_tabs_addtab(char*);
void gui_tabs_delete(char*);
void gui_tabs_select(char*);
void gui_tabs_notifyChange(char*);
int gui_tabs_exists(char*);
#endif
