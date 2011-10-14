#ifndef gui_h_guard
#define gui_h_guard

void gui_init(void);
void gui_end(void);
void gui_loop(void);

void gui_register_key(int,void (*)(void));
void gui_unreg_key(int);

void gui_register_global_key(int,void (*)(void));
void gui_unreg_global_key(int);

void gui_clean_local_keys(void);

#endif
