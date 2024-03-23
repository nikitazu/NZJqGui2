#include "gui_helpers.h"

V2Df window_calculate_center_screen_origin(S2Df window_size, S2Df screen_size)
{
    real32_t window_x = screen_size.width / 2 - window_size.width / 2;
    real32_t window_y = screen_size.height / 2 - window_size.height / 2;
    return v2df(window_x, window_y);
}

void window_set_center_screen_origin(Window* window)
{
    S2Df screen_size = gui_resolution();
    S2Df window_size = window_get_size(window);
    V2Df center_orig = window_calculate_center_screen_origin(window_size, screen_size);

    window_origin(window, center_orig);
}
