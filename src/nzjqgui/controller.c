#include "controller.h"

Controller* controller_create(void)
{
    Controller* ctrl = heap_new0(Controller);
    return ctrl;
}

void controller_destroy(Controller** controller)
{
    heap_delete(controller, Controller);
}

void controller_main_window(Controller* ctrl, Window* window)
{
    ctrl->main_window = window;
}

Window* controller_get_main_window(Controller* ctrl)
{
    return ctrl->main_window;
}

void controller_json_edit(Controller* ctrl, Edit* json_edit)
{
    ctrl->json_edit = json_edit;
}

Edit* controller_get_json_edit(Controller* ctrl)
{
    return ctrl->json_edit;
}