#include "main_menu.h"
#include "controller.h"


#define FILE_TYPE_ARR_SIZE 1


/* Определения "наперёд"
 * --
 */

static Menu* i_file_menu_create(Controller* ctrl);
static void i_OnFileOpen(Controller* ctrl, Event* e);
static void i_OnFileSave(Controller* ctrl, Event* e);
static void i_OnFileQuit(Controller* ctrl, Event* e);
static Menu* i_help_menu_create(Controller* ctrl);
static void i_OnHelpAbout(Controller* ctrl, Event* e);
static void i_OnHelpManual(Controller* ctrl, Event* e);


/* Публичные функции
 * --
 */

Menu* main_menu_create(Controller* ctrl)
{
    Menu* menu = menu_create();

    MenuItem* file_item = menuitem_create();
    MenuItem* help_item = menuitem_create();

    Menu* file_submenu = i_file_menu_create(ctrl);
    Menu* help_submenu = i_help_menu_create(ctrl);

    menuitem_text(file_item, "Файл");
    menuitem_text(help_item, "Справка");

    menuitem_submenu(file_item, &file_submenu);
    menuitem_submenu(help_item, &help_submenu);

    menu_item(menu, file_item);
    menu_item(menu, help_item);

    return menu;
}


/* Закрытые функции
 * --
 */

/* Меню Файл */

static Menu* i_file_menu_create(Controller* ctrl)
{
    Menu* menu = menu_create();

    MenuItem* open_item = menuitem_create();
    MenuItem* save_item = menuitem_create();
    MenuItem* sepr_item = menuitem_separator();
    MenuItem* quit_item = menuitem_create();

    menuitem_text(open_item, "Открыть");
    menuitem_text(save_item, "Сохранить");
    menuitem_text(quit_item, "Выход");

    menuitem_OnClick(open_item, listener(ctrl, i_OnFileOpen, Controller));
    menuitem_OnClick(save_item, listener(ctrl, i_OnFileSave, Controller));
    menuitem_OnClick(quit_item, listener(ctrl, i_OnFileQuit, Controller));

    menu_item(menu, open_item);
    menu_item(menu, save_item);
    menu_item(menu, sepr_item);
    menu_item(menu, quit_item);

    return menu;
}

static void i_OnFileOpen(Controller* ctrl, Event* e)
{
    char_t* path = NULL;
    String* file_content = NULL;
    file_type_t file_type;

    bstd_printf("[ИНФ] Меню: Файл -> Открыть\n");

    Window* main_window = controller_get_main_window(ctrl);
    char_t* file_types[FILE_TYPE_ARR_SIZE] = { "*.json" };

    path = comwin_open_file(main_window, file_types, FILE_TYPE_ARR_SIZE, NULL);

    if (path != NULL) {
        bstd_printf("[ИНФ] Выбран файл: %s\n", path);
        if (hfile_exists(path, &file_type) && file_type == ekARCHIVE) {
            bstd_printf("[ИНФ] Найден файл: %s\n", path);
            ferror_t err;
            file_content = hfile_string(path, &err);
            if (err == ekFOK && file_content != NULL) {
                Edit* json_edit = controller_get_json_edit(ctrl);
                edit_text(json_edit, tc(file_content));
            }
        }
    }

    str_destopt(&file_content);
    unref(e);
}

static void i_OnFileSave(Controller* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Файл -> Сохранить\n");
    unref(e);
}

static void i_OnFileQuit(Controller* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Файл -> Выход\n");
    osapp_finish();
    unref(ctrl);
    unref(e);
}

/* Меню Справка */

static Menu* i_help_menu_create(Controller* ctrl)
{
    Menu* menu = menu_create();

    MenuItem* about_item = menuitem_create();
    MenuItem* manual_item = menuitem_create();

    menuitem_text(about_item, "О программе");
    menuitem_text(manual_item, "Руководство пользователя");

    menuitem_OnClick(about_item, listener(ctrl, i_OnHelpAbout, Controller));
    menuitem_OnClick(manual_item, listener(ctrl, i_OnHelpManual, Controller));

    menu_item(menu, about_item);
    menu_item(menu, manual_item);

    return menu;
}

static void i_OnHelpAbout(Controller* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Справка -> О программе\n");
    unref(e);
}

static void i_OnHelpManual(Controller* ctrl, Event* e)
{
    bstd_printf("[ИНФ] Меню: Справка -> Руководство пользователя\n");
    unref(e);
}
