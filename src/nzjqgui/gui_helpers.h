#ifndef GUI_HELPERS_H
#define GUI_HELPERS_H

/* GUI helper functions */

#include <nappgui.h>

__EXTERN_C

V2Df window_calculate_center_origin(S2Df window_size, S2Df parent_size, V2Df parent_pos);
void window_set_center_parent_origin(Window* window, S2Df parent_size, V2Df parent_pos);
void window_set_center_screen_origin(Window* window);

__END_C

#endif
