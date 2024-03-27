#include "gui_helpers.h"

V2Df window_calculate_center_origin(S2Df window_size, S2Df parent_size, V2Df parent_pos)
{
    real32_t window_x = (parent_size.width - window_size.width) / 2 + parent_pos.x;
    real32_t window_y = (parent_size.height - window_size.height) / 2 + parent_pos.y;

    return v2df(window_x, window_y);
}

void window_set_center_parent_origin(Window* window, S2Df parent_size, V2Df parent_pos)
{
    S2Df window_size = window_get_size(window);
    V2Df center_orig = window_calculate_center_origin(window_size, parent_size, parent_pos);

    window_origin(window, center_orig);
}

void window_set_center_screen_origin(Window* window)
{
    S2Df screen_size = gui_resolution();

    window_set_center_parent_origin(window, screen_size, kV2D_ZEROf);
}
