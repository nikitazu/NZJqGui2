#ifndef CONTROLLER_H
#define CONTROLLER_H

/* Контроллер */

#include <nappgui.h>

typedef struct _controller_t Controller;

struct _controller_t
{
    Window* main_window;
    Edit* json_edit;
};

__EXTERN_C

Controller* controller_create(void);
void controller_destroy(Controller** controller);
void controller_main_window(Controller* ctrl, Window* window);
Window* controller_get_main_window(Controller* ctrl);
void controller_json_edit(Controller* ctrl, Edit* json_edit);
Edit* controller_get_json_edit(Controller* ctrl);

__END_C

#endif
