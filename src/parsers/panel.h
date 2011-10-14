#ifndef parsers_panel_h_guard
#define parsers_panel_h_guard

#include <types/forum.h>

void parse_panel_init(void);
void parse_panel_end(void);
forum *parse_panel(char *page);

#endif
